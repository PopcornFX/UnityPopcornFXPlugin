//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_geometrics/include/ge_mesh.h>

#include <pk_particles/include/Renderers/ps_renderer_billboard.h>
#include <pk_particles/include/Renderers/ps_renderer_ribbon.h>
#include <pk_particles/include/Renderers/ps_renderer_mesh.h>
#include <pk_particles/include/Renderers/ps_renderer_triangle.h>

#include <pk_render_helpers/include/draw_requests/rh_billboard.h>
#include <pk_render_helpers/include/draw_requests/rh_ribbon.h>
#include <pk_render_helpers/include/draw_requests/rh_mesh.h>
#include <pk_render_helpers/include/draw_requests/rh_triangle.h>
#include "pk_render_helpers/include/frame_collector/rh_particle_render_data_factory.h"

#include <pk_render_helpers/include/frame_collector/rh_particle_render_data_factory.h>

#include "NativeToManaged.h"
#include "ImplemGraphicsAPI/RenderAPI_Data.h"
#if		defined(PK_ORBIS)
#include "ImplemGraphicsAPI/RenderAPI_GNMData.h"
#elif	defined(PK_UNKNOWN2)
#include "ImplemGraphicsAPI/RenderAPI_AGCData.h"
#endif


#define		STATIC_MESH_DESCRIPTOR_ARRAY_COUNT	3
#define		STATIC_SUBMESH_ARRAY_COUNT			5

// We might want to make this dynamic in the future so that it can
// be set from the C#. It will require to implement the missing
// permutations of slice buffer copies.
const int				kInitialVertexCount = 0xFF;
const int				kInitialIndexCount = kInitialVertexCount * 3;
const u32				kMaximumIndexPerParticle = 12;

namespace	PopcornFX
{
	class	CParticleRenderer_Billboard;
	class	CParticleRenderer_Ribbon;
	class	CParticleRenderer_Triangle;
	class	CUnityRenderDataFactory;
}

class	CFrameCollector;

namespace	ShaderVariationFlags
{
	// This will actually change which shader variation is used to draw the particles:
	enum	EShaderVariationFlags
	{
		Has_CorrectDeformation		= (1 << 0),
		Has_AnimBlend				= (1 << 1),
		Has_AlphaRemap				= (1 << 2),
		Has_Lighting				= (1 << 3),
		Has_LightingLegacy			= (1 << 4),
		Has_Soft					= (1 << 5),
		Has_DistortionMap			= (1 << 6),
		Has_Color					= (1 << 7),
		Has_DiffuseMap				= (1 << 8),
		Has_DoubleSided				= (1 << 9),
		Has_CastShadow				= (1 << 10),
		Has_Atlas					= (1 << 11),
		Has_Size2					= (1 << 12),
		Has_DiffuseRamp				= (1 << 13),
		Has_FluidVAT				= (1 << 14),
		Has_SoftVAT					= (1 << 15),
		Has_RigidVAT				= (1 << 16),
		Has_Emissive				= (1 << 17),
		Has_EmissiveRamp			= (1 << 18),
		Has_SkeletalAnim			= (1 << 19),
		Has_SkeletalInterpol		= (1 << 20),
		Has_SkeletalTrackInterpol	= (1 << 21),
		Has_TransformUVs			= (1 << 22),
		Has_LightingLegacyOpaque	= (1 << 23),
		Has_AlphaMasks				= (1 << 24),
		Has_UVDistortions			= (1 << 25),
		Has_Dissolve				= (1 << 26),
		ShaderVariation_Count		= 27
	};
}

namespace	UVGeneration
{
	enum	EUVGeneration
	{
		RotateUV	= (1 << 0),
		FlipU		= (1 << 1),
		FlipV		= (1 << 2)
	};
}

namespace	BlendMode
{
	// This will set different uniform flags in the shader:
	enum	EBlendMode
	{
		// Transparent
		Additive = 0,
		AdditiveNoAlpha,
		AlphaBlend,
		PremultipliedAlpha,
		// Opaque:
		Solid,
		Masked,
		BlendMode_Count,
	};
}

