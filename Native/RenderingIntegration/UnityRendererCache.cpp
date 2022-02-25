//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityRendererCache.h"
#include "UnityRenderDataFactory.h"
#include <pk_render_helpers/include/basic_renderer_properties/rh_basic_renderer_properties.h>

#include "NativeToManaged.h"
#include "RuntimeManager.h"

#include <pk_kernel/include/kr_resources.h>
#include <pk_geometrics/include/ge_mesh_resource.h>

//----------------------------------------------------------------------------

CParticleMaterialDescFlags::CParticleMaterialDescFlags()
:	m_ShaderVariationFlags(0)
,	m_BlendMode(BlendMode::Solid)
,	m_UVGenerationFlags(0)
,	m_DrawOrder(0)
{
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescFlags::HasShaderVariationFlags(u32 flags) const
{
	return (m_ShaderVariationFlags & flags) == flags;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescFlags::HasUVGenerationFlags(u32 flags) const
{
	return (m_UVGenerationFlags & flags) == flags;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescFlags::NeedSort() const
{
	return m_BlendMode == BlendMode::AlphaBlend || m_BlendMode == BlendMode::PremultipliedAlpha;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescFlags::operator == (const CParticleMaterialDescFlags &oth) const
{
	return	m_ShaderVariationFlags == oth.m_ShaderVariationFlags &&
			m_BlendMode == oth.m_BlendMode &&
			m_UVGenerationFlags == oth.m_UVGenerationFlags &&
			m_DrawOrder == oth.m_DrawOrder;
}

//----------------------------------------------------------------------------

u32	CParticleMaterialDescFlags::CombineFlags()
{
	const u32	maxBlendModeBits = IntegerTools::Log2((u32)BlendMode::BlendMode_Count);
	return (m_ShaderVariationFlags << maxBlendModeBits) | m_BlendMode;
}

//----------------------------------------------------------------------------

CParticleMaterialDescBillboard::CParticleMaterialDescBillboard()
:	m_InvSoftnessDistance(1)
,	m_RibbonAlignment(0)
//Feature Lit
,	m_NormalMap(CStringId::Null)
,	m_RoughMetalMap(CStringId::Null)
,	m_CastShadows(false)
,	m_NormalBendingFactor(0.5f)
,	m_Roughness(1.0f)
,	m_Metalness(0.0f)
,	m_DiffuseRampMap(CStringId::Null)
{
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescBillboard::InitFromRenderer(const CRendererDataBase &renderer)
{
	PK_ASSERT(renderer.m_RendererType == Renderer_Billboard || renderer.m_RendererType == Renderer_Ribbon || renderer.m_RendererType == Renderer_Triangle);

	//-----------------------------
	// Choose the shader variation:
	//-----------------------------

	// For billboards and ribbons:
	const SRendererFeaturePropertyValue	*diffuse = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse());
	const SRendererFeaturePropertyValue	*diffuseMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse_DiffuseMap());

	const SRendererFeaturePropertyValue	*size2 = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_EnableSize2D());

	const CGuid							diffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_Color());

	const SRendererFeaturePropertyValue	*atlas = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas());
	const SRendererFeaturePropertyValue	*atlasBlending = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Blending());
	const SRendererFeaturePropertyValue	*alphaRemap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaRemap());
	const SRendererFeaturePropertyValue	*alphaRemapAlphaMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaRemap_AlphaMap());
	const CGuid							alphaRemapCursor = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_AlphaRemap_Cursor());

	const SRendererFeaturePropertyValue	*lit = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit());
	const SRendererFeaturePropertyValue	*litLegacy = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLit());
	const SRendererFeaturePropertyValue	*softParticles = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_SoftParticles());
	const SRendererFeaturePropertyValue	*softnessDistance = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_SoftParticles_SoftnessDistance());
	const SRendererFeaturePropertyValue	*distortion = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Distortion());
	const SRendererFeaturePropertyValue	*distortionMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Distortion_DistortionMap());

	const SRendererFeaturePropertyValue	*diffuseRamp = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_DiffuseRamp());
	const SRendererFeaturePropertyValue	*diffuseRampMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_DiffuseRamp_RampMap());

	// For ribbons only:
	const SRendererFeaturePropertyValue	*correctDeformation = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_CorrectDeformation());

	m_Flags.m_ShaderVariationFlags = 0;
	if (diffuse != null && diffuse->ValueB() && diffuseMap != null && !diffuseMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseMap;
	if (size2 != null && size2->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Size2;
	if (atlas != null && atlas->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Atlas;
	if (atlasBlending != null && atlasBlending->ValueI().x() == 1)
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AnimBlend;
	if (alphaRemap != null && alphaRemap->ValueB() && alphaRemapAlphaMap != null && !alphaRemapAlphaMap->ValuePath().Empty() && alphaRemapCursor.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AlphaRemap;
	if ((lit != null && lit->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Lighting;
	if ((litLegacy != null && litLegacy->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_LightingLegacy;
	if (softParticles != null && softParticles->ValueB() && softnessDistance != null && softnessDistance->ValueF().x() != 0.0f)
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Soft;
	if (distortion != null && distortion->ValueB() && distortionMap != null && !distortionMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DistortionMap;
	if (diffuseColorInput.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Color;
	if (diffuseRamp != null && diffuseRamp->ValueB() && diffuseRampMap != null && !diffuseRampMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseRamp;
	if (correctDeformation != null && correctDeformation->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_CorrectDeformation;

	//-----------------------------
	// Choose the blending mode:
	//-----------------------------

	const SRendererFeaturePropertyValue	*transparent = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent());
	const SRendererFeaturePropertyValue	*transparentType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_Type());
	const SRendererFeaturePropertyValue	*opaque = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque());
	const SRendererFeaturePropertyValue	*opaqueType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_Type());

	// Default blend mode is solid:
	m_Flags.m_BlendMode = BlendMode::Solid;
	// Default draw order zero:
	m_Flags.m_DrawOrder = 0;

	if (transparent != null && transparent->ValueB() && transparentType != null)
	{
		const SRendererFeaturePropertyValue    *globalSortOverride = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_GlobalSortOverride());
		// We handle the draw order only for the semi-transparent particles

		if (renderer.m_RendererType == Renderer_Billboard)
		{
			m_Flags.m_DrawOrder = globalSortOverride != null ? globalSortOverride->ValueI().x() : 0;
		}
		else if (renderer.m_RendererType == Renderer_Ribbon)
		{
			m_Flags.m_DrawOrder = globalSortOverride != null ? globalSortOverride->ValueI().x() : 0;
		}
		else if (renderer.m_RendererType == Renderer_Triangle)
		{
			m_Flags.m_DrawOrder = globalSortOverride != null ? globalSortOverride->ValueI().x() : 0;
		}
		if (transparentType->ValueI().x() == BasicRendererProperties::Additive)
		{
			m_Flags.m_BlendMode = BlendMode::Additive;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::AdditiveNoAlpha)
		{
			m_Flags.m_BlendMode = BlendMode::AdditiveNoAlpha;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::AlphaBlend)
		{
			m_Flags.m_BlendMode = BlendMode::AlphaBlend;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::PremultipliedAlpha)
		{
			m_Flags.m_BlendMode = BlendMode::PremultipliedAlpha;
		}
	}
	else if (m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DistortionMap))
	{
		m_Flags.m_BlendMode = BlendMode::Additive;
	}
	else if (opaque != null && opaque->ValueB() && opaqueType != null)
	{
		if (opaqueType->ValueI().x() == BasicRendererProperties::Solid)
		{
			m_Flags.m_BlendMode = BlendMode::Solid;
		}
		else if (opaqueType->ValueI().x() == BasicRendererProperties::Masked)
		{
			m_Flags.m_BlendMode = BlendMode::Masked;
		}
	}

	//-----------------------------
	// Retrieve the shader uniforms (texture path, property values...):
	//-----------------------------

	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DiffuseMap) != 0)
		m_DiffuseMap = CStringId(diffuseMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
		m_AlphaMap = CStringId(alphaRemapAlphaMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DistortionMap) != 0)
		m_DiffuseMap = CStringId(distortionMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Soft) != 0)
		m_InvSoftnessDistance = 1.0f / softnessDistance->ValueF().x();
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DiffuseRamp) != 0)
		m_DiffuseRampMap = CStringId(diffuseRampMap->ValuePath());

	//-----------------------------
	// Get the UV generation flags:
	//-----------------------------
	const SRendererFeaturePropertyValue	*texUV = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TextureUVs());
	const SRendererFeaturePropertyValue	*flipU = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TextureUVs_FlipU());
	const SRendererFeaturePropertyValue	*flipV = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TextureUVs_FlipV());
	const SRendererFeaturePropertyValue	*flipUV = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_FlipUVs());
	const SRendererFeaturePropertyValue	*rotateTexture = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TextureUVs_RotateTexture());

	m_Flags.m_UVGenerationFlags = 0;

	const bool	isTexUVActivated = texUV != null ? texUV->ValueB() : false;
	if (isTexUVActivated)
	{
		PK_ASSERT(renderer.m_RendererType == Renderer_Ribbon);
		m_Flags.m_UVGenerationFlags |= (flipU != null && flipU->ValueB()) ? UVGeneration::FlipU : 0;
		m_Flags.m_UVGenerationFlags |= (flipV != null && flipV->ValueB()) ? UVGeneration::FlipV : 0;
		m_Flags.m_UVGenerationFlags |= (rotateTexture != null && rotateTexture->ValueB()) ? UVGeneration::RotateUV : 0;
	}

	const bool	isUVFlipped = flipUV != null ? flipUV->ValueB() : false;
	if (isUVFlipped)
	{
		PK_ASSERT(renderer.m_RendererType == Renderer_Billboard);
		m_Flags.m_UVGenerationFlags |= UVGeneration::FlipU;
		m_Flags.m_UVGenerationFlags |= UVGeneration::FlipV;
	}

	// ----------------------------
	// Lighting Features
	// ----------------------------
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		const SRendererFeaturePropertyValue	*normalMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_NormalMap());
		//Wait for roughMetalMap channel configuration in PopcornFX Editor
		const SRendererFeaturePropertyValue	*roughMetalMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_RoughMetalMap());
		const SRendererFeaturePropertyValue	*normalBend = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_NormalBend());
		const SRendererFeaturePropertyValue	*normalBendingFactor = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_NormalBend_NormalBendingFactor());
		const SRendererFeaturePropertyValue	*castShadows = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_CastShadows());
		const SRendererFeaturePropertyValue	*roughness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_Roughness());
		const SRendererFeaturePropertyValue	*metalness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_Metalness());

		if (normalMap != null && !normalMap->ValuePath().Empty())
			m_NormalMap = CStringId(normalMap->ValuePath());
		if (roughMetalMap != null && !roughMetalMap->ValuePath().Empty())
			m_RoughMetalMap = CStringId(roughMetalMap->ValuePath());
		if (castShadows != null)
		{
			m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_CastShadow;
			m_CastShadows = castShadows->ValueB();
		}
		if (normalBend != null && normalBend->ValueB() && normalBendingFactor != null)
			m_NormalBendingFactor = normalBendingFactor->ValueF().x();
		else
			m_NormalBendingFactor = 0.0f;
		if (roughness != null)
			m_Roughness = roughness->ValueF().x();
		if (metalness != null)
			m_Metalness = metalness->ValueF().x();
	}
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_LightingLegacy) != 0)
	{
		//SID_LegacyLit()							OK
		//SID_LegacyLit_NormalMap()					OK
		//SID_LegacyLit_NormalBendingFactor()		--
		//SID_LegacyLit_NormalWrapFactor()			--
		//SID_LegacyLit_LightExponent()				--
		//SID_LegacyLit_LightScale()				--
		//SID_LegacyLit_AmbientLight()				--
		//SID_LegacyLit_CastShadows()				--
		//SID_LegacyLit_SpecularMap()				--
		//const SRendererFeaturePropertyValue	*normalMapLegacy = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLit_NormalMap());
		//if (m_NormalMap == CStringId::Null && normalMapLegacy != null && !normalMapLegacy->ValuePath().Empty())
		//{
		//	m_NormalMap = CStringId(normalMapLegacy->ValuePath());
		//}
		CLog::Log(PK_WARN, "Legacy Lit render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescBillboard::operator == (const CParticleMaterialDescBillboard &oth) const
{
	return	m_Flags == oth.m_Flags &&
			m_DiffuseMap == oth.m_DiffuseMap &&
			m_AlphaMap == oth.m_AlphaMap &&
			m_InvSoftnessDistance == oth.m_InvSoftnessDistance &&
			m_NormalMap == oth.m_NormalMap &&
			m_RoughMetalMap == oth.m_RoughMetalMap &&
			m_CastShadows == oth.m_CastShadows &&
			m_NormalBendingFactor == oth.m_NormalBendingFactor &&
			m_Roughness == oth.m_Roughness &&
			m_Metalness == oth.m_Metalness &&
			m_DiffuseRampMap == oth.m_DiffuseRampMap;
}

//----------------------------------------------------------------------------

CParticleMaterialDescMesh::CParticleMaterialDescMesh()
:	m_MeshPath(CStringId::Null)
,	m_DiffuseMap(CStringId::Null)
,	m_HasMeshAtlas(false)
//Feature Lit
,	m_NormalMap(CStringId::Null)
,	m_RoughMetalMap(CStringId::Null)
,	m_CastShadows(false)
,	m_NormalBendingFactor(0.5f)
,	m_Roughness(1.0f)
,	m_Metalness(0.0f)
{
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescMesh::InitFromRenderer(const CRendererDataMesh &renderer)
{
	//-----------------------------
	// Choose the shader variation:
	//-----------------------------

	// For billboards and ribbons:
	const SRendererFeaturePropertyValue	*diffuse = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse());
	const SRendererFeaturePropertyValue	*diffuseMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse_DiffuseMap());

	const CGuid							diffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_Color());
	const SRendererFeaturePropertyValue	*lit = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit());
	const SRendererFeaturePropertyValue	*litLegacy = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLit());

	m_Flags.m_ShaderVariationFlags = 0;
	if (diffuse != null && diffuse->ValueB() && diffuseMap != null && !diffuseMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseMap;
	if ((lit != null && lit->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Lighting;
	if ((litLegacy != null && litLegacy->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_LightingLegacy;

	if (diffuseColorInput.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Color;

	//-----------------------------
	// Choose the blending mode:
	//-----------------------------

	const SRendererFeaturePropertyValue	*transparent = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent());
	const SRendererFeaturePropertyValue	*transparentType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_Type());
	const SRendererFeaturePropertyValue	*opaque = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque());
	const SRendererFeaturePropertyValue	*opaqueType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_Type());

	// Default blend mode is solid:
	m_Flags.m_BlendMode = BlendMode::Solid;

	if (transparent != null && transparent->ValueB() && transparentType != null)
	{
		if (transparentType->ValueI().x() == BasicRendererProperties::Additive)
		{
			m_Flags.m_BlendMode = BlendMode::Additive;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::AdditiveNoAlpha)
		{
			m_Flags.m_BlendMode = BlendMode::AdditiveNoAlpha;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::AlphaBlend)
		{
			m_Flags.m_BlendMode = BlendMode::AlphaBlend;
		}
		else if (transparentType->ValueI().x() == BasicRendererProperties::PremultipliedAlpha)
		{
			m_Flags.m_BlendMode = BlendMode::PremultipliedAlpha;
		}
	}
	else if (opaque != null && opaque->ValueB() && opaqueType != null)
	{
		if (opaqueType->ValueI().x() == BasicRendererProperties::Solid)
		{
			m_Flags.m_BlendMode = BlendMode::Solid;
		}
		else if (opaqueType->ValueI().x() == BasicRendererProperties::Masked)
		{
			m_Flags.m_BlendMode = BlendMode::Masked;
		}
	}

	//-----------------------------
	// Retrieve the shader uniforms (mesh resource, property values...):
	//-----------------------------
	const SRendererFeaturePropertyValue	*mesh = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Mesh());
	const SRendererFeaturePropertyValue	*meshAtlas = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_MeshAtlas());

	if (PK_VERIFY(mesh != null && !mesh->ValuePath().Empty()))
		m_MeshPath = CStringId(mesh->ValuePath());
	else
		return false;

	const bool hasMeshAtlas = (meshAtlas != null) ? meshAtlas->ValueB() : false;
	if (hasMeshAtlas)
		m_HasMeshAtlas = true;
	else
		m_HasMeshAtlas = false;
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DiffuseMap) != 0)
		m_DiffuseMap = CStringId(diffuseMap->ValuePath());

	// ----------------------------
	// Lighting Features
	// ----------------------------
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		const SRendererFeaturePropertyValue	*normalMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_NormalMap());
		//Wait for roughMetalMap channel configuration in PopcornFX Editor
		const SRendererFeaturePropertyValue	*roughMetalMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_RoughMetalMap());
		const SRendererFeaturePropertyValue	*normalBend = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_NormalBend());
		const SRendererFeaturePropertyValue	*normalBendingFactor = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_NormalBend_NormalBendingFactor());
		const SRendererFeaturePropertyValue	*castShadows = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_CastShadows());
		const SRendererFeaturePropertyValue	*roughness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_Roughness());
		const SRendererFeaturePropertyValue	*metalness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit_Metalness());

		if (normalMap != null && !normalMap->ValuePath().Empty())
			m_NormalMap = CStringId(normalMap->ValuePath());
		if (roughMetalMap != null && !roughMetalMap->ValuePath().Empty())
			m_RoughMetalMap = CStringId(roughMetalMap->ValuePath());
		if (castShadows != null)
			m_CastShadows = castShadows->ValueB();
		if (normalBend != null && normalBend->ValueB() && normalBendingFactor != null)
			m_NormalBendingFactor = normalBendingFactor->ValueF().x();
		else
			m_NormalBendingFactor = 0.0f;
		if (roughness != null)
			m_Roughness = roughness->ValueF().x();
		if (metalness != null)
			m_Metalness = metalness->ValueF().x();
	}
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_LightingLegacy) != 0)
	{
		//SID_LegacyLit()							OK
		//SID_LegacyLit_NormalMap()					OK
		//SID_LegacyLit_NormalBendingFactor()		--
		//SID_LegacyLit_NormalWrapFactor()			--
		//SID_LegacyLit_LightExponent()				--
		//SID_LegacyLit_LightScale()				--
		//SID_LegacyLit_AmbientLight()				--
		//SID_LegacyLit_CastShadows()				--
		//SID_LegacyLit_SpecularMap()				--
		//const SRendererFeaturePropertyValue	*normalMapLegacy = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLit_NormalMap());
		//if (m_NormalMap == CStringId::Null && normalMapLegacy != null && !normalMapLegacy->ValuePath().Empty())
		//{
		//	m_NormalMap = CStringId(normalMapLegacy->ValuePath());
		//}
		CLog::Log(PK_WARN, "Legacy Lit render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescMesh::operator == (const CParticleMaterialDescMesh &oth) const
{
	PK_TODO("Correct operator == for meshes rdr");
	(void)oth;
	return false;
	//return	m_Flags == oth.m_Flags &&
	//		m_DiffuseMap == oth.m_DiffuseMap &&
	//		m_HasMeshAtlas == oth.m_HasMeshAtlas;
}

//----------------------------------------------------------------------------

CUnityRendererCache::~CUnityRendererCache()
{
}

//----------------------------------------------------------------------------

void	CUnityRendererCache::UpdateThread_BuildBillboardingFlags(const PRendererDataBase &rendererData)
{
	(void)rendererData;
	m_Flags.m_NeedSort = m_MaterialDescBillboard.m_Flags.NeedSort();
	m_Flags.m_HasUV =	m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DiffuseMap) ||
						m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DistortionMap);
	m_Flags.m_FlipU = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::FlipU);
	m_Flags.m_FlipV = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::FlipV);
	m_Flags.m_RotateTexture = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::RotateUV);
	m_Flags.m_HasAtlasBlending = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_AnimBlend);
	m_Flags.m_HasRibbonCorrectDeformation = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_CorrectDeformation);
	m_Flags.m_HasNormal = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Lighting);
	m_Flags.m_HasTangent = false; // m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Lighting);
}

