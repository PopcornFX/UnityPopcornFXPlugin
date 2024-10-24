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


	struct SRenderingFeatureAlphaMasksDesc
	{
		const char					*m_AlphaMasks1Map;
		const char					*m_AlphaMasks2Map;
		float						m_AlphaMasks1Intensity;
		float						m_AlphaMasks2Intensity;
		float						m_AlphaMasks1Weight;
		float						m_AlphaMasks2Weight;
		CFloat2						m_AlphaMasks1Scale;
		CFloat2						m_AlphaMasks2Scale;
		float						m_AlphaMasks1RotationSpeed;
		float						m_AlphaMasks2RotationSpeed;
		CFloat2						m_AlphaMasks1TranslationSpeed;
		CFloat2						m_AlphaMasks2TranslationSpeed;

		SRenderingFeatureAlphaMasksDesc()
			: m_AlphaMasks1Map(null)
			, m_AlphaMasks2Map(null)
			, m_AlphaMasks1Intensity(0.f)
			, m_AlphaMasks2Intensity(0.f)
			, m_AlphaMasks1Weight(0.f)
			, m_AlphaMasks2Weight(0.f)
			, m_AlphaMasks1Scale(CFloat2(0.f, 0.f))
			, m_AlphaMasks2Scale(CFloat2(0.f, 0.f))
			, m_AlphaMasks1RotationSpeed(0.f)
			, m_AlphaMasks2RotationSpeed(0.f)
			, m_AlphaMasks1TranslationSpeed(CFloat2(0.f, 0.f))
			, m_AlphaMasks2TranslationSpeed(CFloat2(0.f, 0.f))
		{
		}
	};

	struct SRenderingFeatureUVDistortionsDesc
	{
		const char					*m_UVDistortions1Map;
		const char					*m_UVDistortions2Map;
		float						m_UVDistortions1Intensity;
		float						m_UVDistortions2Intensity;
		CFloat2						m_UVDistortions1Scale;
		CFloat2						m_UVDistortions2Scale;
		float						m_UVDistortions1RotationSpeed;
		float						m_UVDistortions2RotationSpeed;
		CFloat2						m_UVDistortions1TranslationSpeed;
		CFloat2						m_UVDistortions2TranslationSpeed;

		SRenderingFeatureUVDistortionsDesc()
			: m_UVDistortions1Map(null)
			, m_UVDistortions2Map(null)
			, m_UVDistortions1Intensity(0.f)
			, m_UVDistortions2Intensity(0.f)
			, m_UVDistortions1Scale(CFloat2(0.f, 0.f))
			, m_UVDistortions2Scale(CFloat2(0.f, 0.f))
			, m_UVDistortions1RotationSpeed(0.f)
			, m_UVDistortions2RotationSpeed(0.f)
			, m_UVDistortions1TranslationSpeed(CFloat2(0.f, 0.f))
			, m_UVDistortions2TranslationSpeed(CFloat2(0.f, 0.f))
		{
		}
	};

	struct SRenderingFeatureDissolveDesc
	{
		const char	*m_DissolveMap;
		float		m_DissolveSoftness;

		SRenderingFeatureDissolveDesc()
			: m_DissolveMap(null)
			, m_DissolveSoftness(0.f)
		{
		}
	};

	// Create Renderers:
	// Billboards and ribbons:
	struct SPopcornRendererDesc
	{
		const char								*m_CustomName;

		int										m_ShaderVariationFlags;
		int										m_BlendMode;
		ManagedBool								m_IsLegacy;
		ManagedBool								m_RotateUvs;

		const char								*m_DiffuseMap;
		const char								*m_EmissiveMap;
		const char								*m_AlphaRemap;
		const char								*m_DiffuseRampMap;
		const char								*m_EmissiveRampMap;

		float									m_InvSoftnessDistance;
		float									m_AlphaClipThreshold;
		ManagedBool								m_TransformUVs_RGBOnly;

		int										m_BillboardMode;
		int										m_DrawOrder;

		SRenderingFeatureLitDesc				*m_LitRendering;
		SRenderingFeatureAlphaMasksDesc			*m_AlphaMasks;
		SRenderingFeatureUVDistortionsDesc		*m_UVDistortions;
		SRenderingFeatureDissolveDesc			*m_Dissolve;

		int										m_CameraID;
		int										m_UID;

		SPopcornRendererDesc()
		:	m_CustomName(null)
		,	m_ShaderVariationFlags(0)
		,	m_BlendMode(0)
		,	m_IsLegacy(ManagedBool_False)
		,	m_RotateUvs(ManagedBool_False)
		,	m_DiffuseMap(null)
		,	m_EmissiveMap(null)
		,	m_AlphaRemap(null)
		,	m_DiffuseRampMap(null)
		,	m_EmissiveRampMap(null)
		,	m_InvSoftnessDistance(0)
		,	m_AlphaClipThreshold(0)
		,	m_TransformUVs_RGBOnly(ManagedBool_False)
		,	m_BillboardMode(0)
		,	m_DrawOrder(0)
		,	m_LitRendering(null)
		,	m_AlphaMasks(null)
		,	m_UVDistortions(null)
		,	m_Dissolve(null)
		,	m_CameraID(0)
		,	m_UID(0)
		{
		}

		~SPopcornRendererDesc()
		{
			PK_SAFE_DELETE(m_LitRendering);
			m_LitRendering = null;

			PK_SAFE_DELETE(m_AlphaMasks);
			m_AlphaMasks = null;

			PK_SAFE_DELETE(m_UVDistortions);
			m_UVDistortions = null;

			PK_SAFE_DELETE(m_Dissolve);
			m_Dissolve = null;
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

	// Decals:
	struct SDecalRendererDesc
	{
		int				m_ShaderVariationFlags;

		const char		*m_DiffuseMap;
		const char		*m_EmissiveMap;

		CFloat4			m_DiffuseColor;
		CFloat3			m_EmissiveColor;

		int				m_TextureAtlasCount;
		CFloat4			*m_TextureAtlas;

		unsigned int	m_UID;

		SDecalRendererDesc()
			: m_ShaderVariationFlags(0)
			, m_DiffuseMap(null)
			, m_EmissiveMap(null)
			, m_DiffuseColor(CFloat4::ZERO)
			, m_EmissiveColor(CFloat3::ZERO)
			, m_TextureAtlasCount(0)
			, m_TextureAtlas(null)
			, m_UID(0)
		{
		}
	};


	// Meshes:
	struct SMeshRendererDesc
	{
		const char					*m_CustomName;

		// Common info for the renderers:
		const char					*m_MeshAsset;
		int							m_ShaderVariationFlags;
		int							m_BlendMode;
		ManagedBool					m_IsLegacy;
		// ------------------------------
		ManagedBool					m_HasMeshAtlas;

		const char					*m_DiffuseMap;
		const char					*m_EmissiveMap;
		const char					*m_AlphaRemap;
		const char					*m_DiffuseRampMap;
		const char					*m_EmissiveRampMap;
		float						m_InvSoftnessDistance;
		float						m_AlphaClipThreshold;

		SRenderingFeatureLitDesc			*m_LitRendering;
		SRenderingFeatureVATDesc			*m_VatRendering;
		SRenderingFeatureSkeletalAnimDesc	*m_AnimDescRendering;
		SRenderingFeatureAlphaMasksDesc		*m_AlphaMasks;
		SRenderingFeatureUVDistortionsDesc	*m_UVDistortions;
		SRenderingFeatureDissolveDesc		*m_Dissolve;

		int							m_TextureAtlasCount;
		CFloat4						*m_TextureAtlas;

		unsigned int				m_UID;
		int							m_DrawOrder;

		ManagedBool					m_TransformUVs_RGBOnly;
		ManagedBool					m_UseVertexColor;

		SMeshRendererDesc()
			: m_CustomName(null)
			, m_MeshAsset(null)
			, m_ShaderVariationFlags(0)
			, m_BlendMode(0)
			, m_IsLegacy(ManagedBool_False)
			, m_HasMeshAtlas(ManagedBool_False)
			, m_DiffuseMap(null)
			, m_EmissiveMap(null)
			, m_AlphaRemap(null)
			, m_DiffuseRampMap(null)
			, m_EmissiveRampMap(null)
			, m_InvSoftnessDistance(0)
			, m_AlphaClipThreshold(0)
			, m_LitRendering(null)
			, m_VatRendering(null)
			, m_AnimDescRendering(null)
			, m_AlphaMasks(null)
			, m_UVDistortions(null)
			, m_Dissolve(null)
			, m_TextureAtlasCount(0)
			, m_TextureAtlas(null)
			, m_UID(0)
			, m_DrawOrder(0)
			, m_TransformUVs_RGBOnly(ManagedBool_False)
			, m_UseVertexColor(ManagedBool_False)
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

			PK_SAFE_DELETE(m_AlphaMasks);
			m_AlphaMasks = null;

			PK_SAFE_DELETE(m_UVDistortions);
			m_UVDistortions = null;

			PK_SAFE_DELETE(m_Dissolve);
			m_Dissolve = null;

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

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResourceUnload(void* delegatePtr);
	bool										OnResourceUnload(const char* path);

	// Particles to Unity interactions:

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastStart(void *delegatePtr);
	void										OnRaycastStart(int count, void **cmd);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastPack(void *delegatePtr);
	void										OnRaycastPack(void **res);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSpherecastStart(void *delegatePtr);
	void										OnSpherecastStart(int count, void **cmd);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSpherecastPack(void *delegatePtr);
	void										OnSpherecastPack(void **res);

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

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewDecalRenderer(void *delegatePtr);
	int											OnSetupNewDecalRenderer(const SDecalRendererDesc *rendererDesc, int idx);

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

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetLightsBuffer(void *delegatePtr);
	void										OnSetLightsBuffer(void *lightInfos, int count);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetSoundsBuffer(void *delegatePtr);
	void										OnSetSoundsBuffer(void *soundInfos, int count);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetDecalsBuffer(void *delegatePtr);
	void										OnSetDecalsBuffer(void *decalInfos, int count);

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
