//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "ParticleMaterialData.h"

#include "UnityGraphicsAPI/IUnityGraphics.h"
#include "NativeToManaged.h"

#include <pk_kernel/include/kr_containers_array.h>
#include <pk_kernel/include/kr_memoryviews.h>
#include <pk_kernel/include/kr_threads.h>
#include <pk_drawers/include/dr_renderers_walker.h>
#include <pk_drawers/include/dr_mesh.h>
#include <pk_drawers/include/dr_mesh_cpu.h>
#include <pk_drawers/include/dr_scene.h>

namespace	PopcornFX
{
	class	CParticleMediumCollection;
	class	CRendererSubView;
	class	CRendererSubView;
	class	CParticleStream_MainMemory;
	class	CParticlePage;
	class	CParticleRenderer_Billboard;
	class	CParticleRenderer_Ribbon;
	class	CParticleMedium;
	class	CParticleDrawer;
}

class	CParticleBaseDrawCall;
class	CParticleBillboardDrawCall;
class	CParticleRibbonDrawCall;
struct	SParticleCollectedFrameToRender;
class	IRenderAPIData;

using namespace PopcornFX;

// Unity renderer callback delayed:

struct	SOnCreateUnityRenderer
{
	// In:
	Drawers::ERendererClass	m_RendererType;
	CString					m_DiffuseMap;
	CString					m_NormalMap;
	CString					m_AlphaRemap;
	CString					m_UserData;
	u32						m_MaterialFlags;
	u32						m_UniformFlags;
	s32						m_DrawOrder;
	float					m_InvSoftnessDistance;
	CString					m_EffectNames;

	// Out:
	int						*m_RendererGUID;
	SRetrieveRendererInfo	m_RendererInfo;

	void				ExecuteUnityCallback();
};

struct	SOnSetRendererActive
{
	// In:
	int					m_RendererGUID;
	ManagedBool			m_Active;

	void				ExecuteUnityCallback();
};

struct	SOnResizeRenderer
{
	// In:
	int						m_RendererGUID;
	int						m_VertexCount;
	int						m_IndexCount;

	// Out:
	SRetrieveRendererInfo	m_RendererInfo;

	void					ExecuteUnityCallback();
};

struct	SOnUpdateRendererBounds
{
	// In:
	int						m_RendererGUID;
	SUpdateRendererBounds	m_Bounds;

	void				ExecuteUnityCallback();
};

/*
Manages the collect of particle data on the GameThread
And the render of those particle data on the RenderThread
*/
class CFrameCollector : public Drawers::IRenderersWalkerDelegates
{
public:
	CFrameCollector();
	~CFrameCollector();

	bool				InitializeAPI(UnityGfxRenderer deviceType);

	void				InstallToMediumCollection(CParticleMediumCollection &collection, bool enableMeshes);

	void				BeginCollectingFrame();
	void				CollectFrame(CParticleMediumCollection *collection);
	void				EndCollectingFrame();

	void				GameThread_CreatePreloadedDrawCalls();
	void				GameThread_CreateMeshTransforms(const Drawers::SMesh_DrawRequest &drawRequest,
														const TMemoryView<CParticleMaterialDescMesh::SMeshDescriptorData> &meshDescriptors);

	void				GameThread_ClearAllPreloadedDrawCalls();

	void				GameThread_ExecUnityCallbacks();

	void				RenderThread_BuildDrawCalls(CRendererSubView &views);

	Drawers::CScene		*GetDrawersScene();
	void				SetRenderApiData(IRenderAPIData *renderApiData);
	IRenderAPIData		*GetRenderApiData();

	void				SetSplitDoubleSidedMaterials(bool splitDoubleSided);
	bool				GetSplitDoubleSidedMaterials() const;

	void				SetDelayUnityCallbacks(bool delayCallbacks);
	void				SetBoundsEnabled(bool enableBounds);

	void				AddPreloadedBillboardMaterial(const CParticleMaterialDescBillboard &matDesc);
	void				AddPreloadedRibbonMaterial(const CParticleMaterialDescBillboard &matDesc);
	void				AddPreloadedMeshMaterial(const CParticleMaterialDescMesh &matDesc);

	void				GameThread_WaitForRenderThreadIFN();

	// Delayed Unity callbacks:
	void				CreatePopcornRenderer(	bool delay,
												const SPopcornRendererDesc &rendererDesc,
												Drawers::ERendererClass	rendererType,
												int *rendererHandle,
												const SRetrieveRendererInfo &rendererInfo);
	void				SetRendererActive(bool delay, int rendererHandle, bool setActive);
	void				ResizeRenderer(bool delay, int rendererHandle, int vertexCount, int indexCount, const SRetrieveRendererInfo &rendererInfo);
	void				UpdateRendererBounds(bool delay, int rendererHandle, const SUpdateRendererBounds &bounds);

private:
	bool			GameThread_CullPage_MainMemory(const CParticleStream_MainMemory &stream, const CParticlePage &page);
	virtual void	RenderersWalk(const CParticleRenderer_Billboard &renderer, const CParticleMedium &medium, CParticleDrawer &particleDrawer) override;
	virtual void	RenderersWalk(const CParticleRenderer_Ribbon &renderer, const CParticleMedium &medium, CParticleDrawer &particleDrawer) override;
	virtual void	RenderersWalk(const CParticleRenderer_Mesh &renderer, const CParticleMedium &medium, CParticleDrawer &particleDrawer) override;

	static void		OnSetMeshInstancesCount(int rendererGUID, int instanceCount);
	static void		OnSetMeshInstancesBuffer(int rendererGUID, void *instanceBuff);

	static bool		ClearRendererMeshIFN(CParticleDrawer &particleDrawer, const CParticleMedium &medium);

