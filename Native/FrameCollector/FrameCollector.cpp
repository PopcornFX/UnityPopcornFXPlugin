//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "FrameCollector.h"
#include "ParticleMaterialData.h"
#include "ParticleCollectedFrameData.h"
#include "DrawCalls/ParticleBaseDrawCall.h"
#include "DrawCalls/BillboardDrawCall.h"
#include "DrawCalls/RibbonDrawCall.h"

#include <pk_particles/include/Renderers/Std/renderer_std.h>
#include <pk_kernel/include/kr_delegates.h>
#include <pk_kernel/include/kr_refcounted_buffer.h>
#include <pk_particles/include/ps_stats.h>
#include <pk_drawers/include/dr_scene.h>
#include <pk_kernel/include/kr_caps_cpu.h>

#include "ImplemGraphicsAPI/RenderAPI_Data.h"

//----------------------------------------------------------------------------

CFrameCollector::CFrameCollector()
	: m_Collecting(null)
	, m_NextToRender(null)
	, m_CollectedFrameCount(0)
	, m_Rendering(null)
	, m_DelayUnityCallbacks(false)
	, m_AreBoundEnabled(true)
{
}

//----------------------------------------------------------------------------

CFrameCollector::~CFrameCollector()
{
	PK_SAFE_DELETE(m_RenderApiData);
	PK_SCOPEDLOCK(m_FullRenderLock);
	PK_SCOPEDLOCK(m_NextToRenderLock);
	m_BillboardDrawCalls.Clear();
	m_RibbonDrawCalls.Clear();
	PK_ASSERT(m_NextToRender != m_Rendering);
	PK_SAFE_DELETE(m_NextToRender);
	PK_SAFE_DELETE(m_Rendering);
}

//----------------------------------------------------------------------------

bool	CFrameCollector::InitializeAPI(UnityGfxRenderer deviceType)
{
	m_RenderApiData = IRenderAPIData::GetRenderAPISpecificData(deviceType);
	if (m_RenderApiData != null)
		m_RenderApiData->LoadContextFunctions();
	return PK_VERIFY(m_RenderApiData != null);
}

//----------------------------------------------------------------------------

void	CFrameCollector::InstallToMediumCollection(CParticleMediumCollection &collection, bool enableMeshes)
{
	m_RenderersWalker.Enable(Drawers::Renderer_Billboard);
	m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Ribbon);
	if (enableMeshes)
	{
		m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Mesh);
	}
	//m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Light);

	//
	// We must install ParticleRenderer mapping
	// or PopcornFX will think we don't know how to render them
	//

	CParticleRenderManager_Std					*renderManager = (CParticleRenderManager_Std*)(collection.RenderManager());
	CParticleRenderManager_Std::TDrawerMapping	*mapping = renderManager->DrawerMapping();

	PK_ASSERT_MESSAGE(!mapping->IsCustomRemapped<CParticleRenderer_Billboard>(), "Renderer billboard already remapped by someone else !");
	mapping->SetRemap<CParticleRenderer_Billboard>(FastDelegate<CbDrawerRemapperStd>(this, &CFrameCollector::Drawer_BuildRemapperWrapper<CParticleRenderer_Billboard>));

	PK_ASSERT_MESSAGE(!mapping->IsCustomRemapped<CParticleRenderer_Ribbon>(), "Renderer ribbon already remapped by someone else !");
	mapping->SetRemap<CParticleRenderer_Ribbon>(FastDelegate<CbDrawerRemapperStd>(this, &CFrameCollector::Drawer_BuildRemapperWrapper<CParticleRenderer_Ribbon>));

	if (enableMeshes)
	{
		PK_ASSERT_MESSAGE(!mapping->IsCustomRemapped<CParticleRenderer_Mesh>(), "Renderer mesh already remapped by someone else !");
		mapping->SetRemap<CParticleRenderer_Mesh>(FastDelegate<CbDrawerRemapperStd>(this, &CFrameCollector::Drawer_BuildRemapperWrapper<CParticleRenderer_Mesh>));
	}
	// PopcornFX will now call Drawer_BuildRemapperWrapper to create
	// our CParticleDrawer_RenderManagerMaster associated with CParticleRenderer(s)
	// CParticleDrawer_RenderManagerMaster could also be used to cache data
}

