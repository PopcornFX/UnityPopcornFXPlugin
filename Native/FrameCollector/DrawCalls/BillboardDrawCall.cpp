//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "BillboardDrawCall.h"

#include "FrameCollector/FrameCollector.h"
#include "FxRendererSubView.h"

#include "ImplemGraphicsAPI/RenderAPI_Data.h"

//----------------------------------------------------------------------------

CParticleBillboardDrawCall::CParticleBillboardDrawCall()
	: m_SortWorkingBufferSize(0)
	, m_SortWorkingBuffer(null)
{
	m_RendererType = Drawers::Renderer_Billboard;
}

//----------------------------------------------------------------------------

CParticleBillboardDrawCall::~CParticleBillboardDrawCall()
{
	PK_FREE(m_SortWorkingBuffer);
}

//----------------------------------------------------------------------------

void			CParticleBillboardDrawCall::Setup(CFrameCollector *renderManager, const CParticleMaterialDescBillboard &materialDesc)
{
	m_FrameCollector = renderManager;
	m_MaterialDesc = materialDesc;
	m_VertexStride = FillOffsetTableAndGetVertexBufferStride(m_SemanticOffsets, m_MaterialDesc.m_Flags.m_MaterialFlags);
}

//----------------------------------------------------------------------------

bool			CParticleBillboardDrawCall::CanRender(const CParticleMaterialDescBillboard &materialDesc) const
{
	return m_MaterialDesc == materialDesc;
}

//----------------------------------------------------------------------------

void			CParticleBillboardDrawCall::AddToRender(const Drawers::SBillboard_DrawRequest &inDrawRequest, const CParticleMaterialDescBillboard &materialDesc)
{
	const CGuid							dri = m_DrawRequests.PushBack(inDrawRequest);
	Drawers::SBillboard_DrawRequest		&drawRequest = m_DrawRequests[dri];
	// modify drawRequest at will, it's yours now
	(void)drawRequest;

	PK_ASSERT(CanRender(materialDesc));
}

//----------------------------------------------------------------------------