//----------------------------------------------------------------------------

bool	CUnityRendererCache::operator==(const CUnityRendererCache &oth) const
{
	if (m_RendererType != oth.m_RendererType)
		return false;

	if (m_HasCustomMat != oth.m_HasCustomMat)
		return false;
	if (m_HasCustomMat && m_CustomMatID != oth.m_CustomMatID)
		return false;
	CRuntimeManager	&pkfxManager = CRuntimeManager::Instance();
	if (m_RendererType == Renderer_Billboard)
	{
		if (pkfxManager.m_PopcornFXRuntimeData->m_GPUBillboarding)
		{
			return	m_MaterialDescBillboard == oth.m_MaterialDescBillboard &&
					Drawers::DefaultBillboardingRequestCompatibilityCheck(m_BillboardBR, oth.m_BillboardBR, Drawers::BillboardingLocation_VertexShader); // We need to break the batching exactly like the draw requests
		}
		else
		{
			return	m_MaterialDescBillboard == oth.m_MaterialDescBillboard &&
					Drawers::DefaultBillboardingRequestCompatibilityCheck(m_BillboardBR, oth.m_BillboardBR, Drawers::BillboardingLocation_CPU); // We need to break the batching exactly like the draw requests
		}
	}
	if (m_RendererType == Renderer_Ribbon)
	{
		return	m_MaterialDescBillboard == oth.m_MaterialDescBillboard &&
				Drawers::DefaultBillboardingRequestCompatibilityCheck(m_RibbonBR, oth.m_RibbonBR, Drawers::BillboardingLocation_CPU); // We need to break the batching exactly like the draw requests
	}
	if (m_RendererType == Renderer_Mesh)
	{
		return	m_MaterialDescMesh == oth.m_MaterialDescMesh &&
				Drawers::DefaultBillboardingRequestCompatibilityCheck(m_MeshBR, oth.m_MeshBR, Drawers::BillboardingLocation_CPU); // We need to break the batching exactly like the draw requests
	}
	if (m_RendererType == Renderer_Triangle)
	{
		return	m_MaterialDescBillboard == oth.m_MaterialDescBillboard &&
				Drawers::DefaultBillboardingRequestCompatibilityCheck(m_TriangleBR, oth.m_TriangleBR, Drawers::BillboardingLocation_CPU); // We need to break the batching exactly like the draw requests
	}
	return false;
}

