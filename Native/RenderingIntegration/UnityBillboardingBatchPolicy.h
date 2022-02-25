//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "FrameCollectorUnityTypes.h"
#include "UnityRendererCache.h"
#include "PKUnity_InterfaceCommon.h"
#include "ImplemGraphicsAPI/RenderAPI_Data.h"

struct	SParticleBuffers;
class	CFrameCollector;

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CUnityBillboardingBatchPolicy
{
public:
	CUnityBillboardingBatchPolicy();
	~CUnityBillboardingBatchPolicy();

	bool			Init(UnityGfxRenderer deviceType);

	// Only called when not empty:
	static bool		CanRender(const Drawers::SBillboard_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SRibbon_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SMesh_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SLight_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SSound_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::STriangle_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		IsStateless() { return false; }

	// Do not remove
	bool		Tick(SUnityRenderContext &, const TMemoryView<SSceneView> &) { return true; }

	void		ClearBatch();
	void		CustomStepFlagInactive();
	bool		AllocBuffers(SUnityRenderContext &ctx, const SBuffersToAlloc &allocBuffers, const TMemoryView<SSceneView> &views, ERendererClass rendererType);

	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SGPUBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SRibbonBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SGPURibbonBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SMeshBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, STriangleBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SSceneView> &views, SGPUTriangleBatchJobs *batchJobs, const SGeneratedInputs &toMap);

	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> &drawRequests, Drawers::CRibbon_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SMesh_DrawRequest * const> &drawRequests, Drawers::CMesh_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SMesh_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SLight_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SLight_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SSound_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SSound_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::STriangle_DrawRequest * const> &drawRequests, Drawers::CTriangle_CPU *batch);
	bool		LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::STriangle_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch);

	bool		WaitForCustomTasks(SUnityRenderContext &ctx);

	bool		UnmapBuffers(SUnityRenderContext &ctx);
	void		ClearBuffers(SUnityRenderContext &ctx);

	bool		AreBillboardingBatchable(const PCRendererCacheBase &firstCache, const PCRendererCacheBase &secondCache) const;

	bool		EmitDrawCall(SUnityRenderContext &ctx, const SDrawCallDesc &toEmit, SUnityDrawOutputs &output);

