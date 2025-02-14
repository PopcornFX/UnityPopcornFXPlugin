//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityRendererCache.h"
#include "UnityRenderDataFactory.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_static.h>

#include "NativeToManaged.h"
#include "RuntimeManager.h"
#include <pk_render_helpers/include/render_features/rh_features_vat_skeletal.h>

#include <pk_kernel/include/kr_resources.h>
#include <pk_geometrics/include/ge_mesh_resource.h>

//----------------------------------------------------------------------------

CParticleMaterialDescFlags::CParticleMaterialDescFlags()
:	m_ShaderVariationFlags(0)
,	m_BlendMode(BlendMode::Solid)
,	m_UVGenerationFlags(0)
,	m_DrawOrder(0)
,	m_IsLegacy(false)
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
,	m_AlphaThreshold(0.5f)
,	m_AtlasSubdivX(0)
,	m_AtlasSubdivY(0)
,	m_RibbonAlignment(0)
//Feature Lit
,	m_CastShadows(false)
,	m_NormalBendingFactor(0.5f)
,	m_Roughness(1.0f)
,	m_Metalness(0.0f)
,	m_AlphaMasks1Intensity(0.f)
,	m_AlphaMasks2Intensity(0.f)
,	m_AlphaMasks1Weight(0.f)
,	m_AlphaMasks2Weight(0.f)
,	m_AlphaMasks1Scale(0.f)
,	m_AlphaMasks2Scale(0.f)
,	m_AlphaMasks1RotationSpeed(0.f)
,	m_AlphaMasks2RotationSpeed(0.f)
,	m_AlphaMasks1TranslationSpeed(0.f, 0.f)
,	m_AlphaMasks2TranslationSpeed(0.f, 0.f)
,	m_UVDistortions1Intensity(0.f)
,	m_UVDistortions2Intensity(0.f)
,	m_UVDistortions1Scale(0.f)
,	m_UVDistortions2Scale(0.f)
,	m_UVDistortions1RotationSpeed(0.f)
,	m_UVDistortions2RotationSpeed(0.f)
,	m_UVDistortions1TranslationSpeed(0.f)
,	m_UVDistortions2TranslationSpeed(0.f)
,	m_DissolveSoftness(0.f)
,	m_TransformUVs_RGBOnly(false)
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

	const CGuid							legacyDiffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_Color());//a
	const CGuid							diffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_DiffuseColor());
	
	// Basic Transform feature (flips U and V independently):
	const SRendererFeaturePropertyValue	*transformUVs = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TransformUVs());
	const SRendererFeaturePropertyValue	*transformUVsRGBOnly = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TransformUVs_RGBOnly());
	const SRendererFeaturePropertyValue	*basicTransformUVs = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_BasicTransformUVs());
	const SRendererFeaturePropertyValue	*basicTransformUVsFlipU = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_BasicTransformUVs_FlipU());
	const SRendererFeaturePropertyValue	*basicTransformUVsFlipV = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_BasicTransformUVs_FlipV());
	const SRendererFeaturePropertyValue	*basicTransformUVsRotateUV = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_BasicTransformUVs_RotateUV());

	const SRendererFeaturePropertyValue	*atlas = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas());
	const SRendererFeaturePropertyValue	*atlasBlending = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Blending());
	const SRendererFeaturePropertyValue	*atlasSource = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Source());
	const SRendererFeaturePropertyValue	*atlasDefinition = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Definition());
	const SRendererFeaturePropertyValue	*atlasSubDiv = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_SubDiv());

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

	const SRendererFeaturePropertyValue *emissive = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive());
	const CGuid							emissiveColor = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Emissive_EmissiveColor());
	const SRendererFeaturePropertyValue *emissiveMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive_EmissiveMap());

	const SRendererFeaturePropertyValue *emissiveRamp = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_EmissiveRamp());
	const SRendererFeaturePropertyValue *emissiveRampMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_EmissiveRamp_RampMap());

	// For ribbons only:
	const SRendererFeaturePropertyValue	*correctDeformation = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_CorrectDeformation());

	// Animated Masked
	const SRendererFeaturePropertyValue *alphaMasks = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks());
	const SRendererFeaturePropertyValue *alphaMask1Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Map());
	const SRendererFeaturePropertyValue *alphaMask2Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Map());
	const SRendererFeaturePropertyValue *alphaMask1Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Intensity());
	const SRendererFeaturePropertyValue *alphaMask2Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Intensity());
	const SRendererFeaturePropertyValue *alphaMask1Weight = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Weight());
	const SRendererFeaturePropertyValue *alphaMask2Weight = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Weight());
	const SRendererFeaturePropertyValue *alphaMask1RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1RotationSpeed());
	const SRendererFeaturePropertyValue *alphaMask2RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2RotationSpeed());
	const SRendererFeaturePropertyValue *alphaMask1Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Scale());
	const SRendererFeaturePropertyValue *alphaMask2Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Scale());
	const SRendererFeaturePropertyValue *alphaMask1TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1TranslationSpeed());
	const SRendererFeaturePropertyValue *alphaMask2TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2TranslationSpeed());

	const SRendererFeaturePropertyValue *uvDistortions = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions());
	const SRendererFeaturePropertyValue *uvDistortions1Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Map());
	const SRendererFeaturePropertyValue *uvDistortions2Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Map());
	const SRendererFeaturePropertyValue *uvDistortions1Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Intensity());
	const SRendererFeaturePropertyValue *uvDistortions2Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Intensity());
	const SRendererFeaturePropertyValue *uvDistortions1RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1RotationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions2RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2RotationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions1Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Scale());
	const SRendererFeaturePropertyValue *uvDistortions2Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Scale());
	const SRendererFeaturePropertyValue *uvDistortions1TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1TranslationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions2TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2TranslationSpeed());

	const SRendererFeaturePropertyValue *dissolve = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve());
	const SRendererFeaturePropertyValue *dissolveMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve_DissolveMap());
	const SRendererFeaturePropertyValue *dissolveSoftness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve_DissolveSoftness());

	m_Flags.m_ShaderVariationFlags = 0;
	m_Flags.m_IsLegacy = renderer.m_Declaration.m_MaterialPath.Contains("Library/PopcornFXCore/Materials/Legacy");

	if (diffuse != null && diffuse->ValueB() && diffuseMap != null && !diffuseMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseMap;
	if (size2 != null && size2->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Size2;
	if (transformUVs != null && transformUVs->ValueB())
	{
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_TransformUVs;
		if (transformUVsRGBOnly != null && transformUVsRGBOnly->ValueB())
			m_TransformUVs_RGBOnly = transformUVsRGBOnly->ValueB();
	}
	if (atlas != null && atlas->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Atlas;
	if (atlasSource == null || atlasSource->ValueI().x() == 0)
	{
		if (atlasDefinition != null && atlasDefinition->m_Type == PropertyType_TextureAtlasPath && !atlasDefinition->ValuePath().Empty())
		{
			m_AtlasDefinition = CStringId(atlasDefinition->ValuePath());
		}
	}
	else
	{
		if (atlasSubDiv != null && atlasSubDiv->ValueI().x() > 0 && atlasSubDiv->ValueI().y() > 0)
		{
			m_AtlasSubdivX = atlasSubDiv->ValueI().x();
			m_AtlasSubdivY = atlasSubDiv->ValueI().y();
		}
	}
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
	if (diffuseColorInput.Valid() || legacyDiffuseColorInput.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Color;
	if (diffuseRamp != null && diffuseRamp->ValueB() && diffuseRampMap != null && !diffuseRampMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseRamp;
	if (emissiveRamp != null && emissiveRamp->ValueB() && emissiveRampMap != null && !emissiveRampMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_EmissiveRamp;
	if (correctDeformation != null && correctDeformation->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_CorrectDeformation;
	if (emissive != null && emissive->ValueB() && emissiveColor.Valid() && emissiveMap != null && !emissiveMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Emissive;
	if (alphaMasks != null && alphaMasks->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AlphaMasks;
	if (uvDistortions != null && uvDistortions->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_UVDistortions;
	if (dissolve != null && dissolve->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Dissolve;


	//-----------------------------
	// Choose the blending mode:
	//-----------------------------

	const SRendererFeaturePropertyValue	*transparent = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent());
	const SRendererFeaturePropertyValue	*transparentType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_Type());
	const SRendererFeaturePropertyValue	*opaque = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque());
	const SRendererFeaturePropertyValue	*opaqueType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_Type());
	const SRendererFeaturePropertyValue	*alphaThreshold = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_MaskThreshold());

	// Default blend mode is solid:
	m_Flags.m_BlendMode = BlendMode::Solid;
	// Default draw order zero:
	m_Flags.m_DrawOrder = 0;

	if (transparent != null && transparent->ValueB())
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

		if (transparentType == null)
		{
			m_Flags.m_BlendMode = BlendMode::PremultipliedAlpha;
			if (lit->ValueB())
			{
				m_Flags.m_BlendMode = BlendMode::AlphaBlend;
			}
		}
		else
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
			if (alphaThreshold != null)
			{
				m_AlphaThreshold = alphaThreshold->ValueF().x();
			}
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
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Emissive && emissiveMap != null && !emissiveMap->ValuePath().Empty()) != 0)
		m_EmissiveMap = CStringId(emissiveMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_EmissiveRamp) != 0)
		m_EmissiveRampMap = CStringId(emissiveRampMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks) != 0)
	{
		m_AlphaMasks1Map = CStringId(alphaMask1Map->ValuePath());
		m_AlphaMasks2Map = CStringId(alphaMask2Map->ValuePath());
		m_AlphaMasks1Intensity = alphaMask1Intensity->ValueF().x();
		m_AlphaMasks2Intensity = alphaMask2Intensity->ValueF().x();
		m_AlphaMasks1Weight = alphaMask1Weight->ValueF().x();
		m_AlphaMasks2Weight = alphaMask2Weight->ValueF().x();
		m_AlphaMasks1Scale = alphaMask1Scale->ValueF().xy();
		m_AlphaMasks2Scale = alphaMask2Scale->ValueF().xy();
		m_AlphaMasks1RotationSpeed = alphaMask1RotationSpeed->ValueF().x();
		m_AlphaMasks2RotationSpeed = alphaMask2RotationSpeed->ValueF().x();
		m_AlphaMasks1TranslationSpeed = alphaMask1TranslationSpeed->ValueF().xy();
		m_AlphaMasks2TranslationSpeed = alphaMask2TranslationSpeed->ValueF().xy();	
	}
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_UVDistortions) != 0)
	{
		m_UVDistortions1Map = CStringId(uvDistortions1Map->ValuePath());
		m_UVDistortions2Map = CStringId(uvDistortions2Map->ValuePath());
		m_UVDistortions1Intensity = uvDistortions1Intensity->ValueF().x();
		m_UVDistortions2Intensity = uvDistortions2Intensity->ValueF().x();
		m_UVDistortions1Scale = uvDistortions1Scale->ValueF().xy();
		m_UVDistortions2Scale = uvDistortions2Scale->ValueF().xy();
		m_UVDistortions1RotationSpeed = uvDistortions1RotationSpeed->ValueF().x();
		m_UVDistortions2RotationSpeed = uvDistortions2RotationSpeed->ValueF().x();
		m_UVDistortions1TranslationSpeed = uvDistortions1TranslationSpeed->ValueF().xy();
		m_UVDistortions2TranslationSpeed = uvDistortions2TranslationSpeed->ValueF().xy();	
	}

	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Dissolve) != 0)
	{
		m_DissolveMap = CStringId(dissolveMap->ValuePath());
		m_DissolveSoftness = dissolveSoftness->ValueF().x();
	}

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
		// m_Flags.m_UVGenerationFlags |= UVGeneration::FlipU;
		m_Flags.m_UVGenerationFlags |= UVGeneration::FlipV;
	}

	if (basicTransformUVs != null && basicTransformUVs->ValueB())
	{
		m_Flags.m_UVGenerationFlags |= (basicTransformUVsFlipU != null && basicTransformUVsFlipU->ValueB()) ? UVGeneration::FlipU : 0;
		m_Flags.m_UVGenerationFlags |= (basicTransformUVsFlipV != null && basicTransformUVsFlipV->ValueB()) ? UVGeneration::FlipV : 0;
		m_Flags.m_UVGenerationFlags |= (basicTransformUVsRotateUV != null && basicTransformUVsRotateUV->ValueB()) ? UVGeneration::RotateUV : 0;
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
		//if (m_NormalMap.Empty() && normalMapLegacy != null && !normalMapLegacy->ValuePath().Empty())
		//{
		//	m_NormalMap = CStringId(normalMapLegacy->ValuePath());
		//}
		CLog::Log(PK_WARN, "Legacy Lit render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
	}

	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_LightingLegacyOpaque) != 0)
	{
		CLog::Log(PK_WARN, "Legacy Lit Opaque render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescSound::InitFromRenderer(const CRendererDataSound &renderer)
{
	PK_ASSERT(renderer.m_RendererType == Renderer_Sound);

	const SRendererFeaturePropertyValue *soundData = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Sound_SoundData());
	if (soundData != null && !soundData->ValuePath().Empty())
	{
		m_SoundData = CStringId(soundData->ValuePath());
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescDecal::InitFromRenderer(const CRendererDataDecal &renderer)
{
	PK_ASSERT(renderer.m_RendererType == Renderer_Decal);

	const SRendererFeaturePropertyValue *diffuse								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse());
	const SRendererFeaturePropertyValue *diffuseMap								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse_DiffuseMap());
	const CGuid							legacyDiffuseColor						= renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_Color());
	const CGuid							diffuseColor							= renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_DiffuseColor());

	const SRendererFeaturePropertyValue *emissive								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive());
	const SRendererFeaturePropertyValue *emissiveMap							= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive_EmissiveMap());
	const CGuid							emissiveColor							= renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Emissive_EmissiveColor());

	const SRendererFeaturePropertyValue *fadeTop								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Decal_FadeTop());
	const SRendererFeaturePropertyValue *fadeBottom								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Decal_FadeBottom());
	const SRendererFeaturePropertyValue *fadeAngle								= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Decal_FadeAngle());

	const SRendererFeaturePropertyValue *atlas									= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas());
	const SRendererFeaturePropertyValue *atlasBlending							= renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Blending());

	m_Flags.m_IsLegacy = renderer.m_RendererDeclaration.m_MaterialPath.Contains("Library/PopcornFXCore/Materials/Legacy");

	if (diffuse != null && diffuse->ValueB() && diffuseMap != null && !diffuseMap->ValuePath().Empty() && legacyDiffuseColor.Valid() && diffuseColor.Valid())
	{
		m_DiffuseMap = CStringId(diffuseMap->ValuePath());
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseMap;
	}
	else
	{
		m_Flags.m_ShaderVariationFlags &= ~ShaderVariationFlags::Has_DiffuseMap;
	}

	if (emissive != null && emissive->m_ValueB && emissiveMap != null && !emissiveMap->ValuePath().Empty() && emissiveColor.Valid())
	{
		m_EmissiveMap = CStringId(emissiveMap->ValuePath());
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Emissive;
	}
	else
	{
		m_Flags.m_ShaderVariationFlags &= ~ShaderVariationFlags::Has_Emissive;
	}
	//ToDo
	if (fadeTop != null)
	{
	
	}
	if (fadeBottom != null)
	{
	
	}
	if (fadeAngle != null)
	{
	
	}


	if (atlas != null && atlas->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Atlas;
	else
	{
		m_Flags.m_ShaderVariationFlags &= ~ShaderVariationFlags::Has_Atlas;
	}
	if (atlasBlending != null && atlasBlending->ValueI().x() == 1)
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AnimBlend;
	else
	{
		m_Flags.m_ShaderVariationFlags &= ~ShaderVariationFlags::Has_AnimBlend;
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescDecal::operator == (const CParticleMaterialDescDecal& oth) const
{

	return m_DiffuseMap == oth.m_DiffuseMap && m_EmissiveMap == oth.m_EmissiveMap;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescSound::operator == (const CParticleMaterialDescSound &oth) const
{
	if (m_SoundData == oth.m_SoundData)
		return true;
	return false;
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
			m_DiffuseRampMap == oth.m_DiffuseRampMap &&
			m_EmissiveMap == oth.m_EmissiveMap &&
			m_EmissiveRampMap == oth.m_EmissiveRampMap &&
			m_AlphaMasks1Map == oth.m_AlphaMasks1Map &&
			m_AlphaMasks2Map == oth.m_AlphaMasks2Map &&
			m_AlphaMasks1Intensity == oth.m_AlphaMasks1Intensity &&
			m_AlphaMasks2Intensity == oth.m_AlphaMasks2Intensity &&
			m_AlphaMasks1Weight == oth.m_AlphaMasks1Weight &&
			m_AlphaMasks2Weight == oth.m_AlphaMasks2Weight &&
			m_AlphaMasks1Scale == oth.m_AlphaMasks1Scale &&
			m_AlphaMasks2Scale == oth.m_AlphaMasks2Scale &&
			m_AlphaMasks1RotationSpeed == oth.m_AlphaMasks1RotationSpeed &&
			m_AlphaMasks2RotationSpeed == oth.m_AlphaMasks2RotationSpeed &&
			m_AlphaMasks1TranslationSpeed == oth.m_AlphaMasks1TranslationSpeed &&
			m_AlphaMasks2TranslationSpeed == oth.m_AlphaMasks2TranslationSpeed &&
			m_UVDistortions1Map == oth.m_UVDistortions1Map &&
			m_UVDistortions2Map == oth.m_UVDistortions2Map &&
			m_UVDistortions1Intensity == oth.m_UVDistortions1Intensity &&
			m_UVDistortions2Intensity == oth.m_UVDistortions2Intensity &&
			m_UVDistortions1Scale == oth.m_UVDistortions1Scale &&
			m_UVDistortions2Scale == oth.m_UVDistortions2Scale &&
			m_UVDistortions1RotationSpeed == oth.m_UVDistortions1RotationSpeed &&
			m_UVDistortions2RotationSpeed == oth.m_UVDistortions2RotationSpeed &&
			m_UVDistortions1TranslationSpeed == oth.m_UVDistortions1TranslationSpeed &&
			m_UVDistortions2TranslationSpeed == oth.m_UVDistortions2TranslationSpeed &&
			m_DissolveMap == oth.m_DissolveMap &&
			m_AtlasDefinition == oth.m_AtlasDefinition &&
			m_AtlasSubdivX == oth.m_AtlasSubdivX &&
			m_AtlasSubdivY == oth.m_AtlasSubdivY &&
			m_AlphaThreshold == oth.m_AlphaThreshold;
}

//----------------------------------------------------------------------------

CParticleMaterialDescMesh::CParticleMaterialDescMesh()
:	m_HasMeshAtlas(false)
//Feature Lit
,	m_CastShadows(false)
,	m_NormalBendingFactor(0.5f)
,	m_Roughness(1.0f)
,	m_Metalness(0.0f)
//Feature Vat
,	m_Vat_NumFrames(0)
,	m_Vat_PackedData(false)
,	m_Vat_BoundsPivot(CFloat2::ZERO)
,	m_Vat_NormalizedData(false)
,	m_Vat_BoundsPosition(CFloat2::ZERO)
,	m_Vat_PadToPowerOf2(false)
,	m_Vat_PaddedRatio(CFloat2::ZERO)
,	m_SkeletalAnimUseBoneScale(false)
,	m_SkeletalAnimTextureResol(CUint2::ZERO)
,	m_SkeletalAnimCount(1)
,	m_SkeletalAnimTranslationBoundsMin(CFloat3::ZERO)
,	m_SkeletalAnimTranslationBoundsMax(CFloat3::ZERO)
,	m_SkeletalAnimScaleBoundsMin(CFloat3::ZERO)
,	m_SkeletalAnimScaleBoundsMax(CFloat3::ZERO)
,	m_InvSoftnessDistance(0.0f)
,	m_AlphaThreshold(0.5f)
,	m_TransformUVs_RGBOnly(false)
,	m_UseVertexColor(false)
,	m_AtlasSubdivs(0, 0)
{
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescMesh::InitFromRenderer(const CRendererDataMesh &renderer)
{
	//-----------------------------
	// Choose the shader variation:
	//-----------------------------

	// For Meshes:
	const SRendererFeaturePropertyValue	*diffuse = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse());
	const SRendererFeaturePropertyValue	*diffuseMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Diffuse_DiffuseMap());

	const CGuid							legacyDiffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_Color());
	const CGuid							diffuseColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Diffuse_DiffuseColor());
	
	//const CGuid							distoColorInput = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Distortion_Color());
	const SRendererFeaturePropertyValue	*lit = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Lit());
	const SRendererFeaturePropertyValue	*litLegacy = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLit());
	const SRendererFeaturePropertyValue	*litLegacyOpaque = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_LegacyLitOpaque());
	const SRendererFeaturePropertyValue	*alphaRemap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaRemap());
	const SRendererFeaturePropertyValue	*alphaRemapAlphaMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaRemap_AlphaMap());
	const CGuid							alphaRemapCursor = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_AlphaRemap_Cursor());

	const SRendererFeaturePropertyValue	*diffuseRamp = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_DiffuseRamp());
	const SRendererFeaturePropertyValue	*diffuseRampMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_DiffuseRamp_RampMap());
	const SRendererFeaturePropertyValue	*emissive = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive());
	const CGuid							emissiveColor = renderer.m_Declaration.FindAdditionalFieldIndex(BasicRendererProperties::SID_Emissive_EmissiveColor());
	const SRendererFeaturePropertyValue	*emissiveMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Emissive_EmissiveMap());
	const SRendererFeaturePropertyValue	*emissiveRamp = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_EmissiveRamp());
	const SRendererFeaturePropertyValue	*emissiveRampMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_EmissiveRamp_RampMap());
	const SRendererFeaturePropertyValue	*softParticles = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_SoftParticles());
	const SRendererFeaturePropertyValue	*softnessDistance = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_SoftParticles_SoftnessDistance());
	const SRendererFeaturePropertyValue *atlas = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas());
	const SRendererFeaturePropertyValue *atlasBlending = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Atlas_Blending());
	const SRendererFeaturePropertyValue	*transformUVs = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TransformUVs());
	const SRendererFeaturePropertyValue	*transformUVsRGBOnly = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_TransformUVs_RGBOnly());
	const SRendererFeaturePropertyValue	*doubleSided = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Culling_DoubleSided());

	// Animated Masked
	const SRendererFeaturePropertyValue *alphaMasks = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks());
	const SRendererFeaturePropertyValue *alphaMask1Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Map());
	const SRendererFeaturePropertyValue *alphaMask2Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Map());
	const SRendererFeaturePropertyValue *alphaMask1Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Intensity());
	const SRendererFeaturePropertyValue *alphaMask2Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Intensity());
	const SRendererFeaturePropertyValue *alphaMask1Weight = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Weight());
	const SRendererFeaturePropertyValue *alphaMask2Weight = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Weight());
	const SRendererFeaturePropertyValue *alphaMask1RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1RotationSpeed());
	const SRendererFeaturePropertyValue *alphaMask2RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2RotationSpeed());
	const SRendererFeaturePropertyValue *alphaMask1Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1Scale());
	const SRendererFeaturePropertyValue *alphaMask2Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2Scale());
	const SRendererFeaturePropertyValue *alphaMask1TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask1TranslationSpeed());
	const SRendererFeaturePropertyValue *alphaMask2TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_AlphaMasks_Mask2TranslationSpeed());

	const SRendererFeaturePropertyValue *uvDistortions = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions());
	const SRendererFeaturePropertyValue *uvDistortions1Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Map());
	const SRendererFeaturePropertyValue *uvDistortions2Map = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Map());
	const SRendererFeaturePropertyValue *uvDistortions1Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Intensity());
	const SRendererFeaturePropertyValue *uvDistortions2Intensity = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Intensity());
	const SRendererFeaturePropertyValue *uvDistortions1RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1RotationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions2RotationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2RotationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions1Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1Scale());
	const SRendererFeaturePropertyValue *uvDistortions2Scale = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2Scale());
	const SRendererFeaturePropertyValue *uvDistortions1TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion1TranslationSpeed());
	const SRendererFeaturePropertyValue *uvDistortions2TranslationSpeed = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UVDistortions_Distortion2TranslationSpeed());

	const SRendererFeaturePropertyValue *dissolve = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve());
	const SRendererFeaturePropertyValue *dissolveMap = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve_DissolveMap());
	const SRendererFeaturePropertyValue *dissolveSoftness = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Dissolve_DissolveSoftness());

	m_Flags.m_IsLegacy = renderer.m_RendererDeclaration.m_MaterialPath.Contains("Library/PopcornFXCore/Materials/Legacy");
	m_Flags.m_ShaderVariationFlags = 0;

	if (diffuse != null && diffuse->ValueB() && diffuseMap != null && !diffuseMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseMap;
	if (alphaRemap != null && alphaRemap->ValueB() && alphaRemapAlphaMap != null && !alphaRemapAlphaMap->ValuePath().Empty() && alphaRemapCursor.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AlphaRemap;
	if (diffuseRamp != null && diffuseRamp->ValueB() && diffuseRampMap != null && !diffuseRampMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DiffuseRamp;
	if (emissive != null && emissive->ValueB() && emissiveColor.Valid() && emissiveMap != null &&  !emissiveMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Emissive;
	if (emissiveRamp != null && emissiveRamp->ValueB() && emissiveRampMap != null && !emissiveRampMap->ValuePath().Empty())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_EmissiveRamp;
	if ((lit != null && lit->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Lighting;
	if ((litLegacy != null && litLegacy->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_LightingLegacy;
	if ((litLegacyOpaque != null && litLegacyOpaque->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_LightingLegacyOpaque;
	if (softParticles != null && softParticles->ValueB() && softnessDistance != null && softnessDistance->ValueF().x() != 0.0f)
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Soft;
	if (atlas != null && atlas->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Atlas;
	if (atlasBlending != null && atlasBlending->ValueI().x() == 1)
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AnimBlend;
	if (transformUVs != null && transformUVs->ValueB())
	{
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_TransformUVs;
		if (transformUVsRGBOnly != null && transformUVsRGBOnly->ValueB())
			m_TransformUVs_RGBOnly = transformUVsRGBOnly->ValueB();
	}
	if (doubleSided != null && doubleSided->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_DoubleSided;

	if (diffuseColorInput.Valid() || legacyDiffuseColorInput.Valid())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Color;
	if (alphaMasks != null && alphaMasks->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_AlphaMasks;
	if (uvDistortions != null && uvDistortions->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_UVDistortions;
	if (dissolve != null && dissolve->ValueB())
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_Dissolve;

	// Animated mesh:
	const SRendererFeaturePropertyValue	*skeletalAnim = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation());
	const SRendererFeaturePropertyValue	*skeletalAnimTexResol = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation_AnimTextureResolution());
	const SRendererFeaturePropertyValue	*skeletalAnimTex = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation_AnimationTexture());
	const SRendererFeaturePropertyValue	*skeletalAnimInterpolate = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimationInterpolate());
	const SRendererFeaturePropertyValue	*skeletalAnimInterpolateTracks = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks());
	const SRendererFeaturePropertyValue	*skeletalAnimUseBonesScale = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimationUseBonesScale());
	const SRendererFeaturePropertyValue	*skeletalAnimBonesTranslateBoundsMin = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation_AnimPositionsBoundsMin());
	const SRendererFeaturePropertyValue	*skeletalAnimBonesTranslateBoundsMax = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation_AnimPositionsBoundsMax());
	const SRendererFeaturePropertyValue	*skeletalAnimBonesScaleBoundsMin = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimationUseBonesScale_AnimScalesBoundsMin());
	const SRendererFeaturePropertyValue	*skeletalAnimBonesScaleBoundsMax = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimationUseBonesScale_AnimScalesBoundsMax());
	const SRendererFeaturePropertyValue* skeletalAnimCount = renderer.m_Declaration.FindProperty(SkeletalAnimationTexture::SID_SkeletalAnimation_AnimTracksCount());

	if (skeletalAnim != null && skeletalAnim->ValueB() && skeletalAnimTexResol != null)
	{
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_SkeletalAnim;
		if (skeletalAnimInterpolate != null && skeletalAnimInterpolate->ValueB())
			m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_SkeletalInterpol;
		if (skeletalAnimInterpolateTracks != null && skeletalAnimInterpolateTracks->ValueB())
			m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_SkeletalTrackInterpol;
	}

	// Animated Masked
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks) != 0)
	{
		m_AlphaMasks1Map = CStringId(alphaMask1Map->ValuePath());
		m_AlphaMasks2Map = CStringId(alphaMask2Map->ValuePath());
		m_AlphaMasks1Intensity = alphaMask1Intensity->ValueF().x();
		m_AlphaMasks2Intensity = alphaMask2Intensity->ValueF().x();
		m_AlphaMasks1Weight = alphaMask1Weight->ValueF().x();
		m_AlphaMasks2Weight = alphaMask2Weight->ValueF().x();
		m_AlphaMasks1Scale = alphaMask1Scale->ValueF().xy();
		m_AlphaMasks2Scale = alphaMask2Scale->ValueF().xy();
		m_AlphaMasks1RotationSpeed = alphaMask1RotationSpeed->ValueF().x();
		m_AlphaMasks2RotationSpeed = alphaMask2RotationSpeed->ValueF().x();
		m_AlphaMasks1TranslationSpeed = alphaMask1TranslationSpeed->ValueF().xy();
		m_AlphaMasks2TranslationSpeed = alphaMask2TranslationSpeed->ValueF().xy();	
	}
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_UVDistortions) != 0)
	{
		m_UVDistortions1Map = CStringId(uvDistortions1Map->ValuePath());
		m_UVDistortions2Map = CStringId(uvDistortions2Map->ValuePath());
		m_UVDistortions1Intensity = uvDistortions1Intensity->ValueF().x();
		m_UVDistortions2Intensity = uvDistortions2Intensity->ValueF().x();
		m_UVDistortions1Scale = uvDistortions1Scale->ValueF().xy();
		m_UVDistortions2Scale = uvDistortions2Scale->ValueF().xy();
		m_UVDistortions1RotationSpeed = uvDistortions1RotationSpeed->ValueF().x();
		m_UVDistortions2RotationSpeed = uvDistortions2RotationSpeed->ValueF().x();
		m_UVDistortions1TranslationSpeed = uvDistortions1TranslationSpeed->ValueF().xy();
		m_UVDistortions2TranslationSpeed = uvDistortions2TranslationSpeed->ValueF().xy();	
	}

	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Dissolve) != 0)
	{
		m_DissolveMap = CStringId(dissolveMap->ValuePath());
		m_DissolveSoftness = dissolveSoftness->ValueF().x();
	}


	//-----------------------------
	// Choose the blending mode:
	//-----------------------------

	const SRendererFeaturePropertyValue	*transparent = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent());
	const SRendererFeaturePropertyValue	*transparentType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_Type());
	const SRendererFeaturePropertyValue	*opaque = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque());
	const SRendererFeaturePropertyValue	*opaqueType = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_Type());
	const SRendererFeaturePropertyValue	*alphaThreshold = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Opaque_MaskThreshold());

	// Default blend mode is solid:
	m_Flags.m_BlendMode = BlendMode::Solid;

	if (transparent != null && transparent->ValueB())
	{		
		const SRendererFeaturePropertyValue *globalSortOverride = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Transparent_GlobalSortOverride());
		m_Flags.m_DrawOrder = globalSortOverride != null ? globalSortOverride->ValueI().x() : 0;

		if (!m_Flags.m_IsLegacy)
		{
			m_Flags.m_BlendMode = BlendMode::PremultipliedAlpha; // Allows for both AlphaBlend and Additive in the same shader
			if (!diffuse)
				m_Flags.m_BlendMode = BlendMode::Additive;
		}
		else if (transparentType != null)
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
			if (alphaThreshold != null)
			{
				m_AlphaThreshold = alphaThreshold->ValueF().x();
			}
		}
	}

	//-----------------------------
	// Retrieve the shader uniforms (mesh resource, property values...):
	//-----------------------------
	const SRendererFeaturePropertyValue	*mesh = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_Mesh());
	const SRendererFeaturePropertyValue	*meshAtlas = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_MeshAtlas());
	const SRendererFeaturePropertyValue* meshUseVertexColor = renderer.m_Declaration.FindProperty(BasicRendererProperties::SID_UseVertexColor());

	if (PK_VERIFY(mesh != null && !mesh->ValuePath().Empty()))
		m_MeshPath = CStringId(CFilePath::Purified(mesh->ValuePath()));
	else
		return false;

	if (meshUseVertexColor != null && meshUseVertexColor->ValueB())
		m_UseVertexColor = true;
	// Skeletal anim:
	if (skeletalAnim != null && skeletalAnim->ValueB() && skeletalAnimTexResol != null && skeletalAnimTex != null &&
		skeletalAnimBonesTranslateBoundsMin != null && skeletalAnimBonesTranslateBoundsMax != null && skeletalAnimCount != null)
	{
		m_SkeletalAnimationTexture = CStringId(CFilePath::Purified(skeletalAnimTex->ValuePath()));
		m_SkeletalAnimTextureResol = skeletalAnimTexResol->ValueI().xy();
		m_SkeletalAnimTranslationBoundsMin = skeletalAnimBonesTranslateBoundsMin->ValueF().xyz();
		m_SkeletalAnimTranslationBoundsMax = skeletalAnimBonesTranslateBoundsMax->ValueF().xyz();
		m_SkeletalAnimCount = (u32)skeletalAnimCount->ValueI().x();
		m_UseVertexColor = true;
		if (skeletalAnimUseBonesScale != null && skeletalAnimUseBonesScale->ValueB() &&
			skeletalAnimBonesScaleBoundsMin != null && skeletalAnimBonesScaleBoundsMax != null)
		{
			m_SkeletalAnimUseBoneScale = true;
			m_SkeletalAnimScaleBoundsMin = skeletalAnimBonesScaleBoundsMin->ValueF().xyz();
			m_SkeletalAnimScaleBoundsMax = skeletalAnimBonesScaleBoundsMax->ValueF().xyz();
		}
	}

	const bool hasMeshAtlas = (meshAtlas != null) ? meshAtlas->ValueB() : false;
	if (hasMeshAtlas)
		m_HasMeshAtlas = true;
	else
		m_HasMeshAtlas = false;
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DiffuseMap) != 0)
		m_DiffuseMap = CStringId(CFilePath::Purified(diffuseMap->ValuePath()));
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
		m_AlphaMap = CStringId(alphaRemapAlphaMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_DiffuseRamp) != 0)
		m_DiffuseRampMap = CStringId(diffuseRampMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Emissive && emissiveMap != null && !emissiveMap->ValuePath().Empty()) != 0)
		m_EmissiveMap = CStringId(emissiveMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_EmissiveRamp) != 0)
		m_EmissiveRampMap = CStringId(emissiveRampMap->ValuePath());
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Soft) != 0)
		m_InvSoftnessDistance = 1.0f / softnessDistance->ValueF().x();

	//-----------------------------
	// VAT features
	//-----------------------------
	const SRendererFeaturePropertyValue	*fluid = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid());
	const SRendererFeaturePropertyValue	*soft = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft());
	const SRendererFeaturePropertyValue	*rigid = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Rigid());

	if ((fluid != null && fluid->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_FluidVAT;
	else if ((soft != null && soft->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_SoftVAT;
	else if ((rigid != null && rigid->ValueB()))
		m_Flags.m_ShaderVariationFlags |= ShaderVariationFlags::Has_RigidVAT;

	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_FluidVAT) != 0)
	{
		m_UseVertexColor = true;
		const SRendererFeaturePropertyValue	*fluid_PositionMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_PositionMap());
		if (fluid_PositionMap != null && !fluid_PositionMap->ValuePath().Empty())
			m_Vat_PositionMap = CStringId(CFilePath::Purified(fluid_PositionMap->ValuePath()));

		const SRendererFeaturePropertyValue	*fluid_NormalMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_NormalMap());
		if (fluid_NormalMap != null && !fluid_NormalMap->ValuePath().Empty())
			m_Vat_NormalMap = CStringId(CFilePath::Purified(fluid_NormalMap->ValuePath()));

		const SRendererFeaturePropertyValue	*fluid_ColorMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_ColorMap());
		if (fluid_ColorMap != null && !fluid_ColorMap->ValuePath().Empty())
			m_Vat_ColorMap = CStringId(CFilePath::Purified(fluid_ColorMap->ValuePath()));

		const SRendererFeaturePropertyValue	*fluid_NumFrames = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_NumFrames());
		if (fluid_NumFrames != null)
			m_Vat_NumFrames = fluid_NumFrames->ValueI().x();

		const SRendererFeaturePropertyValue	*fluid_PackedData = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_PackedData());
		if (fluid_PackedData != null)
			m_Vat_PackedData = fluid_PackedData->ValueB();
	}
	else if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_SoftVAT) != 0)
	{
		m_UseVertexColor = true;
		const SRendererFeaturePropertyValue	*soft_PositionMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft_PositionMap());
		if (soft_PositionMap != null && !soft_PositionMap->ValuePath().Empty())
			m_Vat_PositionMap = CStringId(CFilePath::Purified(soft_PositionMap->ValuePath()));

		const SRendererFeaturePropertyValue	*soft_NormalMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft_NormalMap());
		if (soft_NormalMap != null && !soft_NormalMap->ValuePath().Empty())
			m_Vat_NormalMap = CStringId(CFilePath::Purified(soft_NormalMap->ValuePath()));

		const SRendererFeaturePropertyValue	*soft_ColorMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft_ColorMap());
		if (soft_ColorMap != null && !soft_ColorMap->ValuePath().Empty())
			m_Vat_ColorMap = CStringId(CFilePath::Purified(soft_ColorMap->ValuePath()));

		const SRendererFeaturePropertyValue	*soft_NumFrames = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft_NumFrames());
		if (soft_NumFrames != null)
			m_Vat_NumFrames = soft_NumFrames->ValueI().x();

		const SRendererFeaturePropertyValue	*soft_PackedData = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Soft_PackedData());
		if (soft_PackedData != null)
			m_Vat_PackedData = soft_PackedData->ValueB();
	}
	else if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_RigidVAT) != 0)
	{
		m_UseVertexColor = true;
		const SRendererFeaturePropertyValue	*rigid_PositionMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_PositionMap());
		if (rigid_PositionMap != null && !rigid_PositionMap->ValuePath().Empty())
			m_Vat_PositionMap = CStringId(CFilePath::Purified(rigid_PositionMap->ValuePath()));

		const SRendererFeaturePropertyValue	*rigid_RotationMap = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_RotationMap());
		if (rigid_RotationMap != null && !rigid_RotationMap->ValuePath().Empty())
			m_Vat_RotationMap = CStringId(CFilePath::Purified(rigid_RotationMap->ValuePath()));

		const SRendererFeaturePropertyValue	*rigid_NumFrames = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_NumFrames());
		if (rigid_NumFrames != null)
			m_Vat_NumFrames = rigid_NumFrames->ValueI().x();

		const SRendererFeaturePropertyValue	*rigid_BoundsPivot = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_BoundsPivot());
		if (rigid_BoundsPivot != null)
			m_Vat_BoundsPivot = CFloat2{ rigid_BoundsPivot->ValueF().x(), rigid_BoundsPivot->ValueF().y() };
	}

	const SRendererFeaturePropertyValue	*vat_NormalizedData = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_NormalizedData());
	if (vat_NormalizedData != null)
		m_Vat_NormalizedData = vat_NormalizedData->ValueB();

	const SRendererFeaturePropertyValue	*vat_BoundsPosition = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_BoundsPosition());
	if (vat_BoundsPosition != null)
		m_Vat_BoundsPosition = CFloat2{ vat_BoundsPosition->ValueF().x(), vat_BoundsPosition->ValueF().y() };

	const SRendererFeaturePropertyValue	*vat_PadToPowerOf2 = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_PadToPowerOf2());
	if (vat_PadToPowerOf2 != null && vat_PadToPowerOf2->ValueB())
	{
		m_Vat_PadToPowerOf2 = true;

		const SRendererFeaturePropertyValue	*vat_PaddedRatio = renderer.m_Declaration.FindProperty(VertexAnimationRendererProperties::SID_VertexAnimation_PaddedRatio());
		if (vat_PaddedRatio != null)
			m_Vat_PaddedRatio = CFloat2{ vat_PaddedRatio->ValueF().x(), vat_PaddedRatio->ValueF().y() };
	}
	else
		m_Vat_PadToPowerOf2 = false;

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
			m_NormalMap = CStringId(CFilePath::Purified(normalMap->ValuePath()));
		if (roughMetalMap != null && !roughMetalMap->ValuePath().Empty())
			m_RoughMetalMap = CStringId(CFilePath::Purified(roughMetalMap->ValuePath()));
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
		//if (m_NormalMap.Empty() && normalMapLegacy != null && !normalMapLegacy->ValuePath().Empty())
		//{
		//	m_NormalMap = CStringId(normalMapLegacy->ValuePath());
		//}
		CLog::Log(PK_WARN, "Legacy Lit render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
	}
	if ((m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_LightingLegacyOpaque) != 0)
	{
		CLog::Log(PK_WARN, "Legacy Lit Opaque render feature is not supported in Unity, Please use Lit instead (in PopcornFX editor)");
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
						m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Emissive) ||
						m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DistortionMap);
	m_Flags.m_FlipU = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::FlipU);
	m_Flags.m_FlipV = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::FlipV);
	m_Flags.m_RotateTexture = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::RotateUV);
	m_Flags.m_HasAtlasBlending = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_AnimBlend);
	m_Flags.m_HasRibbonCorrectDeformation = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_CorrectDeformation);
	m_Flags.m_HasNormal = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Lighting);
	m_Flags.m_HasTangent = m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Lighting);
	m_Flags.m_HasRawUV0 =	 m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Atlas) &&
							(m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_AlphaMasks) ||
							m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_UVDistortions) ||
							m_MaterialDescBillboard.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Dissolve));
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
	if (m_RendererType == Renderer_Sound)
	{
		return	m_MaterialDescSound == oth.m_MaterialDescSound;
	}
	if (m_RendererType == Renderer_Decal)
	{
		return m_MaterialDescDecal == oth.m_MaterialDescDecal;
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
	desc.m_IsLegacy = m_MaterialDescBillboard.m_Flags.m_IsLegacy ? ManagedBool_True : ManagedBool_False;
	desc.m_RotateUvs = m_MaterialDescBillboard.m_Flags.HasUVGenerationFlags(UVGeneration::RotateUV) ? ManagedBool_True : ManagedBool_False;
	desc.m_DiffuseMap = m_MaterialDescBillboard.m_DiffuseMap.ToStringData();
	desc.m_EmissiveMap = m_MaterialDescBillboard.m_EmissiveMap.ToStringData();
	desc.m_AlphaRemap = m_MaterialDescBillboard.m_AlphaMap.ToStringData();
	desc.m_DiffuseRampMap = m_MaterialDescBillboard.m_DiffuseRampMap.ToStringData();
	desc.m_EmissiveRampMap = m_MaterialDescBillboard.m_EmissiveRampMap.ToStringData();

	if (m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks)
	{
		desc.m_AlphaMasks = PK_NEW(SRenderingFeatureAlphaMasksDesc);
		desc.m_AlphaMasks->m_AlphaMasks1Map = m_MaterialDescBillboard.m_AlphaMasks1Map.ToStringData();
		desc.m_AlphaMasks->m_AlphaMasks2Map = m_MaterialDescBillboard.m_AlphaMasks2Map.ToStringData();
		desc.m_AlphaMasks->m_AlphaMasks1Intensity = m_MaterialDescBillboard.m_AlphaMasks1Intensity;
		desc.m_AlphaMasks->m_AlphaMasks2Intensity = m_MaterialDescBillboard.m_AlphaMasks2Intensity;
		desc.m_AlphaMasks->m_AlphaMasks1Weight = m_MaterialDescBillboard.m_AlphaMasks1Weight;
		desc.m_AlphaMasks->m_AlphaMasks2Weight = m_MaterialDescBillboard.m_AlphaMasks2Weight;
		desc.m_AlphaMasks->m_AlphaMasks1Scale = m_MaterialDescBillboard.m_AlphaMasks1Scale;
		desc.m_AlphaMasks->m_AlphaMasks2Scale = m_MaterialDescBillboard.m_AlphaMasks2Scale;
		desc.m_AlphaMasks->m_AlphaMasks1RotationSpeed = m_MaterialDescBillboard.m_AlphaMasks1RotationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks2RotationSpeed = m_MaterialDescBillboard.m_AlphaMasks2RotationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks1TranslationSpeed = m_MaterialDescBillboard.m_AlphaMasks1TranslationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks2TranslationSpeed = m_MaterialDescBillboard.m_AlphaMasks2TranslationSpeed;
	}
	
	if (m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_UVDistortions)
	{
		desc.m_UVDistortions = PK_NEW(SRenderingFeatureUVDistortionsDesc);
		desc.m_UVDistortions->m_UVDistortions1Map = m_MaterialDescBillboard.m_UVDistortions1Map.ToStringData();
		desc.m_UVDistortions->m_UVDistortions2Map = m_MaterialDescBillboard.m_UVDistortions2Map.ToStringData();
		desc.m_UVDistortions->m_UVDistortions1Intensity = m_MaterialDescBillboard.m_UVDistortions1Intensity;
		desc.m_UVDistortions->m_UVDistortions2Intensity = m_MaterialDescBillboard.m_UVDistortions2Intensity;
		desc.m_UVDistortions->m_UVDistortions1Scale = m_MaterialDescBillboard.m_UVDistortions1Scale;
		desc.m_UVDistortions->m_UVDistortions2Scale = m_MaterialDescBillboard.m_UVDistortions2Scale;
		desc.m_UVDistortions->m_UVDistortions1RotationSpeed = m_MaterialDescBillboard.m_UVDistortions1RotationSpeed;
		desc.m_UVDistortions->m_UVDistortions2RotationSpeed = m_MaterialDescBillboard.m_UVDistortions2RotationSpeed;
		desc.m_UVDistortions->m_UVDistortions1TranslationSpeed = m_MaterialDescBillboard.m_UVDistortions1TranslationSpeed;
		desc.m_UVDistortions->m_UVDistortions2TranslationSpeed = m_MaterialDescBillboard.m_UVDistortions2TranslationSpeed;
	}

	if (m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Dissolve)
	{
		desc.m_Dissolve = PK_NEW(SRenderingFeatureDissolveDesc);
		desc.m_Dissolve->m_DissolveMap = m_MaterialDescBillboard.m_DissolveMap.ToStringData();
		desc.m_Dissolve->m_DissolveSoftness = m_MaterialDescBillboard.m_DissolveSoftness;
	}

	desc.m_InvSoftnessDistance = m_MaterialDescBillboard.m_InvSoftnessDistance;

	switch (m_RendererType) {

		case ERendererClass::Renderer_Billboard:
			desc.m_BillboardMode = m_BillboardBR.m_Mode;
			break;
		case ERendererClass::Renderer_Ribbon:
			desc.m_BillboardMode = m_RibbonBR.m_Mode;
			break;
		default:
			desc.m_BillboardMode = 0;
			break;
	}

	desc.m_DrawOrder = m_MaterialDescBillboard.m_Flags.m_DrawOrder;
	desc.m_AlphaClipThreshold = m_MaterialDescBillboard.m_AlphaThreshold;
	desc.m_TransformUVs_RGBOnly = m_MaterialDescBillboard.m_TransformUVs_RGBOnly  ? ManagedBool_True : ManagedBool_False;

	desc.m_UID = m_UID;
	if ((m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		desc.m_LitRendering = PK_NEW(SRenderingFeatureLitDesc);

		if (!PK_VERIFY(desc.m_LitRendering != null))
			return false;

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

bool	CUnityRendererCache::GetRendererInfo(SDecalRendererDesc &desc)
{
	if (m_MaterialDescDecal.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DiffuseMap) &&
		!m_MaterialDescDecal.m_DiffuseMap.Valid() ||
		m_MaterialDescDecal.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Emissive) &&
		!m_MaterialDescDecal.m_EmissiveMap.Valid())
		return false;

	desc.m_ShaderVariationFlags		= m_MaterialDescDecal.m_Flags.m_ShaderVariationFlags;

	desc.m_DiffuseMap				= m_MaterialDescDecal.m_DiffuseMap.ToStringData();
	desc.m_EmissiveMap				= m_MaterialDescDecal.m_EmissiveMap.ToStringData();
	desc.m_DiffuseColor				= m_MaterialDescDecal.m_DiffuseColor;
	desc.m_EmissiveColor			= m_MaterialDescDecal.m_EmissiveColor;
	desc.m_UID						= m_UID;

	if (m_Atlas != null)
	{
		desc.m_TextureAtlasCount = m_Atlas->AtlasRectCount();
		desc.m_TextureAtlas = static_cast<CFloat4 *>(PK_MALLOC(m_Atlas->AtlasRectCount() * sizeof(CFloat4)));
		Mem::Copy(desc.m_TextureAtlas, m_Atlas->m_RectsFp32.RawDataPointer(), desc.m_TextureAtlasCount * sizeof(CFloat4));
	}

	return true;
}

bool	CUnityRendererCache::GetRendererInfo(SMeshRendererDesc &desc)
{
	desc.m_UID = m_UID;
	desc.m_MeshAsset = m_MaterialDescMesh.m_MeshPath.ToStringData();
	if (desc.m_MeshAsset == null)
		return false;
	desc.m_ShaderVariationFlags = m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags;
	desc.m_BlendMode = m_MaterialDescMesh.m_Flags.m_BlendMode;
	desc.m_IsLegacy = m_MaterialDescMesh.m_Flags.m_IsLegacy ? ManagedBool_True : ManagedBool_False;
	desc.m_HasMeshAtlas = m_MaterialDescMesh.m_HasMeshAtlas ? ManagedBool_True : ManagedBool_False;
	desc.m_DiffuseMap = m_MaterialDescMesh.m_DiffuseMap.ToStringData();
	desc.m_InvSoftnessDistance = m_MaterialDescMesh.m_InvSoftnessDistance;
	desc.m_AlphaClipThreshold = m_MaterialDescMesh.m_AlphaThreshold;
	desc.m_DrawOrder = m_MaterialDescMesh.m_Flags.m_DrawOrder;
	desc.m_TransformUVs_RGBOnly = m_MaterialDescMesh.m_TransformUVs_RGBOnly  ? ManagedBool_True : ManagedBool_False;
	desc.m_UseVertexColor = m_MaterialDescMesh.m_UseVertexColor ? ManagedBool_True : ManagedBool_False;

	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks) != 0)
	{
		desc.m_AlphaMasks = PK_NEW(SRenderingFeatureAlphaMasksDesc);
		desc.m_AlphaMasks->m_AlphaMasks1Map = m_MaterialDescMesh.m_AlphaMasks1Map.ToStringData();
		desc.m_AlphaMasks->m_AlphaMasks2Map = m_MaterialDescMesh.m_AlphaMasks2Map.ToStringData();
		desc.m_AlphaMasks->m_AlphaMasks1Intensity = m_MaterialDescMesh.m_AlphaMasks1Intensity;
		desc.m_AlphaMasks->m_AlphaMasks2Intensity = m_MaterialDescMesh.m_AlphaMasks2Intensity;
		desc.m_AlphaMasks->m_AlphaMasks1Weight = m_MaterialDescMesh.m_AlphaMasks1Weight;
		desc.m_AlphaMasks->m_AlphaMasks2Weight = m_MaterialDescMesh.m_AlphaMasks2Weight;
		desc.m_AlphaMasks->m_AlphaMasks1Scale = m_MaterialDescMesh.m_AlphaMasks1Scale;
		desc.m_AlphaMasks->m_AlphaMasks2Scale = m_MaterialDescMesh.m_AlphaMasks2Scale;
		desc.m_AlphaMasks->m_AlphaMasks1RotationSpeed = m_MaterialDescMesh.m_AlphaMasks1RotationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks2RotationSpeed = m_MaterialDescMesh.m_AlphaMasks2RotationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks1TranslationSpeed = m_MaterialDescMesh.m_AlphaMasks1TranslationSpeed;
		desc.m_AlphaMasks->m_AlphaMasks2TranslationSpeed = m_MaterialDescMesh.m_AlphaMasks2TranslationSpeed;
	}
	
	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_UVDistortions) != 0)
	{
		desc.m_UVDistortions = PK_NEW(SRenderingFeatureUVDistortionsDesc);
		desc.m_UVDistortions->m_UVDistortions1Map = m_MaterialDescMesh.m_UVDistortions1Map.ToStringData();
		desc.m_UVDistortions->m_UVDistortions2Map = m_MaterialDescMesh.m_UVDistortions2Map.ToStringData();
		desc.m_UVDistortions->m_UVDistortions1Intensity = m_MaterialDescMesh.m_UVDistortions1Intensity;
		desc.m_UVDistortions->m_UVDistortions2Intensity = m_MaterialDescMesh.m_UVDistortions2Intensity;
		desc.m_UVDistortions->m_UVDistortions1Scale = m_MaterialDescMesh.m_UVDistortions1Scale;
		desc.m_UVDistortions->m_UVDistortions2Scale = m_MaterialDescMesh.m_UVDistortions2Scale;
		desc.m_UVDistortions->m_UVDistortions1RotationSpeed = m_MaterialDescMesh.m_UVDistortions1RotationSpeed;
		desc.m_UVDistortions->m_UVDistortions2RotationSpeed = m_MaterialDescMesh.m_UVDistortions2RotationSpeed;
		desc.m_UVDistortions->m_UVDistortions1TranslationSpeed = m_MaterialDescMesh.m_UVDistortions1TranslationSpeed;
		desc.m_UVDistortions->m_UVDistortions2TranslationSpeed = m_MaterialDescMesh.m_UVDistortions2TranslationSpeed;
	}

	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Dissolve) != 0)
	{
		desc.m_Dissolve = PK_NEW(SRenderingFeatureDissolveDesc);
		desc.m_Dissolve->m_DissolveMap = m_MaterialDescMesh.m_DissolveMap.ToStringData();
		desc.m_Dissolve->m_DissolveSoftness = m_MaterialDescMesh.m_DissolveSoftness;
	}

	bool fluidVAT = (m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_FluidVAT) != 0;
	bool softVAT = (m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_SoftVAT) != 0;
	bool rigidVAT = (m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_RigidVAT) != 0;

	if (fluidVAT || softVAT || rigidVAT)
	{
		desc.m_VatRendering = PK_NEW(SRenderingFeatureVATDesc);

		if (!PK_VERIFY(desc.m_VatRendering != null))
			return false;

		if (!m_MaterialDescMesh.m_Vat_PositionMap.Empty())
			desc.m_VatRendering->m_PositionMap = m_MaterialDescMesh.m_Vat_PositionMap.ToStringData();

		if (softVAT)
		{
			if (!m_MaterialDescMesh.m_Vat_NormalMap.Empty())
				desc.m_VatRendering->m_NormalMap = m_MaterialDescMesh.m_Vat_NormalMap.ToStringData();
		}

		if (fluidVAT || softVAT)
		{
			if (!m_MaterialDescMesh.m_Vat_ColorMap.Empty())
				desc.m_VatRendering->m_ColorMap = m_MaterialDescMesh.m_Vat_ColorMap.ToStringData();

			desc.m_VatRendering->m_PackedData = m_MaterialDescMesh.m_Vat_PackedData ? ManagedBool_True : ManagedBool_False;
		}

		desc.m_VatRendering->m_NumFrames = m_MaterialDescMesh.m_Vat_NumFrames;

		if (rigidVAT)
		{
			if (!m_MaterialDescMesh.m_Vat_RotationMap.Empty())
				desc.m_VatRendering->m_RotationMap = m_MaterialDescMesh.m_Vat_RotationMap.ToStringData();

			desc.m_VatRendering->m_BoundsPivot = m_MaterialDescMesh.m_Vat_BoundsPivot;
		}

		desc.m_VatRendering->m_NormalizedData = m_MaterialDescMesh.m_Vat_NormalizedData ? ManagedBool_True : ManagedBool_False;
		desc.m_VatRendering->m_BoundsPosition = m_MaterialDescMesh.m_Vat_BoundsPosition;
		
		desc.m_VatRendering->m_PadToPowerOf2 = m_MaterialDescMesh.m_Vat_PadToPowerOf2 ? ManagedBool_True : ManagedBool_False;
		desc.m_VatRendering->m_PaddedRatio = m_MaterialDescMesh.m_Vat_PaddedRatio;
	}
	else
		desc.m_VatRendering = null;

	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		desc.m_LitRendering = PK_NEW(SRenderingFeatureLitDesc);
		if (!PK_VERIFY(desc.m_LitRendering != null))
			return false;
		desc.m_LitRendering->m_NormalMap = m_MaterialDescMesh.m_NormalMap.ToStringData();
		desc.m_LitRendering->m_RoughMetalMap = m_MaterialDescMesh.m_RoughMetalMap.ToStringData();
		desc.m_LitRendering->m_CastShadows = m_MaterialDescMesh.m_CastShadows ? ManagedBool_True : ManagedBool_False;
		desc.m_LitRendering->m_NormalBendingFactor = m_MaterialDescMesh.m_NormalBendingFactor;
		desc.m_LitRendering->m_Roughness = m_MaterialDescMesh.m_Roughness;
		desc.m_LitRendering->m_Metalness = m_MaterialDescMesh.m_Metalness;
	}
	else
		desc.m_LitRendering = null;

	if ((m_MaterialDescMesh.m_Flags.m_ShaderVariationFlags & ShaderVariationFlags::Has_SkeletalAnim) != 0)
	{
		desc.m_AnimDescRendering = PK_NEW(SRenderingFeatureSkeletalAnimDesc);
		if (!PK_VERIFY(desc.m_AnimDescRendering != null))
			return false;
		desc.m_AnimDescRendering->m_AnimTexture = m_MaterialDescMesh.m_SkeletalAnimationTexture.ToStringData();
		desc.m_AnimDescRendering->m_TextureResol = m_MaterialDescMesh.m_SkeletalAnimTextureResol;
		desc.m_AnimDescRendering->m_AnimCount = m_MaterialDescMesh.m_SkeletalAnimCount;
		desc.m_AnimDescRendering->m_UseBoneScale = m_MaterialDescMesh.m_SkeletalAnimUseBoneScale ? ManagedBool_True : ManagedBool_False;
		desc.m_AnimDescRendering->m_TranslationBoundsMin = m_MaterialDescMesh.m_SkeletalAnimTranslationBoundsMin;
		desc.m_AnimDescRendering->m_TranslationBoundsMax = m_MaterialDescMesh.m_SkeletalAnimTranslationBoundsMax;
		desc.m_AnimDescRendering->m_ScaleBoundsMin = m_MaterialDescMesh.m_SkeletalAnimScaleBoundsMin;
		desc.m_AnimDescRendering->m_ScaleBoundsMax = m_MaterialDescMesh.m_SkeletalAnimScaleBoundsMax;
	}
	if (m_Atlas != null)
	{
		desc.m_TextureAtlasCount = m_Atlas->AtlasRectCount();
		desc.m_TextureAtlas = static_cast<CFloat4*>(PK_MALLOC(m_Atlas->AtlasRectCount() * sizeof(CFloat4)));
		Mem::Copy(desc.m_TextureAtlas, m_Atlas->m_RectsFp32.RawDataPointer(), desc.m_TextureAtlasCount * sizeof(CFloat4));
	}

	if (!m_MaterialDescMesh.m_DiffuseRampMap.Empty())
		desc.m_DiffuseRampMap = m_MaterialDescMesh.m_DiffuseRampMap.ToStringData();
	if (!m_MaterialDescMesh.m_EmissiveMap.Empty())
		desc.m_EmissiveMap = m_MaterialDescMesh.m_EmissiveMap.ToStringData();
	if (!m_MaterialDescMesh.m_EmissiveRampMap.Empty())
		desc.m_EmissiveRampMap = m_MaterialDescMesh.m_EmissiveRampMap.ToStringData();
	if (!m_MaterialDescMesh.m_AlphaMap.Empty())
		desc.m_AlphaRemap = m_MaterialDescMesh.m_AlphaMap.ToStringData();
	return true;
}