void	CUnityRendererCache::SetAssetName(const CString &assetName)
{
	m_AssetName = assetName;
}

const CString & CUnityRendererCache::GetAssetName() const
{
	return m_AssetName;
}

void	CUnityRendererCache::GetRendererLitFeatureInfo(SRenderingFeatureLitDesc **litRenderingdesc)
{
	(void)litRenderingdesc;
	PK_TODO("Merge m_MaterialDescBillboard and m_MaterialDescMesh");
	//if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting != 0))
	//{
	//	*litRenderingdesc = PK_NEW(SRenderingFeatureLitDesc);
	//	PK_ASSERT(*litRenderingdesc != null);
	//
	//	(*litRenderingdesc)->m_NormalMap = m_MaterialDescMesh.m_NormalMap.ToStringData();
	//	(*litRenderingdesc)->m_RoughMetalMap = m_MaterialDescMesh.m_RoughMetalMap.ToStringData();
	//	(*litRenderingdesc)->m_CastShadows = m_MaterialDescMesh.m_CastShadows ? ManagedBool_True : ManagedBool_False;
	//	(*litRenderingdesc)->m_NormalBendingFactor = m_MaterialDescMesh.m_NormalBendingFactor;
	//	(*litRenderingdesc)->m_Roughness = m_MaterialDescMesh.m_Roughness;
	//	(*litRenderingdesc)->m_Metalness = m_MaterialDescMesh.m_Metalness;
	//}
	//else
	//	*litRenderingdesc = null;
}