//----------------------------------------------------------------------------

class	CParticleMaterialDescFlags
{
public:
	// Material description (changes the billboarding and the inputs):
	u32						m_ShaderVariationFlags;

	// Blending mode:
	BlendMode::EBlendMode	m_BlendMode;

	u32						m_UVGenerationFlags;

	s32						m_DrawOrder;

	// Relates to editor legacy or default materials
	bool					m_IsLegacy;

	CParticleMaterialDescFlags();

	bool		HasShaderVariationFlags(u32 flags) const;
	bool		HasUVGenerationFlags(u32 flags) const;
	bool		NeedSort() const;
	bool		operator == (const CParticleMaterialDescFlags &oth) const;

	u32		CombineFlags();
};

//----------------------------------------------------------------------------

class	CParticleMaterialDescBillboard
{
public:
	CParticleMaterialDescFlags	m_Flags;

	// Shader uniforms:
	CStringId					m_DiffuseMap;
	CStringId					m_AlphaMap;
	float						m_InvSoftnessDistance;
	float						m_AlphaThreshold;

	//Atlas
	CStringId					m_AtlasDefinition;
	u32							m_AtlasSubdivX;
	u32							m_AtlasSubdivY;

	// Ribbon alignment mode:
	u32							m_RibbonAlignment;
	// Lit Feature
	CStringId					m_NormalMap;
	CStringId					m_RoughMetalMap;
	bool						m_CastShadows;
	float						m_NormalBendingFactor;
	float						m_Roughness;
	float						m_Metalness;

	CStringId					m_DiffuseRampMap;
	CStringId					m_EmissiveMap;
	CStringId					m_EmissiveRampMap;
	bool						m_IsEmissive3 = false;

	// Animated Masked Feature
	CStringId					m_AlphaMasks1Map;
	CStringId					m_AlphaMasks2Map;
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

	// UV Distortion Features
	CStringId					m_UVDistortions1Map;
	CStringId					m_UVDistortions2Map;
	float						m_UVDistortions1Intensity;
	float						m_UVDistortions2Intensity;
	CFloat2						m_UVDistortions1Scale;
	CFloat2						m_UVDistortions2Scale;
	float						m_UVDistortions1RotationSpeed;
	float						m_UVDistortions2RotationSpeed;
	CFloat2						m_UVDistortions1TranslationSpeed;
	CFloat2						m_UVDistortions2TranslationSpeed;

	// Dissolve
	CStringId					m_DissolveMap;
	float						m_DissolveSoftness;

	//TransformUVs Feature
	bool						m_TransformUVs_RGBOnly;

	CParticleMaterialDescBillboard();

	bool		InitFromRenderer(const CRendererDataBase &renderer);

	bool		operator == (const CParticleMaterialDescBillboard &oth) const;
};

//----------------------------------------------------------------------------

class	CParticleMaterialDescMesh
{
public:
	// Should be in an array of mesh descriptor when we will handle multiple meshes:
	CParticleMaterialDescFlags				m_Flags;

	CStringId								m_MeshPath;
	CStringId								m_DiffuseMap;

	bool									m_HasMeshAtlas;

	CStringId								m_NormalMap;
	CStringId								m_RoughMetalMap;

	bool									m_CastShadows;

	float									m_NormalBendingFactor;
	float									m_Roughness;
	float									m_Metalness;

	// VAT
	CStringId								m_Vat_PositionMap;
	CStringId								m_Vat_NormalMap;
	CStringId								m_Vat_ColorMap;
	CStringId								m_Vat_RotationMap;
	int										m_Vat_NumFrames;
	bool									m_Vat_PackedData;
	CFloat2									m_Vat_BoundsPivot;
	bool									m_Vat_NormalizedData;
	CFloat2									m_Vat_BoundsPosition;
	bool									m_Vat_PadToPowerOf2;
	CFloat2									m_Vat_PaddedRatio;

