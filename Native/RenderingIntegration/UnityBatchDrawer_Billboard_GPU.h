//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_billboard_gpu.h>
#include "UnityRendererCache.h"
#include "FrameCollectorUnityTypes.h"

//----------------------------------------------------------------------------

class	CUnityBatchDrawer_Billboard_GPU : public PopcornFX::CRendererBatchJobs_Billboard_GPUBB
{
public:
	CUnityBatchDrawer_Billboard_GPU();
	~CUnityBatchDrawer_Billboard_GPU();

	virtual bool		Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass) override;

	virtual bool		AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const override;

	virtual bool		AllocBuffers(SRenderContext &ctx) override;
	virtual bool		CanRender(PopcornFX::SRenderContext &ctx) const override;
	virtual bool		MapBuffers(SRenderContext &ctx) override;
	virtual bool		UnmapBuffers(SRenderContext &ctx) override;
	virtual bool		LaunchCustomTasks(SRenderContext &ctx) override;

	virtual void		BeginFrame(PopcornFX::SRenderContext &ctx) override;
	virtual bool		EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc &toEmit) override;
	virtual void		Destroy() override;

private:
	void				_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active);
	void				_ClearBuffers();
	void				_UpdateThread_SetUnityMeshBounds();
	void				_UpdateThread_ResizeUnityMesh(const PopcornFX::SUnityRenderContext &ctx);
	bool				_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs);
	bool				_RenderThread_SetupBuffersGeomBillboards(const SGeneratedInputs &toMap);

private:
	TArray<SUnityMeshInfoPerView>	m_UnityMeshInfoPerViews;
	CParticleMaterialDescBillboard	m_MaterialDescBillboard;
	SUnityMeshInfo					m_UnityMeshInfo;
	CAABB							m_BBox;

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
	bool							m_IsEmissive3 = false;

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
		SSizedBuffer<CFloat2>	m_RawTexCoords0;
		SSizedBuffer<CFloat4>	m_UVRemap;

		// Additional fields that we handle:
		SSizedBuffer<float>		m_AtlasId;
		SSizedBuffer<CFloat4>	m_Colors;
		SSizedBuffer<CFloat3>	m_EmissiveColors3;
		SSizedBuffer<CFloat4>	m_EmissiveColors4;
		SSizedBuffer<float>		m_AlphaCursor;
		SSizedBuffer<float>		m_TransformUVsRotate;
		SSizedBuffer<CFloat2>	m_TransformUVsScale;
		SSizedBuffer<CFloat2>	m_TransformUVsOffset;
		SSizedBuffer<float>		m_AlphaMask1Cursor;
		SSizedBuffer<float>		m_AlphaMask2Cursor;
		SSizedBuffer<float>		m_UVDistortion1Cursor;
		SSizedBuffer<float>		m_UVDistortion2Cursor;
		SSizedBuffer<float>		m_DissolveCursor;

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
			m_RawTexCoords0.FreeIFN();
			m_AtlasId.FreeIFN();
			m_UVRemap.FreeIFN();
			m_Colors.FreeIFN();
			m_EmissiveColors3.FreeIFN();
			m_EmissiveColors4.FreeIFN();
			m_AlphaCursor.FreeIFN();

			m_TransformUVsRotate.FreeIFN();
			m_TransformUVsScale.FreeIFN();
			m_TransformUVsOffset.FreeIFN();

			m_AlphaMask1Cursor.FreeIFN();
			m_AlphaMask2Cursor.FreeIFN();
			m_UVDistortion1Cursor.FreeIFN();
			m_UVDistortion2Cursor.FreeIFN();
			m_DissolveCursor.FreeIFN();

		}
	};

	// Buffers in which we are going to billboard the particles
	SParticleBuffers			m_ParticleBuffers;

	// Mapped Unity buffers:
	TArray<void*>				m_MappedVtxBuffer;
	void						*m_MappedIdxBuffer;
	void						*m_MappedInfoBillboardBuffer;
	void						*m_MappedAtlasesBuffer;

	// --------------------------------------
	// FINAL SOA TO AOS COPY:
	// --------------------------------------

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