bool	CUnityRendererCache::GetRendererInfo(SPopcornRendererDesc &desc)
{
	desc.m_ShaderVariationFlags = m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags;
	desc.m_BlendMode = m_MaterialDescBillboard.m_Flags.m_BlendMode;
	desc.m_RotateUvs = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::RotateUV) &&
		m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_CorrectDeformation) ? ManagedBool_True : ManagedBool_False;
	desc.m_DiffuseMap = m_MaterialDescBillboard.m_DiffuseMap.ToStringData();
	desc.m_AlphaRemap = m_MaterialDescBillboard.m_AlphaMap.ToStringData();
	desc.m_DiffuseRampMap = m_MaterialDescBillboard.m_DiffuseRampMap.ToStringData();
	desc.m_InvSoftnessDistance = m_MaterialDescBillboard.m_InvSoftnessDistance;
	desc.m_BillboardMode = m_RendererType == ERendererClass::Renderer_Billboard ? m_BillboardBR.m_Mode : 0;
	desc.m_DrawOrder = m_MaterialDescBillboard.m_Flags.m_DrawOrder;

	if ((m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		desc.m_LitRendering = PK_NEW(SRenderingFeatureLitDesc);
		PK_ASSERT(desc.m_LitRendering != null);

		desc.m_LitRendering->m_NormalMap = m_MaterialDescBillboard.m_NormalMap.ToStringData();
		desc.m_LitRendering->m_RoughMetalMap = m_MaterialDescBillboard.m_RoughMetalMap.ToStringData();
		desc.m_LitRendering->m_CastShadows = m_MaterialDescBillboard.m_CastShadows ? ManagedBool_True : ManagedBool_False;
		desc.m_LitRendering->m_NormalBendingFactor = m_MaterialDescBillboard.m_NormalBendingFactor;
		desc.m_LitRendering->m_Roughness = m_MaterialDescBillboard.m_Roughness;
		desc.m_LitRendering->m_Metalness = m_MaterialDescBillboard.m_Metalness;
	}
	else
		desc.m_LitRendering = null;
	//GetRendererLitFeatureInfo(&desc.m_LitRendering);
	return true;
}