void		CUnityRendererCache::CreateUnityMesh(u32 idx, bool gpuBillboarding, bool isEmissive3)
{
	for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
	{
		if (m_RendererType == Renderer_Billboard || m_RendererType == Renderer_Ribbon || m_RendererType == Renderer_Triangle || m_RendererType == Renderer_Decal)
		{
			// Renderer info in case we need to update the renderer's buffers:
			SRetrieveRendererInfo	rendererInfo;
			ManagedBool				hasCustomMat = ManagedBool_False;
			ManagedBool				useComputeBuffers = ManagedBool_False;

			rendererInfo.m_VBHandler = &m_UnityMeshInfoPerViews[i].m_VBHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_IBHandler = &m_UnityMeshInfoPerViews[i].m_IBHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_VertexBufferSize = &m_UnityMeshInfo.m_VBElemCount;
			rendererInfo.m_IndexBufferSize = &m_UnityMeshInfo.m_IBElemCount;
			rendererInfo.m_IsIndex32 = &m_UnityMeshInfo.m_LargeIndices;

			rendererInfo.m_HasCustomMaterial = &hasCustomMat;
			rendererInfo.m_CustomMatID = &m_CustomMatID;

			rendererInfo.m_InfoBSize = &m_UnityMeshInfo.m_InfoSize;
			rendererInfo.m_InfoBHandler = &m_UnityMeshInfoPerViews[i].m_InfoHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_AtlasesBSize = &m_UnityMeshInfo.m_AtlasesSize;
			rendererInfo.m_AtlasesBHandler = &m_UnityMeshInfoPerViews[i].m_AtlasesHandler.m_Buffer->m_DeviceLocal;

			rendererInfo.m_IndirectArgsBHandler = &m_UnityMeshInfoPerViews[i].m_IndirectArgsHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_IndirectArgsParticleCountMultiplier = &m_UnityMeshInfo.m_IndirectArgsParticleCountMultiplier;

			rendererInfo.m_UseComputeBuffers = &useComputeBuffers;

			// We alloc the Unity mesh here:
			if (m_RendererType == Renderer_Decal)
			{
				SDecalRendererDesc desc;
				GetRendererInfo(desc);
				m_UnityMeshInfoPerViews[i].m_RendererGUID = ::OnSetupNewDecalRenderer(&desc, idx);
			}
			else
			{
				SPopcornRendererDesc desc;
				GetRendererInfo(desc);
				desc.m_CameraID = i;

				if (m_RendererType == Renderer_Billboard)
				{
					m_UnityMeshInfoPerViews[i].m_RendererGUID = ::OnSetupNewBillboardRenderer(&desc, idx);
				}
				else if (m_RendererType == Renderer_Ribbon)
				{
					m_UnityMeshInfoPerViews[i].m_RendererGUID = ::OnSetupNewRibbonRenderer(&desc, idx);
				}
				else if (m_RendererType == Renderer_Triangle)
				{
					m_UnityMeshInfoPerViews[i].m_RendererGUID = ::OnSetupNewTriangleRenderer(&desc, idx);
				}
			}

			PK_ASSERT(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1);
			if (m_UnityMeshInfoPerViews[i].m_RendererGUID < 0)
				return;

			::OnResizeRenderer(m_UnityMeshInfoPerViews[i].m_RendererGUID, 0, 0x100, 0x300);
			::OnRetrieveRendererBufferInfo(m_UnityMeshInfoPerViews[i].m_RendererGUID, &rendererInfo);
			m_HasCustomMat = hasCustomMat == ManagedBool_True ? true : false;

			if (gpuBillboarding && m_RendererType == Renderer_Billboard)
			{
				m_UnityMeshInfo.m_VertexStride = GetGeomBillboardVertexBufferStride(m_BillboardBR.m_Mode, m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags);
			}
			else
			{
				m_UnityMeshInfo.m_VertexStride = FillOffsetTableAndGetVertexBufferStride(m_UnityMeshInfo.m_SemanticOffsets, m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags, isEmissive3);
			}
			m_HasCustomMat = hasCustomMat == ManagedBool_True ? true : false;

			// Set the info if the handlers are compute buffers or not:
#if		defined(PK_ORBIS)

			((CGNMBufferHandles*)m_UnityMeshInfoPerViews[i].m_VBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CGNMBufferHandles*)m_UnityMeshInfoPerViews[i].m_IBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CGNMBufferHandles*)m_UnityMeshInfoPerViews[i].m_InfoHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CGNMBufferHandles*)m_UnityMeshInfoPerViews[i].m_AtlasesHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CGNMBufferHandles*)m_UnityMeshInfoPerViews[i].m_IndirectArgsHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