//----------------------------------------------------------------------------

void	CFrameCollector::GameThread_CreatePreloadedDrawCalls()
{
	// We need to execute all the Unity callbacks before any push back in the draw-call array (pointers will be invalid otherwise)
	GameThread_ExecUnityCallbacks();
	// We start by creating the draw calls for the preloaded effects:
	if (!m_PreloadedBillboardMatDesc.Empty() ||
		!m_PreloadedRibbonMatDesc.Empty() ||
		!m_PreloadedMeshMatDesc.Empty())
	{
		GameThread_WaitForRenderThreadIFN(); // Sync render thread to load new effects

		// We create the draw-calls for all the preloaded material descs to avoid dynamically creating the vertex and index buffers
		if (!m_PreloadedBillboardMatDesc.Empty())
		{
			CreateCompatibleDrawCalls(m_PreloadedBillboardMatDesc.View(), m_BillboardDrawCalls);
			m_PreloadedBillboardMatDesc.Clear();
		}
		if (!m_PreloadedRibbonMatDesc.Empty())
		{
			CreateCompatibleDrawCalls(m_PreloadedRibbonMatDesc.View(), m_RibbonDrawCalls);
			m_PreloadedRibbonMatDesc.Clear();
		}
		if (!m_PreloadedMeshMatDesc.Empty())
		{
			// Do nothing here...
			m_PreloadedMeshMatDesc.Clear();
		}
	}
}

//----------------------------------------------------------------------------

void	CFrameCollector::GameThread_CreateMeshTransforms(	const Drawers::SMesh_DrawRequest &drawRequest,
															const TMemoryView<CParticleMaterialDescMesh::SMeshDescriptorData> &meshDescriptors)
{
	m_BB.Prepare(TMemoryView<const Drawers::SMesh_DrawRequest>(drawRequest));

	u32						totalParticleCount = m_BB.TotalParticleCount();
	const u32				meshDescriptorCount = meshDescriptors.Count();
	TMemoryView<const u32>	perMeshParticleCount;

	if (meshDescriptorCount > 1)
		perMeshParticleCount = m_BB.PerMeshParticleCount();
	else
		perMeshParticleCount = TMemoryView<const u32>(totalParticleCount);

	PK_ASSERT(meshDescriptorCount == perMeshParticleCount.Count());

	// We call the C# to get proper handles on the per instance parameters
	for (u32 i = 0; i < meshDescriptorCount; ++i)
	{
		CParticleMaterialDescMesh::SMeshDescriptorData	&curMeshDescriptor = meshDescriptors[i];
		u32												curMeshDescriptorParticleCount = perMeshParticleCount[i];
		// Transforms and Colors:
		u32												meshInstanceBufferSize = curMeshDescriptorParticleCount * (sizeof(CFloat4x4) + sizeof(CFloat4));
		bool											needUpdateBuffer = false;

		meshInstanceBufferSize = Mem::Align(meshInstanceBufferSize, 0x1000); // Alloc per block of 4096

		if (meshInstanceBufferSize > curMeshDescriptor.m_MeshInstanceBufferSize)
		{
			if (curMeshDescriptor.m_MeshInstanceBuffer != null)
			{
				PK_FREE(curMeshDescriptor.m_MeshInstanceBuffer);
			}
			// Resize the per instance buffer IFN:
			curMeshDescriptor.m_MeshInstanceBuffer = PK_MALLOC_ALIGNED(meshInstanceBufferSize, 0x10);
			curMeshDescriptor.m_MeshInstanceBufferSize = meshInstanceBufferSize;
			needUpdateBuffer = true;
		}

		void		*curBuff = curMeshDescriptor.m_MeshInstanceBuffer;

		// For the matrices:
		curMeshDescriptor.m_MatricesView = TStridedMemoryView<CFloat4x4>(static_cast<CFloat4x4*>(curBuff), curMeshDescriptorParticleCount);
		curBuff = Mem::AdvanceRawPointer(curBuff, curMeshDescriptorParticleCount * sizeof(CFloat4x4));
		curMeshDescriptor.m_ColorsView = TStridedMemoryView<CFloat4>(static_cast<CFloat4*>(curBuff), curMeshDescriptorParticleCount);

		if (curMeshDescriptor.m_RendererGUID >= 0)
		{
			if (needUpdateBuffer)
			{
				OnSetMeshInstancesBuffer(curMeshDescriptor.m_RendererGUID, curMeshDescriptor.m_MeshInstanceBuffer);
			}
			OnSetMeshInstancesCount(curMeshDescriptor.m_RendererGUID, curMeshDescriptorParticleCount);
		}
	}
	// -----------------------------------------------------------

	m_Exec_Matrices.Clear();
	m_Exec_Matrices.m_PerMeshMatrices = TStridedMemoryView<TStridedMemoryView<CFloat4x4> >(&meshDescriptors[0].m_MatricesView, meshDescriptorCount, sizeof(CParticleMaterialDescMesh::SMeshDescriptorData));
	m_BB.AddExec(&m_Exec_Matrices);

	TStridedMemoryView<TStridedMemoryView<CFloat4> >	colors = TStridedMemoryView<TStridedMemoryView<CFloat4> >(&meshDescriptors[0].m_ColorsView, meshDescriptorCount, sizeof(CParticleMaterialDescMesh::SMeshDescriptorData));

	m_Exec_Colors.Clear();
	PK_ONLY_IF_ASSERTS(bool setup = )m_Exec_Colors.SetupedAndAddExec<CFloat4>(m_BB, CParticleMaterialDescMesh::m_DiffuseColorStringId, colors, CFloat4::ONE);

	PK_ASSERT(setup);

	m_BB.LaunchTasks(m_DrawersScene);

	m_BB.WaitTasks();
	m_BB.Clear();
}