bool	CUnityRendererCache::GetRendererInfo(SMeshRendererDesc &desc)
{
	desc.m_MeshAsset = m_MaterialDescMesh.m_MeshPath.ToStringData();
	if (desc.m_MeshAsset == null)
		return false;
	desc.m_ShaderVariationFlags = m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags;
	desc.m_BlendMode = m_MaterialDescMesh.m_Flags.m_BlendMode;
	desc.m_HasMeshAtlas = m_MaterialDescMesh.m_HasMeshAtlas ? ManagedBool_True : ManagedBool_False;
	desc.m_DiffuseMap = m_MaterialDescMesh.m_DiffuseMap.ToStringData();

	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		desc.m_LitRendering = PK_NEW(SRenderingFeatureLitDesc);
		PK_ASSERT(desc.m_LitRendering != null);

		desc.m_LitRendering->m_NormalMap = m_MaterialDescMesh.m_NormalMap.ToStringData();
		desc.m_LitRendering->m_RoughMetalMap = m_MaterialDescMesh.m_RoughMetalMap.ToStringData();
		desc.m_LitRendering->m_CastShadows = m_MaterialDescMesh.m_CastShadows ? ManagedBool_True : ManagedBool_False;
		desc.m_LitRendering->m_NormalBendingFactor = m_MaterialDescMesh.m_NormalBendingFactor;
		desc.m_LitRendering->m_Roughness = m_MaterialDescMesh.m_Roughness;
		desc.m_LitRendering->m_Metalness = m_MaterialDescMesh.m_Metalness;
	}
	else
		desc.m_LitRendering = null;
	return true;
	//GetRendererLitFeatureInfo(&desc.m_LitRendering);
}