void			CParticleBillboardDrawCall::LaunchBillboarding(CRendererSubView &view)
{
	PK_SCOPEDPROFILE();

	if (m_UnityCallbacksDelayed)
		m_DrawRequests.Clear();

	if (m_DrawRequests.Empty())
		return;

	if (!m_BB.Prepare(m_DrawRequests.View()))
		return;

	const u32					vertexCount = m_BB.TotalVertexCount();
	const u32					indexCount = m_BB.TotalIndexCount();
	CParticleMaterialDescFlags	&materialDescFlags = m_MaterialDesc.m_Flags;

	if (!PK_VERIFY(RenderThread_AllocBuffers(vertexCount, indexCount)))
		return;

	m_Exec_PNT.Clear();
	m_Exec_PNT.m_BillboardingMatrix = view.BillBoardingMatrix();
	m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_Positions, vertexCount, 0x10);
	if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_Lighting))
	{
		PK_ASSERT(m_ParticleBuffers.m_Normals != null);
		m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_Normals, vertexCount, 0x10);
	}
	m_BB.AddExecPage(&m_Exec_PNT);

	if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_Color))
	{
		PK_ASSERT(m_ParticleBuffers.m_Colors != null);
		m_Exec_Colors.Clear();
		m_Exec_Colors.m_Colors = TStridedMemoryView<CFloat4>(m_ParticleBuffers.m_Colors, vertexCount, 0x10);
		m_BB.AddExecPage(&m_Exec_Colors);
	}

	m_Exec_Texcoords.Clear();
	m_Exec_Texcoords.m_Texcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords0, vertexCount);
	if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_AnimBlend))
	{
		PK_ASSERT(m_ParticleBuffers.m_TexCoords1 != null && m_ParticleBuffers.m_AtlasId != null);
		m_Exec_Texcoords.m_Texcoord2s = TStridedMemoryView<CFloat2>((CFloat2*)m_ParticleBuffers.m_TexCoords1, vertexCount);
		m_Exec_Texcoords.m_AtlasIds = TMemoryView<float>(m_ParticleBuffers.m_AtlasId, vertexCount);
	}
	m_BB.AddExecPage(&m_Exec_Texcoords);

	if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_AlphaRemap))
	{
		PK_ASSERT(m_ParticleBuffers.m_AlphaCursor != null);
		m_Exec_AlphaCursors.Clear();
		m_Exec_AlphaCursors.m_AlphaCursors = TMemoryView<float>(m_ParticleBuffers.m_AlphaCursor, vertexCount);
		m_BB.AddExecPage(&m_Exec_AlphaCursors);
	}

	const bool		doSort = materialDescFlags.NeedSort();

	m_Exec_Indices.Clear();
	if (!m_Exec_Indices.m_IndexStream.Setup(m_ParticleBuffers.m_Indices, indexCount, m_LargeIndices == ManagedBool_True))
		return;
	m_Exec_Indices.m_OverrideSort = (doSort ? 1 : 0);
	m_Exec_Indices.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_Positions, vertexCount, 0x10);
	m_Exec_Indices.m_SortDirection = view.BillBoardingMatrix().StrippedZAxis();

	u32	workingBufferSize = m_Exec_Indices.WorkingBufferSize(m_BB); // will read m_OverrideSort

	if (workingBufferSize > m_SortWorkingBufferSize)
	{
		workingBufferSize = Mem::Align<0x1000>(workingBufferSize);
		m_SortWorkingBufferSize = workingBufferSize;
		if (m_SortWorkingBuffer != null)
			PK_FREE(m_SortWorkingBuffer);
		m_SortWorkingBuffer = PK_MALLOC_ALIGNED(workingBufferSize, 0x10);
	}
	if (workingBufferSize > 0 && m_SortWorkingBuffer == null)
		return;

	if (!m_Exec_Indices.SetupedAndAddExec(m_BB, m_SortWorkingBuffer, workingBufferSize))
		return;

	PK_ASSERT(m_FrameCollector != null && m_FrameCollector->GetDrawersScene() != null);

	// Copy the vertices from Structure-Of-Arrays to Array-Of-Structures
	VertexBufferSAO2AOSPrepareCopy();
	m_BB.AddExecLatePage(&m_Exec_SAO2AOS);

	m_BB.LaunchTasks(*m_FrameCollector->GetDrawersScene());
}

//----------------------------------------------------------------------------

void			CParticleBillboardDrawCall::FinishBillboarding(CRendererSubView &views)
{
	if (m_DrawRequests.Empty())
		return;

	PK_SCOPEDPROFILE();

	m_BB.WaitTasks();
	// As soon as we finish the billboarding, we can unlock the particle stream to avoid a copy on write on the next update:
	m_DrawRequests.Clear();

	if (!m_ParticleBuffers.BillboardInMappedIdxBuffer() &&
		!m_FrameCollector->GetRenderApiData()->NeedTempBuffer())
	{
		RenderThread_FillIndexBuffer();
	}

	if (m_FrameCollector->GetRenderApiData()->NeedTempBuffer())
	{
		m_FrameCollector->GetRenderApiData()->CopyTempToNativeBuffer(m_Slice.m_VBHandler, false, m_ParticleBuffers.m_TempVB, m_ParticleBuffers.m_TempVBSize);
		m_FrameCollector->GetRenderApiData()->CopyTempToNativeBuffer(m_Slice.m_IBHandler, true, m_ParticleBuffers.m_Indices, m_ParticleBuffers.m_IndexBufferSize);
	}

	m_FrameCollector->GetRenderApiData()->EndModifyNativeBuffer(m_Slice.m_VBHandler, false);
	m_FrameCollector->GetRenderApiData()->EndModifyNativeBuffer(m_Slice.m_IBHandler, true);

	m_DrawCallReady = true;
}

//----------------------------------------------------------------------------