//----------------------------------------------------------------------------

void	CFrameCollector::GameThread_ClearAllPreloadedDrawCalls()
{
	PK_SCOPEDLOCK(m_FullRenderLock);

	m_BillboardDrawCalls.Clear();
	m_RibbonDrawCalls.Clear();

	m_CreateUnityRenderer.Clear();
	m_SetRendererActive.Clear();
	m_ResizeRenderer.Clear();
	m_UpdateRendererBounds.Clear();
}

//----------------------------------------------------------------------------

void	CFrameCollector::GameThread_ExecUnityCallbacks()
{
	PK_SCOPEDPROFILE();
	for (u32 i = 0; i < m_CreateUnityRenderer.Count(); ++i)
	{
		m_CreateUnityRenderer[i].ExecuteUnityCallback();
	}
	m_CreateUnityRenderer.Clear();

	for (u32 i = 0; i < m_SetRendererActive.Count(); ++i)
	{
		m_SetRendererActive[i].ExecuteUnityCallback();
	}
	m_SetRendererActive.Clear();

	if (!m_ResizeRenderer.Empty())
	{
		// Just to make sure we are not requesting a resize while the render thread is running:
		GameThread_WaitForRenderThreadIFN();
	}
	for (u32 i = 0; i < m_ResizeRenderer.Count(); ++i)
	{
		m_ResizeRenderer[i].ExecuteUnityCallback();
	}
	m_ResizeRenderer.Clear();

	for (u32 i = 0; i < m_UpdateRendererBounds.Count(); ++i)
	{
		m_UpdateRendererBounds[i].ExecuteUnityCallback();
	}
	m_UpdateRendererBounds.Clear();
}

//----------------------------------------------------------------------------