void		CUnityRendererCache::CreateUnityMesh(u32 idx, bool gpuBillboarding)
{
	if (m_RendererType == Renderer_Billboard || m_RendererType == Renderer_Ribbon || m_RendererType == Renderer_Triangle)
	{
		// Renderer info in case we need to update the renderer's buffers:
		SRetrieveRendererInfo	rendererInfo;
		ManagedBool				hasCustomMat = ManagedBool_False;
		ManagedBool				useComputeBuffers = ManagedBool_False;

		rendererInfo.m_VBHandler = &m_UnityMeshInfo.m_VBHandler.m_Buffer->m_DeviceLocal;
		rendererInfo.m_IBHandler = &m_UnityMeshInfo.m_IBHandler.m_Buffer->m_DeviceLocal;
		rendererInfo.m_VertexBufferSize = &m_UnityMeshInfo.m_VBElemCount;
		rendererInfo.m_IndexBufferSize = &m_UnityMeshInfo.m_IBElemCount;
		rendererInfo.m_IsIndex32 = &m_UnityMeshInfo.m_LargeIndices;

		rendererInfo.m_HasCustomMaterial = &hasCustomMat;
		rendererInfo.m_CustomMatID = &m_CustomMatID;

		rendererInfo.m_InfoBSize = &m_UnityMeshInfo.m_InfoSize;
		rendererInfo.m_InfoBHandler = &m_UnityMeshInfo.m_InfoHandler.m_Buffer->m_DeviceLocal;
		rendererInfo.m_AtlasesBSize = &m_UnityMeshInfo.m_AtlasesSize;
		rendererInfo.m_AtlasesBHandler = &m_UnityMeshInfo.m_AtlasesHandler.m_Buffer->m_DeviceLocal;

		rendererInfo.m_IndirectArgsBHandler = &m_UnityMeshInfo.m_IndirectArgsHandler.m_Buffer->m_DeviceLocal;
		rendererInfo.m_IndirectArgsParticleCountMultiplier = &m_UnityMeshInfo.m_IndirectArgsParticleCountMultiplier;

		rendererInfo.m_UseComputeBuffers = &useComputeBuffers;

		// We alloc the Unity mesh here:
		SPopcornRendererDesc	desc;
		GetRendererInfo(desc);

		if (m_RendererType == Renderer_Billboard)
		{
			m_UnityMeshInfo.m_RendererGUID = ::OnSetupNewBillboardRenderer(&desc, idx);
			if (m_UnityMeshInfo.m_RendererGUID < 0)
				return;
			::OnResizeRenderer(m_UnityMeshInfo.m_RendererGUID, 0, 0x100, 0x300);
			::OnRetrieveRendererBufferInfo(m_UnityMeshInfo.m_RendererGUID, &rendererInfo);
		}
		else if (m_RendererType == Renderer_Ribbon)
		{
			m_UnityMeshInfo.m_RendererGUID = ::OnSetupNewRibbonRenderer(&desc, idx);
			if (m_UnityMeshInfo.m_RendererGUID < 0)
				return;
			::OnResizeRenderer(m_UnityMeshInfo.m_RendererGUID, 0, 0x100, 0x300);
			::OnRetrieveRendererBufferInfo(m_UnityMeshInfo.m_RendererGUID, &rendererInfo);
		}
		else if (m_RendererType == Renderer_Triangle)
		{
			m_UnityMeshInfo.m_RendererGUID = ::OnSetupNewTriangleRenderer(&desc, idx);
			if (m_UnityMeshInfo.m_RendererGUID < 0)
				return;
			::OnResizeRenderer(m_UnityMeshInfo.m_RendererGUID, 0, 0x100, 0x300);
			::OnRetrieveRendererBufferInfo(m_UnityMeshInfo.m_RendererGUID, &rendererInfo);
		}

		if (gpuBillboarding && m_RendererType == Renderer_Billboard)
		{
			m_UnityMeshInfo.m_VertexStride = GetGeomBillboardVertexBufferStride(m_BillboardBR.m_Mode, m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags);
		}
		else
		{
			m_UnityMeshInfo.m_VertexStride = FillOffsetTableAndGetVertexBufferStride(m_UnityMeshInfo.m_SemanticOffsets, m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags);
		}
		m_HasCustomMat = hasCustomMat == ManagedBool_True ? true : false;

		// Set the info if the handlers are compute buffers or not:
#if		defined(PK_ORBIS)

		((CGNMBufferHandles*)m_UnityMeshInfo.m_VBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CGNMBufferHandles*)m_UnityMeshInfo.m_IBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CGNMBufferHandles*)m_UnityMeshInfo.m_InfoHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CGNMBufferHandles*)m_UnityMeshInfo.m_AtlasesHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CGNMBufferHandles*)m_UnityMeshInfo.m_IndirectArgsHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