	// Skeletal anim:
	CStringId								m_SkeletalAnimationTexture;
	bool									m_SkeletalAnimUseBoneScale;
	CUint2									m_SkeletalAnimTextureResol;
	u32										m_SkeletalAnimCount;
	CFloat3									m_SkeletalAnimTranslationBoundsMin;
	CFloat3									m_SkeletalAnimTranslationBoundsMax;
	CFloat3									m_SkeletalAnimScaleBoundsMin;
	CFloat3									m_SkeletalAnimScaleBoundsMax;

	CStringId								m_DiffuseRampMap;
	CStringId								m_EmissiveMap;
	CStringId								m_EmissiveRampMap;
	CStringId								m_AlphaMap;

	// Animated Masked Feature
	CStringId								m_AlphaMasks1Map;
	CStringId								m_AlphaMasks2Map;
	float									m_AlphaMasks1Intensity;
	float									m_AlphaMasks2Intensity;
	float									m_AlphaMasks1Weight;
	float									m_AlphaMasks2Weight;
	CFloat2									m_AlphaMasks1Scale;
	CFloat2									m_AlphaMasks2Scale;
	float									m_AlphaMasks1RotationSpeed;
	float									m_AlphaMasks2RotationSpeed;
	CFloat2									m_AlphaMasks1TranslationSpeed;
	CFloat2									m_AlphaMasks2TranslationSpeed;

	// UV Distortion Features
	CStringId								m_UVDistortions1Map;
	CStringId								m_UVDistortions2Map;
	float									m_UVDistortions1Intensity;
	float									m_UVDistortions2Intensity;
	CFloat2									m_UVDistortions1Scale;
	CFloat2									m_UVDistortions2Scale;
	float									m_UVDistortions1RotationSpeed;
	float									m_UVDistortions2RotationSpeed;
	CFloat2									m_UVDistortions1TranslationSpeed;
	CFloat2									m_UVDistortions2TranslationSpeed;

	// Dissolve
	CStringId								m_DissolveMap;
	float									m_DissolveSoftness;

	float									m_InvSoftnessDistance;
	float									m_AlphaThreshold;

	bool									m_TransformUVs_RGBOnly;
	bool									m_UseVertexColor;


	// Should break batching (no batching for meshes right now so this is useless):
	CStringId								m_AtlasPath;
	CUint2									m_AtlasSubdivs;

	CParticleMaterialDescMesh();

	bool		InitFromRenderer(const CRendererDataMesh &renderer);

	bool		operator == (const CParticleMaterialDescMesh &oth) const;
};


class CParticleMaterialDescSound
{
public:
	CStringId	m_SoundData;

	bool InitFromRenderer(const CRendererDataSound &renderer);

	bool		operator == (const CParticleMaterialDescSound &oth) const;
};

class CParticleMaterialDescDecal
{
public:
	CParticleMaterialDescFlags	m_Flags;
	CStringId					m_DiffuseMap;
	CStringId					m_EmissiveMap;
	CFloat4						m_DiffuseColor;
	CFloat3						m_EmissiveColor;

	bool	InitFromRenderer(const CRendererDataDecal &renderer);

	bool	operator == (const CParticleMaterialDescDecal &oth) const;
};

struct	SUnityMeshInfoPerView
{
	// Unity mesh handler:
	int							m_RendererGUID;

	// Unity mesh data:
	// CPU + GPU Billboarding:
	SBufferHandles				m_VBHandler;
	// GPU Billboarding:
	SBufferHandles				m_IBHandler;
	SBufferHandles				m_InfoHandler;
	SBufferHandles				m_AtlasesHandler;
	SBufferHandles				m_IndirectArgsHandler;

	SUnityMeshInfoPerView()
	: m_RendererGUID(-1)
	, m_VBHandler()
	, m_IBHandler()
	, m_InfoHandler()
	, m_AtlasesHandler()
	, m_IndirectArgsHandler()
	{

	}

	bool	Init(UnityGfxRenderer deviceType)
	{
		m_VBHandler.Init(deviceType);
		m_IBHandler.Init(deviceType);
		m_InfoHandler.Init(deviceType);
		m_AtlasesHandler.Init(deviceType);
		m_IndirectArgsHandler.Init(deviceType);

		return true;
	}

};