void	CFrameCollector::RenderThread_BuildDrawCalls(CRendererSubView &views)
{
	PK_SCOPEDPROFILE();
	PK_SCOPEDLOCK(m_FullRenderLock);

	if (m_Rendering == null) // first frame maybe
	{
		return;
	}
	m_Rendering->m_Rendered = true;

	SParticleCollectedFrameToRender		&frame = *m_Rendering;

	FindCompatibleDrawCallAndLaunchBillboarding(frame.m_BillboardDrawRequests.View(), frame.m_BillboardMaterialDescs.View(), m_BillboardDrawCalls.ViewForWriting(), views);
	FindCompatibleDrawCallAndLaunchBillboarding(frame.m_RibbonDrawRequests.View(), frame.m_RibbonMaterialDescs.View(), m_RibbonDrawCalls.ViewForWriting(), views);

	// Clean indices:
	for (u32 dci = 0; dci < m_BillboardDrawCalls.Count(); ++dci)
	{
		m_BillboardDrawCalls[dci].RenderThread_CleanUnusedIndices();
	}
	for (u32 dci = 0; dci < m_RibbonDrawCalls.Count(); ++dci)
	{
		m_RibbonDrawCalls[dci].RenderThread_CleanUnusedIndices();
	}

	// Finish billboarding:
	for (u32 dci = 0; dci < m_BillboardDrawCalls.Count(); ++dci)
	{
		m_BillboardDrawCalls[dci].FinishBillboarding(views);
	}
	for (u32 dci = 0; dci < m_RibbonDrawCalls.Count(); ++dci)
	{
		m_RibbonDrawCalls[dci].FinishBillboarding(views);
	}

	// Once the billboarding is over, we can clear the draw requests to avoid the expensive copy-on-write:
	frame.Clear();
}

//----------------------------------------------------------------------------

Drawers::CScene		*CFrameCollector::GetDrawersScene()
{
	return &m_DrawersScene;
}

//----------------------------------------------------------------------------

void				CFrameCollector::SetRenderApiData(IRenderAPIData *renderApiData)
{
	m_RenderApiData = renderApiData;
}

//----------------------------------------------------------------------------

IRenderAPIData		*CFrameCollector::GetRenderApiData()
{
	return m_RenderApiData;
}

//----------------------------------------------------------------------------

void				CFrameCollector::SetSplitDoubleSidedMaterials(bool splitDoubleSided)
{
	m_SplitDoubleSidedMaterials = splitDoubleSided;
}

//----------------------------------------------------------------------------

bool				CFrameCollector::GetSplitDoubleSidedMaterials() const
{
	return m_SplitDoubleSidedMaterials;
}

//----------------------------------------------------------------------------

void				CFrameCollector::SetDelayUnityCallbacks(bool delayCallbacks)
{
	m_DelayUnityCallbacks = delayCallbacks;
}

//----------------------------------------------------------------------------

void				CFrameCollector::SetBoundsEnabled(bool enableBounds)
{
	m_AreBoundEnabled = enableBounds;
}

//----------------------------------------------------------------------------

void				CFrameCollector::AddPreloadedBillboardMaterial(const CParticleMaterialDescBillboard &matDesc)
{
	m_PreloadedBillboardMatDesc.PushBack(matDesc);
}

//----------------------------------------------------------------------------

void				CFrameCollector::AddPreloadedRibbonMaterial(const CParticleMaterialDescBillboard &matDesc)
{
	m_PreloadedRibbonMatDesc.PushBack(matDesc);
}

//----------------------------------------------------------------------------

void				CFrameCollector::AddPreloadedMeshMaterial(const CParticleMaterialDescMesh &matDesc)
{
	m_PreloadedMeshMatDesc.PushBack(matDesc);
}

//----------------------------------------------------------------------------

void	CFrameCollector::GameThread_WaitForRenderThreadIFN()
{
	PK_SCOPEDLOCK(m_FullRenderLock);
}

//----------------------------------------------------------------------------

