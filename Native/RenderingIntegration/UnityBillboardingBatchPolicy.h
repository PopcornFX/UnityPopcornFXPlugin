//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_refcounted_buffer.h>
#include "FrameCollectorUnityTypes.h"
#include "UnityRendererCache.h"
#include "PKUnity_InterfaceCommon.h"
#include "ManagedToNative.h"
#include "ImplemGraphicsAPI/RenderAPI_Data.h"

struct	SParticleBuffers;
class	CFrameCollector;
class	CUnityRenderDataFactory;

__PK_API_BEGIN

//----------------------------------------------------------------------------

class	CUnityBillboardingBatchPolicy
{
public:
	CUnityBillboardingBatchPolicy();
	~CUnityBillboardingBatchPolicy();

	bool			Init(UnityGfxRenderer deviceType, CUnityRenderDataFactory *renderDataFactory);

	// Only called when not empty:
	static bool		CanRender(const Drawers::SBillboard_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SRibbon_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SMesh_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SLight_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::SSound_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		CanRender(const Drawers::STriangle_DrawRequest *request, const PRendererCacheBase &rendererCache, SUnityRenderContext &ctx);
	static bool		IsStateless() { return false; }

	// Do not remove
	bool		Tick(SUnityRenderContext &, const TMemoryView<SUnitySceneView> &);

	void		ClearBatch();
	void		CustomStepFlagInactive();
	bool		AllocBuffers(SUnityRenderContext &ctx, const SBuffersToAlloc &allocBuffers, const TMemoryView<SUnitySceneView> &views, ERendererClass rendererType);

	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SGPUBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SRibbonBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SGPURibbonBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SMeshBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, STriangleBatchJobs *batchJobs, const SGeneratedInputs &toMap);
	