#elif	defined(PK_UNKNOWN2)

			((CUNKNOWN2BufferHandles*)m_UnityMeshInfoPerViews[i].m_VBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CUNKNOWN2BufferHandles*)m_UnityMeshInfoPerViews[i].m_IBHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CUNKNOWN2BufferHandles*)m_UnityMeshInfoPerViews[i].m_InfoHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CUNKNOWN2BufferHandles*)m_UnityMeshInfoPerViews[i].m_AtlasesHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
			((CUNKNOWN2BufferHandles*)m_UnityMeshInfoPerViews[i].m_IndirectArgsHandler.m_Buffer.Get())->m_IsComputeBuffer = useComputeBuffers;
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

			m_UnityMeshInfoPerViews[i].m_RendererGUID = ::OnSetupNewMeshRenderer(&desc, idx);
			if (m_UnityMeshInfoPerViews[i].m_RendererGUID < 0)
				return;

			const s32	meshLODsCount = ::OnGetMeshLODsCount(m_UnityMeshInfoPerViews[i].m_RendererGUID);

			if (!PK_VERIFY(m_PerLODMeshCount.Resize(meshLODsCount)))
				return;
			u32	totalMeshCount = 0;
			for (s32 lodIdx = 0; lodIdx < meshLODsCount; ++lodIdx)
			{
				s32		meshCount = ::OnGetMeshCount(m_UnityMeshInfoPerViews[i].m_RendererGUID, lodIdx);
				if (meshCount < 0)
					return;
				m_PerLODMeshCount[lodIdx] = static_cast<u32>(meshCount);
				totalMeshCount += m_PerLODMeshCount[lodIdx];
			}
			if (!PK_VERIFY(m_SubMeshBounds.Resize(totalMeshCount)))
				return;
			m_GlobalMeshBounds.Degenerate();
			for (u32 iMeshCount = 0; iMeshCount < totalMeshCount; ++iMeshCount)
			{
				CFloat3 bb;
				::OnGetMeshBounds(m_UnityMeshInfoPerViews[i].m_RendererGUID, iMeshCount, &bb);
				m_SubMeshBounds[iMeshCount] = bb;
				m_GlobalMeshBounds.Add(bb);
			}
			::OnRetrieveRendererBufferInfo(m_UnityMeshInfoPerViews[i].m_RendererGUID, &rendererInfo);
			m_HasCustomMat = hasCustomMat == ManagedBool_True ? true : false;
		}
	}
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataBillboard>(const CRendererDataBillboard *renderer)
{
	m_UID = renderer->m_Declaration.m_RendererUID;
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
	m_UID = renderer->m_Declaration.m_RendererUID;
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
	m_UID = renderer->m_Declaration.m_RendererUID;
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
	m_UID = renderer->m_Declaration.m_RendererUID;
	m_RendererType = renderer->m_RendererType;

	if (!PK_VERIFY(m_MaterialDescBillboard.InitFromRenderer(*renderer)) ||
		!PK_VERIFY(m_TriangleBR.Setup(renderer, null, SParticleStreamDef())))
		return false;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataLight>(const CRendererDataLight *renderer)
{
	m_UID = renderer->m_Declaration.m_RendererUID;
	//nothing material related to setup for lights
	m_RendererType = renderer->m_RendererType;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataSound>(const CRendererDataSound *renderer)
{
	m_UID = renderer->m_Declaration.m_RendererUID;
	if (!PK_VERIFY(m_MaterialDescSound.InitFromRenderer(*renderer)))
		return false;
	m_RendererType = renderer->m_RendererType;
	m_UID = renderer->m_Declaration.m_RendererUID;
	return true;
}

//----------------------------------------------------------------------------

template<>
bool	CUnityRendererCache::GameThread_SetupRenderer<CRendererDataDecal>(const CRendererDataDecal *renderer)
{
	m_UID = renderer->m_Declaration.m_RendererUID;
	m_RendererType = renderer->m_RendererType;

	if (!PK_VERIFY(m_MaterialDescDecal.InitFromRenderer(*renderer)))
		return false;

	m_RendererType = renderer->m_RendererType;
	return true;
}

//----------------------------------------------------------------------------