void	CFrameCollector::CreatePopcornRenderer(	bool delay,
												const SPopcornRendererDesc &rendererDesc,
												Drawers::ERendererClass rendererType,
												int *rendererHandle,
												const SRetrieveRendererInfo &rendererInfo)
{
	PK_ASSERT(rendererType == Drawers::Renderer_Billboard || rendererType == Drawers::Renderer_Ribbon);

	if (delay)
	{
		SOnCreateUnityRenderer		callback;

		callback.m_RendererType = rendererType;
		callback.m_RendererGUID = rendererHandle;
		callback.m_UserData = rendererDesc.m_UserData;
		callback.m_EffectNames = rendererDesc.m_EffectNames;
		callback.m_MaterialFlags = rendererDesc.m_MaterialFlags;
		callback.m_UniformFlags = rendererDesc.m_UniformFlags;
		callback.m_DrawOrder = rendererDesc.m_DrawOrder;
		callback.m_DiffuseMap = rendererDesc.m_DiffuseMap;
		callback.m_NormalMap = rendererDesc.m_NormalMap;
		callback.m_AlphaRemap = rendererDesc.m_AlphaRemap;
		callback.m_InvSoftnessDistance = rendererDesc.m_InvSofnessDistance;
		callback.m_RendererInfo = rendererInfo;

		m_CreateUnityRenderer.PushBack(callback);
	}
	else
	{
		if (rendererType == Drawers::Renderer_Billboard)
		{
			if (PK_VERIFY(_OnSetupNewBillboardRenderer != null && _OnRetrieveRendererBufferInfo != null))
			{
				*rendererHandle = _OnSetupNewBillboardRenderer(&rendererDesc);
				_OnRetrieveRendererBufferInfo(*rendererHandle, &rendererInfo);
			}
		}
		else if (rendererType == Drawers::Renderer_Ribbon)
		{
			if (PK_VERIFY(_OnSetupNewRibbonRenderer != null && _OnRetrieveRendererBufferInfo != null))
			{
				*rendererHandle = _OnSetupNewRibbonRenderer(&rendererDesc);
				_OnRetrieveRendererBufferInfo(*rendererHandle, &rendererInfo);
			}
		}
	}
}

//----------------------------------------------------------------------------

void	CFrameCollector::SetRendererActive(bool delay, int rendererHandle, bool setActive)
{
	if (delay)
	{
		SOnSetRendererActive	callback;

		callback.m_RendererGUID = rendererHandle;
		callback.m_Active = setActive ? ManagedBool_True : ManagedBool_False;

		m_SetRendererActive.PushBack(callback);
	}
	else
	{
		if (PK_VERIFY(_OnSetRendererActive != null))
		{
			_OnSetRendererActive(rendererHandle, setActive ? ManagedBool_True : ManagedBool_False);
		}
	}
}

//----------------------------------------------------------------------------

void	CFrameCollector::ResizeRenderer(bool delay, int rendererHandle, int vertexCount, int indexCount, const SRetrieveRendererInfo &rendererInfo)
{
	if (delay)
	{
		SOnResizeRenderer	callback;

		callback.m_RendererGUID = rendererHandle;
		callback.m_VertexCount = vertexCount;
		callback.m_IndexCount = indexCount;
		callback.m_RendererInfo = rendererInfo;

		m_ResizeRenderer.PushBack(callback);
	}
	else
	{
		if (PK_VERIFY(_OnResizeRenderer != null && _OnRetrieveRendererBufferInfo != null))
		{
			if (_OnResizeRenderer(rendererHandle, vertexCount, indexCount, ManagedBool_True))
			{
				_OnRetrieveRendererBufferInfo(rendererHandle, &rendererInfo);
			}
		}
	}
}

//----------------------------------------------------------------------------

void	CFrameCollector::UpdateRendererBounds(bool delay, int rendererHandle, const SUpdateRendererBounds &bounds)
{
	if (delay)
	{
		SOnUpdateRendererBounds		callback;

		callback.m_RendererGUID = rendererHandle;
		callback.m_Bounds = bounds;

		m_UpdateRendererBounds.PushBack(callback);
	}
	else
	{
		if (PK_VERIFY(_OnUpdateRendererBounds != null))
		{
			_OnUpdateRendererBounds(rendererHandle, &bounds);
		}
	}
}