	template<typename _RendererType>
	CParticleDrawer_Std		*Drawer_BuildRemapperWrapper(CParticleRenderMedium *rMedium, CParticleRenderManager_Std *manager)
	{
		CParticleDrawer_Manager		*cacheMaterial = PK_NEW(CParticleDrawer_Manager(this, rMedium));
		const CParticleRenderer		*hboRd = rMedium->MediumsAndRenderers()[0].m_Renderer.Get();
		const CParticleMedium		*hboMed = rMedium->MediumsAndRenderers()[0].m_MediumToRender.Get();

		if (cacheMaterial == null || hboRd == null || hboMed == null)
			return null;
		const _RendererType	*renderer = HBO::Cast<const _RendererType>(hboRd);

		if (!PK_VERIFY(renderer != null))
		{
			PK_DELETE(cacheMaterial);
			return null;
		}
		cacheMaterial->GameThread_SetupRenderer<_RendererType>(renderer, hboMed);
		return cacheMaterial;
	}

	template<class _MaterialType, class _DrawCallType>
	_DrawCallType		*FindCompatibleDrawCall(const _MaterialType &collectedMat,
												const TMemoryView<_DrawCallType> &drawCalls)
	{
		_DrawCallType			*drawCall = null;

		// Find an already existing draw call that could render us
		for (u32 dci = 0; dci < drawCalls.Count(); ++dci)
		{
			if (drawCalls[dci].CanRender(collectedMat))
			{
				drawCall = &drawCalls[dci];
				break;
			}
		}
		return drawCall;
	}

	template<class _MaterialType, class _DrawCallType>
	void			CreateCompatibleDrawCalls(	const TMemoryView<const _MaterialType> &collectedMat,
												TArray<_DrawCallType> &drawCalls)
	{
		PK_SCOPEDPROFILE();
		for (u32 mati = 0; mati < collectedMat.Count(); ++mati)
		{
			const _MaterialType							&matDesc = collectedMat[mati];
			_DrawCallType								*drawCall = null;

			// Find an already existing draw call that could render us
			drawCall = FindCompatibleDrawCall(matDesc, drawCalls.ViewForWriting());
			// Otherwise we create the draw call:
			if (drawCall == null)
			{
				// We do not want to realloc: check the allocated size
				const CGuid		i = drawCalls.PushBack();
				drawCall = &(drawCalls[i]);
				drawCall->Setup(this, matDesc);
				drawCall->GameThread_CreateUnityRenderers(false);
			}
			else
			{
				drawCall->GameThread_AddRendererNameToUnity(matDesc.m_EffectNamePath);
			}
		}
	}

	template<class _DrawRequestType, class _MaterialType, class _DrawCallType>
	void			FindCompatibleDrawCallAndLaunchBillboarding(const TMemoryView<const _DrawRequestType> &collectedDr,
																const TMemoryView<const _MaterialType> &collectedMat,
																const TMemoryView<_DrawCallType> &drawCalls,
																CRendererSubView &views)
	{
		PK_SCOPEDPROFILE();
		for (u32 i = 0; i < drawCalls.Count(); ++i)
		{
			drawCalls[i].Clear();
		}
		// We then try to fit the draw requests inside some draw-calls:
		for (u32 dri = 0; dri < collectedDr.Count(); ++dri)
		{
			const _DrawRequestType						&dr = collectedDr[dri];
			const _MaterialType							&matDesc = collectedMat[dri];
			_DrawCallType								*drawCall = null;

			// Find an already existing draw call that could render us
			drawCall = FindCompatibleDrawCall(matDesc, drawCalls);
			if (drawCall != null)
			{
				drawCall->AddToRender(dr, matDesc);
			}
		}
		// Then we launch the billboarding tasks:
		for (u32 dci = 0; dci < drawCalls.Count(); ++dci)
		{
			drawCalls[dci].LaunchBillboarding(views);
		}
	}

	bool										m_SplitDoubleSidedMaterials;

	IRenderAPIData								*m_RenderApiData;

	Drawers::CRenderersWalker					m_RenderersWalker;
	SParticleCollectedFrameToRender				*m_Collecting;

	Threads::CCriticalSection					m_FullRenderLock;
	Threads::CCriticalSection					m_NextToRenderLock;
	SParticleCollectedFrameToRender				*m_NextToRender;
	u32											m_CollectedFrameCount;

	SParticleCollectedFrameToRender				*m_Rendering;

	TArray<CParticleBillboardDrawCall>			m_BillboardDrawCalls;
	TArray<CParticleRibbonDrawCall>				m_RibbonDrawCalls;

	// Mesh update
	Drawers::CMesh_CPU							m_BB;
	Drawers::CMesh_Exec_Matrices				m_Exec_Matrices;
	Drawers::CMesh_Exec_MaterialParam			m_Exec_Colors;

	// The mesh draw requests are not collected in the frame as they are filled in the game thread:
	Drawers::CMesh_DrawRequests					m_MeshDrawRequests;

	Drawers::CScene								m_DrawersScene;

	// Preloaded material data still to add in the draw calls:
	TArray<CParticleMaterialDescBillboard>		m_PreloadedBillboardMatDesc;
	TArray<CParticleMaterialDescBillboard>		m_PreloadedRibbonMatDesc;
	TArray<CParticleMaterialDescMesh>			m_PreloadedMeshMatDesc;

	bool										m_DelayUnityCallbacks;
	bool										m_AreBoundEnabled;

	// Delayed Unity callbacks:
	TArray<SOnCreateUnityRenderer>				m_CreateUnityRenderer;
	TArray<SOnSetRendererActive>				m_SetRendererActive;
	TArray<SOnResizeRenderer>					m_ResizeRenderer;
	TArray<SOnUpdateRendererBounds>				m_UpdateRendererBounds;
};