struct		SUnityMeshInfo
{
	s32							m_VBElemCount;
	s32							m_IBElemCount;

	u32							m_SemanticOffsets[__Semantic_Count];
	u32							m_VertexStride;

	ManagedBool					m_LargeIndices;

	CAABB						m_MeshBounds;

	s32							m_InfoSize;

	s32							m_AtlasesSize;

	s32							m_IndirectArgsParticleCountMultiplier;

	SUnityMeshInfo()
	: m_VBElemCount(0)
	, m_IBElemCount(0)
	, m_VertexStride(0)
	, m_LargeIndices(ManagedBool_False)
	, m_MeshBounds(CAABB::DEGENERATED)
	, m_InfoSize(-1)
	, m_AtlasesSize(-1)
	, m_IndirectArgsParticleCountMultiplier(-1)
	{
		for (u32 i = 0; i < __Semantic_Count; ++i)
			m_SemanticOffsets[i] = 0;
	}
};

class	CUnityRendererCache : public CRendererCacheBase
{
public:
	CUnityRendererCache(const CUnityRenderDataFactory *renderDataFactory)
	:	m_RenderDataFactory(renderDataFactory)
	,	m_AssetName("")
	,	m_HasCustomMat(false)
	,	m_CustomMatID(-1)
	,	m_UID(0)
	{
	}

	virtual ~CUnityRendererCache();


private:
	const CUnityRenderDataFactory		*m_RenderDataFactory;
public:

	const CUnityRenderDataFactory		*RenderDataFactory() const { return m_RenderDataFactory; }

	template<typename _T>
	bool								GameThread_SetupRenderer(const _T *renderer);

	void								SetAssetName(const CString& assetName);
	const CString						&GetAssetName() const;

	void								GetRendererLitFeatureInfo(SRenderingFeatureLitDesc **litRenderingdesc);
	bool								GetRendererInfo(SPopcornRendererDesc &desc);
	bool								GetRendererInfo(SDecalRendererDesc &desc);
	bool								GetRendererInfo(SMeshRendererDesc &desc);

	void								CreateUnityMesh(u32 idx, bool gpuBillboarding, bool isEmissive3);

	virtual void						UpdateThread_BuildBillboardingFlags(const PRendererDataBase &renderer) override;


	bool								operator == (const CUnityRendererCache &oth) const;

	// Material description:
	ERendererClass								m_RendererType;
	Drawers::SRibbon_BillboardingRequest		m_RibbonBR;
	Drawers::SBillboard_BillboardingRequest		m_BillboardBR;
	Drawers::SMesh_BillboardingRequest			m_MeshBR;
	Drawers::STriangle_BillboardingRequest		m_TriangleBR;
	// For the Billboards and Ribbons:
	CParticleMaterialDescBillboard				m_MaterialDescBillboard;
	// For the Meshes:
	CParticleMaterialDescMesh					m_MaterialDescMesh;
	CParticleMaterialDescSound					m_MaterialDescSound;
	CParticleMaterialDescDecal					m_MaterialDescDecal;

	CString										m_AssetName;
	
	TArray<SUnityMeshInfoPerView>				m_UnityMeshInfoPerViews;
	SUnityMeshInfo								m_UnityMeshInfo;

	bool										m_HasCustomMat;
	int											m_CustomMatID;
	unsigned int								m_UID;
};
PK_DECLARE_REFPTRCLASS(UnityRendererCache);

template<> bool		CUnityRendererCache::GameThread_SetupRenderer<CRendererDataBillboard>(const CRendererDataBillboard *renderer);
template<> bool		CUnityRendererCache::GameThread_SetupRenderer<CRendererDataRibbon>(const CRendererDataRibbon *renderer);
template<> bool		CUnityRendererCache::GameThread_SetupRenderer<CRendererDataMesh>(const CRendererDataMesh *renderer);
template<> bool		CUnityRendererCache::GameThread_SetupRenderer<CRendererDataTriangle>(const CRendererDataTriangle *renderer);

//----------------------------------------------------------------------------