//----------------------------------------------------------------------------

void	CFrameCollector::BeginCollectingFrame()
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(m_Collecting == null);
	if (m_NextToRender == null)
	{
		m_NextToRender = PK_NEW(SParticleCollectedFrameToRender);
		if (!PK_VERIFY(m_NextToRender != null))
		{
			return;
		}
	}
	else
		m_NextToRender->Clear();
	m_Collecting = m_NextToRender;

	m_Collecting->m_CollectedFrameId = ++m_CollectedFrameCount;

	// Install early particle page culling callback:
	// (called by DrawRequests.LockAndLoad)
	m_Collecting->m_BillboardDrawRequests.CbCullPage_MainMemory() = PopcornFX::CbStreamToRenderCullPage_MainMemory(this, &CFrameCollector::GameThread_CullPage_MainMemory);
	m_Collecting->m_RibbonDrawRequests.CbCullPage_MainMemory() = PopcornFX::CbStreamToRenderCullPage_MainMemory(this, &CFrameCollector::GameThread_CullPage_MainMemory);
}

//----------------------------------------------------------------------------

void	CFrameCollector::CollectFrame(CParticleMediumCollection *particleMediumCollection)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(m_Collecting != null);

	m_RenderersWalker.SetWalkEmptyOnes(true);
	m_RenderersWalker.Walk(*particleMediumCollection, *this); // This will collect the frame and set the particle count for each draw call

	// Clear the mesh dr just after the walk (the buffers are already filled)
	m_MeshDrawRequests.Clear();
}

//----------------------------------------------------------------------------

void	CFrameCollector::EndCollectingFrame()
{
	PK_SCOPEDPROFILE();
	m_Collecting = null;

	// Here we actually resize all the VBOs and IBOs
	for (u32 dci = 0; dci < m_BillboardDrawCalls.Count(); ++dci)
	{
		m_BillboardDrawCalls[dci].GameThread_ResizeUnityRenderers(m_DelayUnityCallbacks, m_AreBoundEnabled);
	}
	for (u32 dci = 0; dci < m_RibbonDrawCalls.Count(); ++dci)
	{
		m_RibbonDrawCalls[dci].GameThread_ResizeUnityRenderers(m_DelayUnityCallbacks, m_AreBoundEnabled);
	}

	// only lock for the swap
	PK_SCOPEDLOCK(m_FullRenderLock);
	PopcornFX::PKSwap(m_NextToRender, m_Rendering);
}

//----------------------------------------------------------------------------

bool	CFrameCollector::GameThread_CullPage_MainMemory(const CParticleStream_MainMemory &stream, const CParticlePage &page)
{
	bool		cullPage = false;

	// We should properly handle the culling here
	// ... page.Bounds() ...
	return cullPage;
}

//----------------------------------------------------------------------------

void    CFrameCollector::OnSetMeshInstancesCount(int rendererGUID, int instanceCount)
{
	if (PK_VERIFY(_OnSetMeshInstancesCount != null))
		_OnSetMeshInstancesCount(rendererGUID, instanceCount);
}

//----------------------------------------------------------------------------

void	CFrameCollector::OnSetMeshInstancesBuffer(int rendererGUID, void *instancesBuffer)
{
	if (PK_VERIFY(_OnSetMeshInstancesBuffer != null))
		_OnSetMeshInstancesBuffer(rendererGUID, instancesBuffer);
}

//----------------------------------------------------------------------------

