//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "PKUnity_InterfaceCommon.h"

#include <pk_particles/include/ps_scene.h>

extern "C"
{
	//----------------------------------------------------------------------------
	// Data structures:
	//----------------------------------------------------------------------------

	// Ray-cast pack:
	struct	SRaycastPack
	{
		const CFloat4	*m_RayOrigins;
		const CFloat4	*m_RayDirections;
		CFloat4			*m_OutNormals;
		CFloat4			*m_OutPositions;
		float			*m_OutDistances;
		int				m_FilterLayer;
		int				m_RayCount;
	};

	struct SRenderingFeatureLitDesc
	{
		const char				*m_NormalMap;
		const char				*m_RoughMetalMap;

		ManagedBool				m_CastShadows;

		float					m_NormalBendingFactor;
		float					m_Roughness;
		float					m_Metalness;

		SRenderingFeatureLitDesc()
			: m_NormalMap(null)
			, m_RoughMetalMap(null)
			, m_CastShadows(ManagedBool_False)
			, m_NormalBendingFactor(0.5f)
			, m_Roughness(1)
			, m_Metalness(0)
		{

		}
	};

	// Create Renderers:
	// Billboards and ribbons:
	struct SPopcornRendererDesc
	{
		int							m_ShaderVariationFlags;
		int							m_BlendMode;
		ManagedBool					m_RotateUvs;

		const char					*m_DiffuseMap;
		const char					*m_EmissiveMap;
		const char					*m_AlphaRemap;
		const char					*m_DiffuseRampMap;
		const char					*m_EmissiveRampMap;
		float						m_InvSoftnessDistance;
		float						m_AlphaClipThreshold;

		int							m_BillboardMode;
		int							m_DrawOrder;

		SRenderingFeatureLitDesc	*m_LitRendering;

		int							m_CameraID;

		SPopcornRendererDesc()
		:	m_ShaderVariationFlags(0)
		,	m_BlendMode(0)
		,	m_RotateUvs(ManagedBool_False)
		,	m_DiffuseMap(null)
		,	m_EmissiveMap(null)
		,	m_AlphaRemap(null)
		,	m_DiffuseRampMap(null)
		,	m_EmissiveRampMap(null)
		,	m_InvSoftnessDistance(0)
		,	m_AlphaClipThreshold(0)
		,	m_BillboardMode(0)
		,	m_DrawOrder(0)
		,	m_LitRendering(null)
		,	m_CameraID(0)
		{
		}
	};

	struct SRenderingFeatureVATDesc
	{
		const char*		m_PositionMap;
		const char*		m_NormalMap;
		const char*		m_ColorMap;
		const char*		m_RotationMap;
		int				m_NumFrames;
		ManagedBool		m_PackedData;
		CFloat2			m_BoundsPivot;
		ManagedBool		m_NormalizedData;
		CFloat2			m_BoundsPosition;
		ManagedBool		m_PadToPowerOf2;
		CFloat2			m_PaddedRatio;

		SRenderingFeatureVATDesc()
			: m_PositionMap(null)
			, m_NormalMap(null)
			, m_ColorMap(null)
			, m_RotationMap(null)
			, m_NumFrames(0)
			, m_PackedData(ManagedBool_False)
			, m_BoundsPivot{}
			, m_NormalizedData(ManagedBool_False)
			, m_BoundsPosition{}
			, m_PadToPowerOf2(ManagedBool_False)
			, m_PaddedRatio{}
		{
		}
	};

	struct SRenderingFeatureSkeletalAnimDesc
	{
		const char		*m_AnimTexture;
		CUint2			m_TextureResol;
		u32				m_AnimCount;
		ManagedBool		m_UseBoneScale;
		CFloat3			m_TranslationBoundsMin;
		CFloat3			m_TranslationBoundsMax;
		CFloat3			m_ScaleBoundsMin;
		CFloat3			m_ScaleBoundsMax;

		SRenderingFeatureSkeletalAnimDesc()
			: m_AnimTexture(null)
			, m_TextureResol(CUint2::ZERO)
			, m_AnimCount(0)
			, m_UseBoneScale(ManagedBool_False)
			, m_TranslationBoundsMin(CFloat3::ZERO)
			, m_TranslationBoundsMax(CFloat3::ZERO)
			, m_ScaleBoundsMin(CFloat3::ZERO)
			, m_ScaleBoundsMax(CFloat3::ZERO)
		{
		}
	};

	// Meshes:
	struct SMeshRendererDesc
	{
		// Common info for the renderers:
		const char					*m_MeshAsset;
		int							m_ShaderVariationFlags;
		int							m_BlendMode;
		// ------------------------------
		ManagedBool					m_HasMeshAtlas;

		const char					*m_DiffuseMap;
		const char					*m_EmissiveMap;
		const char					*m_AlphaRemap;
		const char					*m_DiffuseRampMap;
		const char					*m_EmissiveRampMap;
		float						m_InvSoftnessDistance;
		float						m_AlphaClipThreshold;
		bool						m_DoubleSided;

		SRenderingFeatureLitDesc			*m_LitRendering;
		SRenderingFeatureVATDesc			*m_VatRendering;
		SRenderingFeatureSkeletalAnimDesc	*m_AnimDescRendering;

		int							m_TextureAtlasCount;
		CFloat4						*m_TextureAtlas;

		int							m_DrawOrder;

		SMeshRendererDesc()
			: m_MeshAsset(null)
			, m_ShaderVariationFlags(0)
			, m_BlendMode(0)
			, m_HasMeshAtlas(ManagedBool_False)
			, m_DiffuseMap(null)
			, m_EmissiveMap(null)
			, m_AlphaRemap(null)
			, m_DiffuseRampMap(null)
			, m_EmissiveRampMap(null)
			, m_InvSoftnessDistance(0)
			, m_AlphaClipThreshold(0)
			, m_DoubleSided(false)
			, m_LitRendering(null)
			, m_VatRendering(null)
			, m_AnimDescRendering(null)
			, m_TextureAtlasCount(0)
			, m_TextureAtlas(null)
			, m_DrawOrder(0)
		{
		}

		~SMeshRendererDesc()
		{
			PK_SAFE_DELETE(m_LitRendering);
			m_LitRendering = null;

			PK_SAFE_DELETE(m_VatRendering);
			m_VatRendering = null;

			PK_SAFE_DELETE(m_AnimDescRendering);
			m_AnimDescRendering = null;

			PK_FREE(m_TextureAtlas);
			m_TextureAtlas = null;
		}
	};

	// Bounds
	struct	SUpdateRendererBounds
	{
		CFloat3			m_Min;
		CFloat3			m_Max;
	};

	// Effect info:
	struct	SRetrieveRendererInfo
	{
		ManagedBool	*m_IsIndex32;

		int			*m_VertexBufferSize;
		int			*m_IndexBufferSize;
		int			*m_InfoBSize;
		int			*m_AtlasesBSize;
		int			*m_IndirectArgsParticleCountMultiplier;

		void		**m_VBHandler;
		void		**m_IBHandler;
		void		**m_InfoBHandler;
		void		**m_AtlasesBHandler;
		void		**m_IndirectArgsBHandler;
		ManagedBool	*m_UseComputeBuffers;

		ManagedBool	*m_HasCustomMaterial;
		int			*m_CustomMatID;
	};

	//----------------------------------------------------------------------------
	// Callbacks:
	//----------------------------------------------------------------------------

#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility push(default)
#endif

	// Fake file system manager:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResourceLoad(void *delegatePtr);
	unsigned long long							OnResourceLoad(const char *path, void **handler);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResourceWrite(void *delegatePtr);
	unsigned long long							OnResourceWrite(const char *path, const void *data, u64 offset, u64 size);

	// Particles to Unity interactions:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastPack(void *delegatePtr);
	void										OnRaycastPack(const SRaycastPack *raycastPack);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnFxStopped(void *delegatePtr);
	void										OnFxStopped(int guid);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaiseEvent(void *delegatePtr);
	void										OnRaiseEvent(unsigned int guid, unsigned int key, const char* eventName, unsigned int payloadCount, void *payloadDescs, void *payloadValues);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAudioWaveformData(void *delegatePtr);
	void										*OnGetAudioWaveformData(const char *name, int *nbSamples);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAudioSpectrumData(void *delegatePtr);
	void										*OnGetAudioSpectrumData(const char *name, int *nbSamples);

	// Rendering interactions:
	// Create renderers:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewBillboardRenderer(void *delegatePtr);
	int											OnSetupNewBillboardRenderer(const SPopcornRendererDesc *rendererDesc, int idx);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewRibbonRenderer(void *delegatePtr);
	int											OnSetupNewRibbonRenderer(const SPopcornRendererDesc *rendererDesc, int idx);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewMeshRenderer(void *delegatePtr);
	int											OnSetupNewMeshRenderer(const SMeshRendererDesc *rendererDesc, int idx);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewTriangleRenderer(void *delegatePtr);
	int											OnSetupNewTriangleRenderer(const SPopcornRendererDesc *rendererDesc, int idx);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResizeRenderer(void *delegatePtr);
	ManagedBool									OnResizeRenderer(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetParticleCount(void *delegatePtr);
	void										OnSetParticleCount(int rendererGUID, int particleCount);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetRendererActive(void *delegatePtr);
	void										OnSetRendererActive(int rendererGUID, ManagedBool active);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetMeshInstancesCount(void *delegatePtr);
	void										OnSetMeshInstancesCount(int rendererGUID, int submesh, int instancesCount);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetMeshInstancesBuffer(void *delegatePtr);
	void										OnSetMeshInstancesBuffer(int rendererGUID, int submesh, void *instanceBuffer);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRetrieveCustomMaterialInfo(void *delegatePtr);
	void										OnRetrieveCustomMaterialInfo(int type, const void *rendererDesc, int idx, ManagedBool *hasCustomMaterial, int* customMaterialID);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRetrieveRendererBufferInfo(void *delegatePtr);
	void										OnRetrieveRendererBufferInfo(int rendererGUID, const SRetrieveRendererInfo *info);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnUpdateRendererBounds(void *delegatePtr);
	void										OnUpdateRendererBounds(int rendererGUID, const SUpdateRendererBounds *bounds);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshCount(void *delegatePtr);
	int											OnGetMeshCount(int rendererGUID, int lod);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshLODsCount(void* delegatePtr);
	int											OnGetMeshLODsCount(int rendererGUID);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshBounds(void *delegatePtr);
	void										OnGetMeshBounds(int rendererGUID, int submesh, void* bbox);

#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility pop
#endif

	void										ClearNativeToManagedCallbacks();
}

//----------------------------------------------------------------------------