#elif	defined(PK_UNKNOWN2)

		((CUNKNOWN2BufferHandles*)m_UnityMeshInfo.m_VBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CUNKNOWN2BufferHandles*)m_UnityMeshInfo.m_IBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CUNKNOWN2BufferHandles*)m_UnityMeshInfo.m_InfoHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CUNKNOWN2BufferHandles*)m_UnityMeshInfo.m_AtlasesHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
		((CUNKNOWN2BufferHandles*)m_UnityMeshInfo.m_IndirectArgsHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
#endif	// defined(PK_ORBIS)
	}
	else if (m_RendererType == Renderer_Mesh)
	{
		// Renderer info in case we need to update the renderer's buffers:
		SRetrieveRendererInfo	rendererInfo;
		ManagedBool				hasCustomMat;

		rendererInfo.m_VBHandler = null;
		rendererInfo.m_IBHandler = null;
		rendererInfo.m_VertexBufferSize = null;
		rendererInfo.m_IndexBufferSize = null;
		rendererInfo.m_IsIndex32 = null;

		rendererInfo.m_HasCustomMaterial = &hasCustomMat;
		rendererInfo.m_CustomMatID = &m_CustomMatID;

		rendererInfo.m_InfoBSize = null;
		rendererInfo.m_InfoBHandler = null;
		rendererInfo.m_AtlasesBSize = null;
		rendererInfo.m_AtlasesBHandler = null;

		rendererInfo.m_IndirectArgsBHandler = null;
		rendererInfo.m_IndirectArgsParticleCountMultiplier = null;

		rendererInfo.m_UseComputeBuffers = null;

		SMeshRendererDesc		desc;
		if (!GetRendererInfo(desc))
			return;

 		m_UnityMeshInfo.m_RendererGUID = ::OnSetupNewMeshRenderer(&desc, idx);
		if (m_UnityMeshInfo.m_RendererGUID < 0)
			return;
		m_MeshCount = ::OnGetMeshCount(m_UnityMeshInfo.m_RendererGUID);
		PK_ASSERT(m_MeshCount > 0);
		m_GlobalMeshBound.Degenerate();
		for (u32 i = 0; i < m_MeshCount; ++i)
		{
			CFloat3 bb;

			::OnGetMeshBounds(m_UnityMeshInfo.m_RendererGUID, i, &bb);
			m_SubMeshBounds.PushBack(bb);
			m_GlobalMeshBound.Add(bb);
		}
		::OnRetrieveRendererBufferInfo(m_UnityMeshInfo.m_RendererGUID, &rendererInfo);
		m_HasCustomMat = hasCustomMat == ManagedBool_True ? true : false;
	}
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataBillboard>(const CRendererDataBillboard *renderer)
{
	m_RendererType = renderer->m_RendererType;
	if (!PK_VERIFY(m_MaterialDescBillboard.InitFromRenderer(*renderer)) ||
		!PK_VERIFY(m_BillboardBR.Setup(renderer, null, SParticleStreamDef())))
		return false;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataRibbon>(const CRendererDataRibbon *renderer)
{
	m_RendererType = renderer->m_RendererType;

	if (!PK_VERIFY(m_MaterialDescBillboard.InitFromRenderer(*renderer)) ||
		!PK_VERIFY(m_RibbonBR.Setup(renderer, null, SParticleStreamDef())))
		return false;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataMesh>(const CRendererDataMesh *renderer)
{
	m_RendererType = renderer->m_RendererType;

	if (!PK_VERIFY(m_MaterialDescMesh.InitFromRenderer(*renderer)) ||
		!PK_VERIFY(m_MeshBR.Setup(renderer, null, SParticleStreamDef())))
		return false;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataTriangle>(const CRendererDataTriangle *renderer)
{
	m_RendererType = renderer->m_RendererType;

	if (!PK_VERIFY(m_MaterialDescBillboard.InitFromRenderer(*renderer)) ||
		!PK_VERIFY(m_TriangleBR.Setup(renderer, null, SParticleStreamDef())))
		return false;
	return true;
}

//----------------------------------------------------------------------------