private:
	void		_UpdateThread_ResizeUnityMesh(const SBuffersToAlloc &allocBuffers, IRenderAPIData *renderApiData);
	void		_UpdateThread_SetUnityMeshBounds(const SBuffersToAlloc &allocBuffers);

	void		_UpdateThread_ResizeUnityMeshInstanceCount(const SBuffersToAlloc &allocBuffers, IRenderAPIData *renderApiData);

	bool		_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData);
	bool		_RenderThread_AllocBillboardingBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData);
	bool		_RenderThread_SetupBuffersBillboards(const SGeneratedInputs &toMap, SBillboardBatchJobs *billboardBatch);
	bool		_RenderThread_SetupBuffersGeomBillboards(const SGeneratedInputs &toMap, SGPUBillboardBatchJobs *billboardBatch);
	bool		_RenderThread_SetupBuffersRibbons(const SGeneratedInputs &toMap, SRibbonBatchJobs *ribbonBatch);
	bool		_RenderThread_SetupBuffersMeshes(const SGeneratedInputs &toMap, SMeshBatchJobs *meshBatch);
	bool		_RenderThread_SetupBuffersTriangles(const SGeneratedInputs &toMap, STriangleBatchJobs *triangleBatch);
	bool		_FindAdditionalInput(const CStringId &inputName, EBaseTypeID inputType, const SGeneratedInputs &genInputs);

	void		_PrepareCopySOA2AOS(const SUnityRenderContext &ctx);

	// ----------------------------------------------------------------------------
	// FILLED ON GAME THREAD - ALLOC UNITY BUFFERS:
	// ----------------------------------------------------------------------------

	// --------------------------------------
	// We keep a local copy of the reference material description for this mesh:
	// --------------------------------------
	ERendererClass					m_RendererType;
	CParticleMaterialDescBillboard	m_MaterialDescBillboard;
	CParticleMaterialDescMesh		m_MaterialDescMesh;
	SUnityMeshInfo					m_UnityMeshInfo;
	CAABB							m_BBox;
	bool							m_GPUBillboarding;

	PCRectangleList					m_AtlasList;

	// --------------------------------------
	// Current exact vertex and index count:
	// --------------------------------------
	u32								m_VertexCount;
	u32								m_IndexCount;
	u32								m_ParticleCount;
	u32								m_PrevParticleCount;

	// Can we fill the buffer?
	bool							m_MeshIsValid;

	// ----------------------------------------------------------------------------
	// FILLED ON RENDER THREAD - ALLOC BILLBOARDING BUFFERS:
	// ----------------------------------------------------------------------------

	template<class _Type>
	struct	SSizedBuffer
	{
		_Type	*m_Ptr;
		u32		m_Count;

		SSizedBuffer()
		:	m_Ptr(null)
		,	m_Count(0)
		{
		}

		~SSizedBuffer()
		{
			FreeIFN();
		}

		void	ResizeIFN(Drawers::EGeneratedInput curInputFlag, u32 toAllocFlags, u32 prevToAllocFlags, u32 newVertexCount)
		{
			(void)prevToAllocFlags;
			if ((toAllocFlags & curInputFlag) != 0) // If we need this input buffer
			{
				if (m_Count < newVertexCount || m_Ptr == null)
				{
					u32		paddedVertexCount = Mem::Align<0x1000>(newVertexCount);
					PK_FREE(m_Ptr);
					m_Ptr = (_Type*)PK_MALLOC_ALIGNED(paddedVertexCount * sizeof(_Type), 0x10);
					m_Count = paddedVertexCount;
				}
			}
		}

		void	ResizeIFN(u32 newVertexCount)
		{
			if (m_Count < newVertexCount || m_Ptr == null)
			{
				u32			paddedVertexCount = Mem::Align<0x1000>(newVertexCount);
				PK_FREE(m_Ptr);
				m_Ptr = (_Type*)PK_MALLOC_ALIGNED(paddedVertexCount * sizeof(_Type), 0x10);
				m_Count = paddedVertexCount;
			}
		}

		void	FreeIFN()
		{
			PK_FREE(m_Ptr);
			m_Ptr = null;
			m_Count = 0;
		}
	};

	// --------------------------------------
	// Billboarding buffers:
	// --------------------------------------
	// Allocated buffers:
	struct	SParticleBuffers
	{
		struct	SPerView
		{
			u32						m_GeneratedInputs;
			SSizedBuffer<CFloat4>	m_Positions;
			SSizedBuffer<CFloat4>	m_Normals;
			SSizedBuffer<CFloat2>	m_UVFactors;

			// We do not use Sized buffers for indices as they are void*
			void					*m_Indices;

			SPerView()
			:	m_GeneratedInputs(0)
			,	m_Indices(null)
			{
			}

			~SPerView()
			{
				m_Positions.FreeIFN();
				m_Normals.FreeIFN();
				m_UVFactors.FreeIFN();
				PK_FREE(m_Indices);
			}
		};

		// Buffers info:
		u32						m_IndicesCount;
		u32						m_GeneratedInputs;

		ManagedBool				m_LargeIndices;

		// View dependent buffers:
		TArray<SPerView>		m_PerViewGeom;

		// View independent buffers:
		SPerView				m_ViewIndependantGeom;
		SSizedBuffer<CFloat2>	m_TexCoords0;
		SSizedBuffer<CFloat2>	m_TexCoords1;
		SSizedBuffer<float>		m_AtlasId;
		SSizedBuffer<CFloat4>	m_UVRemap;

		// Additional fields that we handle:
		SSizedBuffer<CFloat4>	m_Colors;
		SSizedBuffer<float>		m_AlphaCursor;

		// View on the additional fields, all are null except for color and alpha cursor:
		TArray<Drawers::SCopyFieldDesc>		m_AdditionalFieldsBuffers;

		SParticleBuffers()
		:	m_IndicesCount(0)
		,	m_GeneratedInputs(0)
		,	m_LargeIndices(ManagedBool_False)
		{
		}

		void	FreeAll()
		{
			m_PerViewGeom.Clear();
			m_TexCoords0.FreeIFN();
			m_TexCoords1.FreeIFN();
			m_AtlasId.FreeIFN();
			m_UVRemap.FreeIFN();
			m_Colors.FreeIFN();
			m_AlphaCursor.FreeIFN();
		}
	};

	// Buffers in which we are going to billboard the particles
	SParticleBuffers			m_ParticleBuffers;

	struct		SMeshParticleBuffers
	{
		u32		m_InstanceCount;
		void	*m_RawPerInstanceBuffer;

		TStridedMemoryView<CFloat4x4>			m_Transforms;
		TStridedMemoryView<CFloat4>				m_Colors;

		SMeshParticleBuffers()
		:	m_InstanceCount(0)
		,	m_RawPerInstanceBuffer(null)
		{
		}

		~SMeshParticleBuffers()
		{
			PK_FREE(m_RawPerInstanceBuffer);
			m_InstanceCount = 0;
			m_RawPerInstanceBuffer = null;
		}
	};

	Drawers::SCopyFieldDescPerMesh		m_MeshColorAdditionalField;
	TArray<SMeshParticleBuffers>		m_PerMeshBuffers;

	// Mapped Unity buffers:
	void						*m_MappedVtxBuffer;
	void						*m_MappedIdxBuffer;
	//GPU ONLY
	void						*m_MappedInfoBillboardBuffer;
	void						*m_MappedAtlasesBuffer;

	// --------------------------------------
	// FINAL SOA TO AOS COPY:
	// --------------------------------------

	// Temp struct to gather the buffers of the first billboarded view (we only gave one mesh per batch for the moment):
	struct		SParticleSourceBuffers
	{
		void 					*m_Indices;
		CFloat4					*m_Positions;
		CFloat4					*m_Normals;
		CFloat2					*m_UVFactors;
		CFloat2					*m_TexCoords0;
		CFloat2					*m_TexCoords1;
		float					*m_AtlasId;
		CFloat4					*m_UVRemap;
		CFloat4					*m_Colors;
		float					*m_AlphaCursor;

		SParticleSourceBuffers()
		{
			Clear();
		}

		void	FromParticleBuffers(const SParticleBuffers &buffers)
		{
			if (buffers.m_GeneratedInputs & Drawers::GenInput_Indices)
				m_Indices = buffers.m_ViewIndependantGeom.m_Indices;
			else
				m_Indices = null;
			if (buffers.m_GeneratedInputs & Drawers::GenInput_Position)
				m_Positions = buffers.m_ViewIndependantGeom.m_Positions.m_Ptr;
			else
				m_Positions = null;
			if (buffers.m_GeneratedInputs & Drawers::GenInput_Normal)
				m_Normals = buffers.m_ViewIndependantGeom.m_Normals.m_Ptr;
			else
				m_Normals = null;
			if (buffers.m_GeneratedInputs & Drawers::GenInput_UVFactors)
				m_UVFactors = buffers.m_ViewIndependantGeom.m_UVFactors.m_Ptr;
			else
				m_UVFactors = null;

			m_TexCoords0 = buffers.m_TexCoords0.m_Ptr;
			m_TexCoords1 = buffers.m_TexCoords1.m_Ptr;
			m_AtlasId = buffers.m_AtlasId.m_Ptr;
			m_UVRemap = buffers.m_UVRemap.m_Ptr;
			m_Colors = buffers.m_Colors.m_Ptr;
			m_AlphaCursor = buffers.m_AlphaCursor.m_Ptr;

			// We get the buffers of the first view for the moment:
			if (!buffers.m_PerViewGeom.Empty())
			{
				if (buffers.m_PerViewGeom.First().m_GeneratedInputs & Drawers::GenInput_Indices)
					m_Indices = buffers.m_PerViewGeom.First().m_Indices;
				if (buffers.m_PerViewGeom.First().m_GeneratedInputs & Drawers::GenInput_Position)
					m_Positions = buffers.m_PerViewGeom.First().m_Positions.m_Ptr;
				if (buffers.m_PerViewGeom.First().m_GeneratedInputs & Drawers::GenInput_Normal)
					m_Normals = buffers.m_PerViewGeom.First().m_Normals.m_Ptr;
				if (buffers.m_PerViewGeom.First().m_GeneratedInputs & Drawers::GenInput_UVFactors)
					m_UVFactors = buffers.m_PerViewGeom.First().m_UVFactors.m_Ptr;
			}
		}

		void	Clear()
		{
			m_Indices = null;
			m_Positions = null;
			m_Normals = null;
			m_UVFactors = null;
			m_TexCoords0 = null;
			m_TexCoords1 = null;
			m_AtlasId = null;
			m_UVRemap = null;
			m_Colors = null;
			m_AlphaCursor = null;
		}
	};

	class	CBillboard_Exec_SOA_OAS
	{
	public:
		u32						m_ShaderVariationFlags;

		SParticleSourceBuffers	m_ParticleBuffers;		// Src
		void					*m_MappedVertexBuffer;	// Dst vtx

		// Dst buffers info:
		u32						(*m_SemanticOffsets)[__Semantic_Count];
		u32						m_VertexStride;

		CBillboard_Exec_SOA_OAS() : m_ShaderVariationFlags(0), m_MappedVertexBuffer(null), m_VertexStride(0) { Mem::Clear(m_SemanticOffsets); }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const Drawers::SBillboard_ExecPage &batch);
		void	operator()(const Drawers::SRibbon_ExecBatch &batch);
		void	operator()(const Drawers::STriangle_ExecPage &batch);

		void	_CopyData(u32 vertexOffset, u32 vertexCount);
	};

	CBillboard_Exec_SOA_OAS		m_Exec_SAO2AOS;

	class	CBilboard_Exec_BillboardInfo
	{
	public:
		struct	SBillboardInfo
		{
			u32			m_Flags;
			float		m_NormalBendingFactor;
		};

		TMemoryView<SBillboardInfo>		m_GeomDrawRequests;

		void	Clear() { Mem::Reinit(*this); }
		bool	Empty() const { return m_GeomDrawRequests.Empty(); }
		void	operator()(const Drawers::CCopyStream_CPU &bb);
		void	operator()(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests);
	private:
		void	_Prepare(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests);
	};

	CBilboard_Exec_BillboardInfo	m_Exec_BillboardInfo;
};

//----------------------------------------------------------------------------
__PK_API_END