bool	CFrameCollector::ClearRendererMeshIFN(CParticleDrawer &particleDrawer, const CParticleMedium &medium)
{
	CParticleDrawer_Manager		*drawerManager = static_cast<CParticleDrawer_Manager*>(&particleDrawer);
	CParticleMaterialDescMesh	&materialDescMesh = drawerManager->m_MaterialDescMesh;
	const u32					meshDescriptorCount = materialDescMesh.m_MeshDescriptors.Count();

	if (medium.EmptyForRendering())
	{
		if (!drawerManager->m_WasEmpty)
		{
			for (u32 i = 0; i < meshDescriptorCount; ++i)
			{
				int rendererGUID = materialDescMesh.m_MeshDescriptors[i].m_RendererGUID;
				OnSetMeshInstancesCount(rendererGUID, 0);
			}
		}
		drawerManager->m_WasEmpty = true;
		return true;
	}
	else
	{
		drawerManager->m_WasEmpty = false;
	}
	return false;
}

//----------------------------------------------------------------------------

void	CFrameCollector::RenderersWalk(const PopcornFX::CParticleRenderer_Billboard &renderer, const PopcornFX::CParticleMedium &medium, CParticleDrawer &particleDrawer)
{
	SParticleCollectedFrameToRender		&frame = *m_Collecting;
	CParticleDrawer_Manager				*drawerManager = static_cast<CParticleDrawer_Manager*>(&particleDrawer);

	if (!PK_VERIFY(frame.m_BillboardMaterialDescs.PushBack().Valid()))
		return;

	// LockAndLoad does:
	//	 builds the PopcornFX::Drawers::SBillboard_DrawRequest
	//	 calls your CbCullPage_MainMemory
	//	 takes references on Particle Stream and its Particle Pages (particle buffer) for rendering (CopyOnWrite references)
	if (!frame.m_BillboardDrawRequests.LockAndLoad(medium, renderer))
	{
		frame.m_BillboardMaterialDescs.PopBack();
		return;
	}

	const Drawers::SBillboard_DrawRequest	&dr = frame.m_BillboardDrawRequests.Last();
	CParticleMaterialDescBillboard			&materialDesc = frame.m_BillboardMaterialDescs.Last();

	materialDesc = drawerManager->m_MaterialDescBillboard;

	CParticleBaseDrawCall	*drawCall = FindCompatibleDrawCall(materialDesc, m_BillboardDrawCalls.ViewForWriting());
	if (drawCall != null) // Sometimes the draw calls are clear but the update still collect the draw calls
	{
		drawCall->GameThread_UpdateVertexAndIndexCount(dr);
	}
	++frame.m_TotalDrawRequestsCount;
}

//----------------------------------------------------------------------------

void	CFrameCollector::RenderersWalk(const CParticleRenderer_Ribbon &renderer, const CParticleMedium &medium, CParticleDrawer &particleDrawer)
{
	SParticleCollectedFrameToRender		&frame = *m_Collecting;
	CParticleDrawer_Manager				*drawerManager = static_cast<CParticleDrawer_Manager*>(&particleDrawer);

	if (!PK_VERIFY(frame.m_RibbonMaterialDescs.PushBack().Valid()))
		return;

	if (!frame.m_RibbonDrawRequests.LockAndLoad(medium, renderer))
	{
		frame.m_RibbonMaterialDescs.PopBack();
		return;
	}

	const Drawers::SRibbon_DrawRequest	&dr = frame.m_RibbonDrawRequests.Last();
	CParticleMaterialDescBillboard		&materialDesc = frame.m_RibbonMaterialDescs.Last();

	materialDesc = drawerManager->m_MaterialDescBillboard;

	CParticleBaseDrawCall	*drawCall = FindCompatibleDrawCall(materialDesc, m_RibbonDrawCalls.ViewForWriting());
	if (drawCall != null) // Sometimes the draw calls are clear but the update still collect the draw calls
	{
		drawCall->GameThread_UpdateVertexAndIndexCount(dr);
	}
	++frame.m_TotalDrawRequestsCount;
}

//----------------------------------------------------------------------------