	bool		MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SGPUTriangleBatchJobs *batchJobs, const SGeneratedInputs &toMap);

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
	void		_UpdateThread_SetUnityMeshBounds(const SBuffersToAlloc &allocBuffers, const TMemoryView<SUnitySceneView> &views);

	bool		_UpdateThread_IssueDrawCallLight(const Drawers::SLight_DrawRequest *lightRequest, CUnityRendererCache *rdrCache);
	bool		_UpdateThread_IssueDrawCallSound(const Drawers::SSound_DrawRequest *soundRequest, CUnityRendererCache *rdrCache, u32 &totalParticleCount);

	void		_UpdateThread_ResizeUnityMeshInstanceCount(const SBuffersToAlloc &allocBuffers, IRenderAPIData *renderApiData);

	bool		_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData);
	bool		_RenderThread_AllocBillboardingBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData);
	bool		_RenderThread_SetupBuffersBillboards(const SGeneratedInputs &toMap, SBillboardBatchJobs *billboardBatch);
	bool		_RenderThread_SetupBuffersGeomBillboards(const SGeneratedInputs &toMap, SGPUBillboardBatchJobs *billboardBatch);
	bool		_RenderThread_SetupBuffersRibbons(const SGeneratedInputs &toMap, SRibbonBatchJobs *ribbonBatch);
	bool		_RenderThread_SetupBuffersMeshes(const SGeneratedInputs &toMap, SMeshBatchJobs *meshBatch);
	bool		_RenderThread_SetupBuffersTriangles(const SGeneratedInputs &toMap, STriangleBatchJobs *triangleBatch);
	bool		_FindAdditionalInput(const CStringId &inputName, EBaseTypeID inputType, const SGeneratedInputs &genInputs);

	bool		_PrepareCopySOA2AOS(const SUnityRenderContext &ctx, u32 idx);


	CUnityRenderDataFactory			*m_RenderDataFactory;

	// ----------------------------------------------------------------------------
	// FILLED ON GAME THREAD - ALLOC UNITY BUFFERS:
	// ----------------------------------------------------------------------------

	TArray<SLightInfo>				m_LightDatas;

	// --------------------------------------
	// We keep a local copy of the reference material description for this mesh:
	// --------------------------------------
	ERendererClass					m_RendererType;
	CParticleMaterialDescBillboard	m_MaterialDescBillboard;
	CParticleMaterialDescMesh		m_MaterialDescMesh;

	TArray<SUnityMeshInfoPerView>	m_UnityMeshInfoPerViews;
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
	bool							m_HasAtlas;
	bool							m_UseSkeletalAnimData;
	bool							m_UseSkeletalAnimInterpolTracksData;

	u32								m_UnusedFrameCount;

	bool							m_HasTransformUV;

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
			SSizedBuffer<CFloat4>	m_Tangents;
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
				m_Tangents.FreeIFN();
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
		SSizedBuffer<CFloat3>	m_EmissiveColors;
		SSizedBuffer<float>		m_AlphaCursor;
		SSizedBuffer<float>		m_TransformUVsRotate;
		SSizedBuffer<CFloat2>	m_TransformUVsScale;
		SSizedBuffer<CFloat2>	m_TransformUVsOffset;

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
			m_EmissiveColors.FreeIFN();
			m_AlphaCursor.FreeIFN();

			m_TransformUVsRotate.FreeIFN();
			m_TransformUVsScale.FreeIFN();
			m_TransformUVsOffset.FreeIFN();
		}
	};

	// Buffers in which we are going to billboard the particles
	SParticleBuffers			m_ParticleBuffers;

	struct		SMeshParticleBuffers
	{
		TStridedMemoryView<CFloat4x4>			m_Transforms;
		TStridedMemoryView<CFloat4>				m_Colors;
		TStridedMemoryView<CFloat3>				m_EmissiveColors;
		TStridedMemoryView<float>				m_AlphaRemapCursor;
		TStridedMemoryView<float>				m_VATCursors;

		TStridedMemoryView<float>				m_AtlasId;

		// Skeletal anim:
		TStridedMemoryView<u32>					m_AnimIdx0;
		TStridedMemoryView<float>				m_AnimCursor0;
		TStridedMemoryView<u32>					m_AnimIdx1;
		TStridedMemoryView<float>				m_AnimCursor1;
		TStridedMemoryView<float>				m_TransitionCursor;

		TStridedMemoryView<float>				m_TransformUVRotate;
		TStridedMemoryView<CFloat2>				m_TransformUVOffset;
		TStridedMemoryView<CFloat2>				m_TransformUVScale;
	};

	TArray<Drawers::SCopyFieldDescPerMesh>	m_MeshAdditionalField;
	TArray<SMeshParticleBuffers>			m_PerMeshBuffers;
	PRefCountedMemoryBuffer					m_MeshBillboardingBuffer;

	// Mapped Unity buffers:
	TArray<void*>				m_MappedVtxBuffer;
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
		CFloat4					*m_Tangents;
		CFloat2					*m_UVFactors;
		CFloat2					*m_TexCoords0;
		CFloat2					*m_TexCoords1;
		float					*m_AtlasId;
		CFloat4					*m_UVRemap;
		CFloat4					*m_Colors;
		CFloat3					*m_EmissiveColors;
		float					*m_AlphaCursor;

		float					*m_TransformUVsRotate;
		CFloat2					*m_TransformUVsOffset;
		CFloat2					*m_TransformUVsScale;

		SParticleSourceBuffers()
		{
			Clear();
		}

		void	FromParticleBuffers(const SParticleBuffers &buffers, u32 viewIdx)
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
			if (buffers.m_GeneratedInputs & Drawers::GenInput_Tangent)
				m_Tangents = buffers.m_ViewIndependantGeom.m_Tangents.m_Ptr;
			else
				m_Tangents = null;
			if (buffers.m_GeneratedInputs & Drawers::GenInput_UVFactors)
				m_UVFactors = buffers.m_ViewIndependantGeom.m_UVFactors.m_Ptr;
			else
				m_UVFactors = null;

			m_TexCoords0 = buffers.m_TexCoords0.m_Ptr;
			m_TexCoords1 = buffers.m_TexCoords1.m_Ptr;
			m_AtlasId = buffers.m_AtlasId.m_Ptr;
			m_UVRemap = buffers.m_UVRemap.m_Ptr;
			m_Colors = buffers.m_Colors.m_Ptr;
			m_EmissiveColors = buffers.m_EmissiveColors.m_Ptr;
			m_AlphaCursor = buffers.m_AlphaCursor.m_Ptr;
			m_TransformUVsRotate = buffers.m_TransformUVsRotate.m_Ptr;
			m_TransformUVsOffset = buffers.m_TransformUVsOffset.m_Ptr;
			m_TransformUVsScale = buffers.m_TransformUVsScale.m_Ptr;

			if (buffers.m_PerViewGeom.Count() > viewIdx)
			{
				if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Indices)
					m_Indices = buffers.m_PerViewGeom[viewIdx].m_Indices;
				if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Position)
					m_Positions = buffers.m_PerViewGeom[viewIdx].m_Positions.m_Ptr;
				if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Normal)
					m_Normals = buffers.m_PerViewGeom[viewIdx].m_Normals.m_Ptr;
				if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Tangent)
					m_Tangents = buffers.m_PerViewGeom[viewIdx].m_Tangents.m_Ptr;
				if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_UVFactors)
					m_UVFactors = buffers.m_PerViewGeom[viewIdx].m_UVFactors.m_Ptr;
			}
		}

		void	Clear()
		{
			m_Indices = null;
			m_Positions = null;
			m_Normals = null;
			m_Tangents = null;
			m_UVFactors = null;
			m_TexCoords0 = null;
			m_TexCoords1 = null;
			m_AtlasId = null;
			m_UVRemap = null;
			m_Colors = null;
			m_AlphaCursor = null;
			m_TransformUVsRotate = null;
			m_TransformUVsOffset = null;
			m_TransformUVsScale = null;
		}
	};

	class	CBillboard_Exec_SOA_OAS
	{
	public:
		u32						m_IdxView = 0;
		u32						m_ShaderVariationFlags;

		SParticleSourceBuffers	m_ParticleBuffers;		// Src
		TArray<void*>			m_MappedVertexBuffer;	// Dst vtx

		// Dst buffers info:
		u32						(*m_SemanticOffsets)[__Semantic_Count];
		u32						m_VertexStride;

		CBillboard_Exec_SOA_OAS() : m_ShaderVariationFlags(0), m_VertexStride(0) { Mem::Clear(m_SemanticOffsets); }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const Drawers::SBillboard_ExecPage &batch);
		void	operator()(const Drawers::SRibbon_ExecBatch &batch);
		void	operator()(const Drawers::STriangle_ExecPage &batch);

		void	_CopyData(u32 vertexOffset, u32 vertexCount);
	};

	TArray<CBillboard_Exec_SOA_OAS>		m_Exec_SAO2AOS;

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