void	CFrameCollector::RenderersWalk(const CParticleRenderer_Mesh &renderer, const CParticleMedium &medium, CParticleDrawer &particleDrawer)
{
	PK_ASSERT(CCurrentThread::IsMainThread());

	if (ClearRendererMeshIFN(particleDrawer, medium))
		return;

	SParticleCollectedFrameToRender		&frame = *m_Collecting;
	CParticleDrawer_Manager				*drawerManager = static_cast<CParticleDrawer_Manager*>(&particleDrawer);

	CParticleMaterialDescMesh	&materialDescMesh = drawerManager->m_MaterialDescMesh;
	PK_ASSERT(renderer.Meshes().Count() == materialDescMesh.m_MeshDescriptors.Count());

	if (!m_MeshDrawRequests.LockAndLoad(medium, renderer))
	{
		return;
	}

	const Drawers::SMesh_DrawRequest	&dr = m_MeshDrawRequests.Last();

	GameThread_CreateMeshTransforms(dr, materialDescMesh.m_MeshDescriptors);

	++frame.m_TotalDrawRequestsCount;
}

//----------------------------------------------------------------------------

void		SOnCreateUnityRenderer::ExecuteUnityCallback()
{
	PK_NAMEDSCOPEDPROFILE("Delayed OnSetupNewBillboardRenderer");

	SPopcornRendererDesc	desc;

	desc.m_UserData = m_UserData.Data();
	desc.m_EffectNames = m_EffectNames.Data();
	desc.m_MaterialFlags = m_MaterialFlags;
	desc.m_UniformFlags = m_UniformFlags;
	desc.m_DrawOrder = m_DrawOrder;
	desc.m_DiffuseMap = m_DiffuseMap.Data();
	desc.m_NormalMap = m_NormalMap.Data();
	desc.m_AlphaRemap = m_AlphaRemap.Data();
	desc.m_InvSofnessDistance = m_InvSoftnessDistance;

	if (m_RendererType == Drawers::Renderer_Billboard &&
		PK_VERIFY(_OnSetupNewBillboardRenderer != null))
	{
		*m_RendererGUID = _OnSetupNewBillboardRenderer(&desc);
	}
	else if (m_RendererType == Drawers::Renderer_Billboard &&
		PK_VERIFY(_OnSetupNewRibbonRenderer != null))
	{
		*m_RendererGUID = _OnSetupNewRibbonRenderer(&desc);
	}
	else
	{
		PK_ASSERT_NOT_REACHED();
	}

	if (PK_VERIFY(_OnRetrieveRendererBufferInfo != null))
	{
		_OnRetrieveRendererBufferInfo(*m_RendererGUID, &m_RendererInfo);
	}
}

//----------------------------------------------------------------------------

void		SOnSetRendererActive::ExecuteUnityCallback()
{
	PK_NAMEDSCOPEDPROFILE("Delayed OnSetRendererActive");
	if (PK_VERIFY(_OnSetRendererActive != null))
	{
		_OnSetRendererActive(m_RendererGUID, m_Active);
	}
}

//----------------------------------------------------------------------------

void		SOnResizeRenderer::ExecuteUnityCallback()
{
	PK_NAMEDSCOPEDPROFILE("Delayed OnResizeRenderer");
	bool	needUpdateSlice = false;

	if (PK_VERIFY(_OnResizeRenderer != null))
	{
		// We update the VBO and IBO size:
		needUpdateSlice = _OnResizeRenderer(m_RendererGUID, m_VertexCount, m_IndexCount, ManagedBool_True) == ManagedBool_True;
	}
	if (needUpdateSlice)
	{
		if (PK_VERIFY(_OnRetrieveRendererBufferInfo != null))
		{
			_OnRetrieveRendererBufferInfo(m_RendererGUID, &m_RendererInfo);
		}
	}
}

//----------------------------------------------------------------------------

void		SOnUpdateRendererBounds::ExecuteUnityCallback()
{
	PK_NAMEDSCOPEDPROFILE("Delayed OnUpdateRendererBounds");
	if (PK_VERIFY(_OnUpdateRendererBounds != null))
	{
		_OnUpdateRendererBounds(m_RendererGUID, &m_Bounds);
	}
}
