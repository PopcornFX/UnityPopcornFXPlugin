//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "EditorOnly/BrowseEffect.h"

#include "EditorNativeToManaged.h"
#include "NativeToManaged.h"
#include "ResourceHandleDummy.h"
#include "PKUnity_FileSystemController.h"

#include <pk_particles/include/ps_descriptor_cache.h>

#include <pk_particles/include/ps_resources.h>
#include <pk_particles/include/ps_event_map.h>
#include <pk_particles/include/ps_descriptor.h>
#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_particles/include/ps_descriptor_cache.h>

#include <pk_particles/include/ps_samplers_classes.h>
#include <pk_particles/include/ps_samplers_shape.h>
#include <pk_particles/include/ps_samplers.h>
#include <pk_particles/include/ps_effect.h>
#include <pk_maths/include/pk_maths_transforms.h>
#include <pk_kernel/include/kr_units.h>
#include <pk_base_object/include/hbo_helpers.h>
#include <pk_particles_toolbox/include/pt_compile.h>	// for SEffectSourceCompilationHelper

#include <pk_render_helpers/include/batches/rh_ribbon_batch.h>
#include <pk_render_helpers/include/batches/rh_billboard_batch.h>
#include <pk_render_helpers/include/batches/rh_mesh_batch.h>

#include "RenderingIntegration/UnityRendererCache.h"

#include <pk_render_helpers/include/basic_renderer_properties/rh_basic_renderer_properties.h>
#include <pk_render_helpers/include/basic_renderer_properties/rh_vertex_animation_renderer_properties.h>
#include <pk_render_helpers/include/draw_requests/rh_billboard.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

// Check if the mirrored enum in the C# for the attribute types is up to date:
PK_STATIC_ASSERT(BaseType_Bool == 1);
PK_STATIC_ASSERT(BaseType_Bool2 == 2);
PK_STATIC_ASSERT(BaseType_Bool3 == 3);
PK_STATIC_ASSERT(BaseType_Bool4 == 4);
PK_STATIC_ASSERT(BaseType_I32 == 25);
PK_STATIC_ASSERT(BaseType_Int2 == 26);
PK_STATIC_ASSERT(BaseType_Int3 == 27);
PK_STATIC_ASSERT(BaseType_Int4 == 28);
PK_STATIC_ASSERT(BaseType_Float == 31);
PK_STATIC_ASSERT(BaseType_Float2 == 32);
PK_STATIC_ASSERT(BaseType_Float3 == 33);
PK_STATIC_ASSERT(BaseType_Float4 == 34);

//----------------------------------------------------------------------------
// Internal functions
//----------------------------------------------------------------------------

static CString	PatchPathForAnimTrack(const CString &path)
{
	CString		patchedPath;

	if (path.EndsWith(".pkmm", CaseInsensitive))
	{
		patchedPath = path.Extract(0, path.Length() - 5);
		patchedPath += ".pkan";
	}
	return patchedPath;
}

struct	SUnityDependencyAppendHelper
{
	TArray<SResourceDependency>		*m_OutListUnique;
	SUnityDependencyAppendHelper(TArray<SResourceDependency> &outList) : m_OutListUnique(&outList) {}

	bool	Append(const SResourceDependency &dependency)
	{
		u32		additionalUsageFlags = 0;

		if (dependency.m_Type == SResourceDependency::Type_Image)
		{
			// The usage will also depend on which renderer property this texture is used for:
			for (u32 i = 0; i < dependency.m_RendererContext.Count(); ++i)
			{
				const SRendererFeaturePropertyValue	*property = dependency.m_RendererContext[i].m_Property;
				if (property != null)
				{
					if (property->m_Name == BasicRendererProperties::SID_Atlas_MotionVectorsMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_Linear;
					else if (property->m_Name == BasicRendererProperties::SID_Distortion_DistortionMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_Linear;
					else if (property->m_Name == BasicRendererProperties::SID_LegacyLit_NormalMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_Linear;
					else if (property->m_Name == BasicRendererProperties::SID_Lit_NormalMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_Linear;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_PositionMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_NormalMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_ColorMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_PositionMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_NormalMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_ColorMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_PositionMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else if (property->m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_RotationMap())
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_VAT;
					else
						additionalUsageFlags |= SResourceDependency::UsageFlags_Image_sRGB;
				}
			}
		}
		for (u32 i = 0; i < m_OutListUnique->Count(); i++)
		{
			if ((*m_OutListUnique)[i] == dependency)
			{
				(*m_OutListUnique)[i].m_Usage |= (dependency.m_Usage | additionalUsageFlags);
				return true;
			}
		}
		if (PK_VERIFY(m_OutListUnique->PushBack(dependency).Valid()))
		{
			m_OutListUnique->Last().m_Usage |= additionalUsageFlags;
			return true;
		}
		return false;
	}
};

//----------------------------------------------------------------------------
// End Internal functions
//----------------------------------------------------------------------------

bool	LoadAndBrowseEffect(void *pkfxContentPtr, int contentByteSize, const char *path)
{
	CConstMemoryStream	memStream(pkfxContentPtr, contentByteSize);
	CStringView			pathView = CStringView(path, SNativeStringUtils::Length(path));
	PBaseObjectFile		file = HBO::g_Context->LoadFileFromStream_Pure(memStream, pathView, false);

	if (!PK_VERIFY(file != null))
	{
		CLog::Log(PK_ERROR, "Could not load the effect file for '%s'", path);
		return false;
	}

	PParticleEffect		effect = file->FindFirstOf<CParticleEffect>();

	if (!PK_VERIFY(effect != null))
	{
		CLog::Log(PK_ERROR, "Could not find the CParticleEffect object in file for '%s'", path);
		return false;
	}

	bool	ret = true;

	if (!BrowseEffect(effect))
		ret = false;

	effect = null;
	file->Unload();
	return ret;
}

bool	BrowseEffect(const PParticleEffect &particleEffect)
{
	bool	usesMeshRenderer = false;

	TArray<SResourceDependency>		dependencies;
	SUnityDependencyAppendHelper	dependenciesAppend(dependencies);

	particleEffect->LoadConnectionMap();
	if (particleEffect->EventConnectionMap() == null)
		return false;

	if (!PK_VERIFY(particleEffect->EventConnectionMap() != null) ||
		!PK_VERIFY(particleEffect->GatherRuntimeDependencies(FastDelegate<bool(const SResourceDependency &dependency)>(&dependenciesAppend, &SUnityDependencyAppendHelper::Append))))
	{
		CLog::Log(PK_WARN, "PopcornFX: Failed to gather dependencies of %s.", particleEffect->File()->Path().Data());
		return false;
	}
	if (!BrowseObjectForDependencies(dependencies, usesMeshRenderer))
		return false;

	if (!BrowseRenderers(particleEffect.Get()))
		return false;

	if (!BrowseExportedEvents(particleEffect.Get()))
		return false;

	if (PK_VERIFY(particleEffect != null))
	{
		const CParticleAttributeList	*allAttribList = particleEffect->AttributeFlatList();

		if (allAttribList != null)
		{
			CParticleAttributeList::_TypeOfAttributeList	attributeList = allAttribList->AttributeList();
			CParticleAttributeList::_TypeOfSamplerList		samplerList = allAttribList->SamplerList();
			const TMemoryView<const u32>					attributeRemapIDs = allAttribList->UniqueAttributeIDs();

			if (!BrowseAttributes(attributeList, attributeRemapIDs) ||
				!BrowseSamplers(samplerList))
				return false;
		}
	}
	::OnGetEffectInfo(usesMeshRenderer ? 1 : 0);
	return true;
}


//----------------------------------------------------------------------------

bool	BrowseObjectForDependencies(TArray<SResourceDependency> &dependencies, bool &usesMeshRenderer)
{
	const u32	dependencyCount = dependencies.Count();

	for (u32 i = 0; i < dependencyCount; ++i)
	{
		SResourceDependency	&dependency = dependencies[i];
		u32					dependencyMask = 0;
		CString				path = dependency.m_Path;

		if (dependency.m_Type == SResourceDependency::Type_Material)
			continue;

		if (dependency.m_Usage & SResourceDependency::UsageFlags_UsedInRender)
		{
			if (dependency.m_Type == SResourceDependency::Type_Mesh)
			{
				dependencyMask |= IsMeshRenderer;
				usesMeshRenderer = true;
			}
			else if (dependency.m_Type == SResourceDependency::Type_Image)
			{
				if (dependency.m_Usage & SResourceDependency::UsageFlags_Image_VAT)
				{
					dependencyMask |= IsVatTexture;
				}
				else if (dependency.m_Usage & SResourceDependency::UsageFlags_Image_Linear)
				{
					dependencyMask |= IsLinearTextureRenderer;
				}
				else
				{
					dependencyMask |= IsTextureRenderer;
				}
			}
		}
		else if (dependency.m_Usage & SResourceDependency::UsageFlags_UsedInSimulation)
		{
			if (dependency.m_Type == SResourceDependency::Type_Mesh &&
				(dependency.m_Usage & SResourceDependency::UsageFlags_Mesh_Sample))
			{
				dependencyMask |= IsMeshSampler;
			}
			else if (dependency.m_Type == SResourceDependency::Type_Image &&
					 ((dependency.m_Usage & SResourceDependency::UsageFlags_Image_Values) ||
					  (dependency.m_Usage & SResourceDependency::UsageFlags_Image_Density)))
			{
				dependencyMask |= IsTextureSampler;
			}
			else if (dependency.m_Type == SResourceDependency::Type_AnimTrack)
			{
				path = PatchPathForAnimTrack(path); // GOOD
			}
		}
		else //SResourceDependency::UsageFlags_Custom // Not supported
		{

		}
		::OnEffectDependencyFound(path.Data(), dependencyMask);
	}
	return true;
}

//----------------------------------------------------------------------------

bool	BrowseRenderers(CParticleEffect *particleEffect)
{
	if (!PK_VERIFY(particleEffect != null))
		return false;

	particleEffect->LoadConnectionMap();
	if (particleEffect->EventConnectionMap() == null)
	{
		PK_RELEASE_ASSERT_NOT_IMPLEMENTED();
		// NOTE(Julien): I don't know if this codepath happens in the Unity plugin. If it does, enable the compile:
#if 0//(PK_PARTICLES_BUILD_COMPILE_CACHE_SERIALIZER != 0) && (PK_COMPILER_BUILD_COMPILER != 0)
		ParticleToolbox::SEffectSourceCompilationHelper	compileHelper;
		compileHelper.Compile(effect.Get());
#endif
	}

	const PCEventConnectionMap eventConnectionMap = particleEffect->EventConnectionMap();
	if (!PK_VERIFY(eventConnectionMap != null))
		return false;
	u32		rendererCount = 0;

	for (const CEventConnectionMap::SLayerDefinition	&layerDef : eventConnectionMap->m_LayerSlots)
	{
		PParticleDescriptor	descriptor = layerDef.m_ParentDescriptor;
		auto	it = descriptor->Renderers().Begin();
		while (it != descriptor->Renderers().End())
		{
			const PRendererDataBase			&renderer = *it;
			CUnityRendererCache				dummyCache(null);

			if (renderer->m_RendererType == Renderer_Billboard)
			{
				SPopcornRendererDesc			unityMeshDesc;
				const CRendererDataBillboard	*dataBillboard = static_cast<const CRendererDataBillboard*>(renderer.Get());

				dummyCache.GameThread_SetupRenderer(dataBillboard);
				dummyCache.GetRendererInfo(unityMeshDesc);
				::OnEffectRendererFound(&unityMeshDesc, renderer->m_RendererType, rendererCount);
			}
			else if (renderer->m_RendererType == Renderer_Ribbon)
			{
				SPopcornRendererDesc			unityMeshDesc;
				const CRendererDataRibbon		*dataRibbon = static_cast<const CRendererDataRibbon*>(renderer.Get());

				dummyCache.GameThread_SetupRenderer(dataRibbon);
				dummyCache.GetRendererInfo(unityMeshDesc);
				::OnEffectRendererFound(&unityMeshDesc, renderer->m_RendererType, rendererCount);
			}
			else if (renderer->m_RendererType == Renderer_Mesh)
			{
				SMeshRendererDesc				unityMeshDesc;
				const CRendererDataMesh			*dataMesh = static_cast<const CRendererDataMesh*>(renderer.Get());

				dummyCache.GameThread_SetupRenderer(dataMesh);
				dummyCache.GetRendererInfo(unityMeshDesc);
				::OnEffectRendererFound(&unityMeshDesc, renderer->m_RendererType, rendererCount);
			}
			else if (renderer->m_RendererType == Renderer_Triangle)
			{
				SPopcornRendererDesc			unityTriangleDesc;
				const CRendererDataTriangle		*dataTriangle = static_cast<const CRendererDataTriangle*>(renderer.Get());

				dummyCache.GameThread_SetupRenderer(dataTriangle);
				dummyCache.GetRendererInfo(unityTriangleDesc);
				::OnEffectRendererFound(&unityTriangleDesc, renderer->m_RendererType, rendererCount);
			}

			++rendererCount;
			++it;
		}
	}
	return true;
}

bool	BrowseExportedEvents(CParticleEffect *particleEffect)
{
	// List all the exported events
	PCEventConnectionMap		connectionMap = particleEffect->EventConnectionMap();
	if (!PK_VERIFY(connectionMap != null))
		return false;
	if (connectionMap != null)
	{
		for (u32 evidx = 0; evidx < connectionMap->m_BroadSlots.Count(); evidx++)
		{
			const CEventConnectionMap::SBroadDefinition	&eventDef = connectionMap->m_BroadSlots[evidx];

			SFxEventDesc	eventDesc;

			eventDesc.m_Slot = evidx;
			eventDesc.m_EventName = eventDef.m_BroadName.ToStringData();
			::OnEffectEventFound(&eventDesc);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	BrowseAttributes(const CParticleAttributeList::_TypeOfAttributeList &attributeList, const TMemoryView<const u32> &remapIds)
{
	CFloat4			fMin(TNumericTraits<float>::kMin);
	CFloat4			fMax(TNumericTraits<float>::kMax);
	CUint4			uMin(TNumericTraits<u32>::kMin);
	CUint4			uMax(TNumericTraits<u32>::kMax);

	if (!PK_VERIFY(attributeList.Count() == remapIds.Count()))
	{
		CLog::Log(PK_WARN, "PopcornFX: Attributes are inconsistent. Fix them inside PopcornFX editor and try reimporting your FX.");
		return false;
	}
	for (u32 i = 0; i < attributeList.Count(); ++i)
	{
		u32										declId = remapIds[i]; // We register the attributes in the order they are in the attribute container
		const CParticleAttributeDeclaration		*attrib = attributeList[declId].Get();

		if (attrib != null)
		{
			SFxAttributeDesc	attribDesc;

			attribDesc.m_AttributeType = attrib->ExportedType();
			attribDesc.m_MinMaxFlag = 0x00;
			if (attrib->HasMin())
				attribDesc.m_MinMaxFlag |= SFxAttributeDesc::EAttrDescFlag::HasMin;
			if (attrib->HasMax())
				attribDesc.m_MinMaxFlag |= SFxAttributeDesc::EAttrDescFlag::HasMax;
			attribDesc.m_AttributeName = attrib->ExportedName().Data();
			attribDesc.m_Description = "";
			if (attribDesc.m_AttributeType == BaseType_Bool ||
				attribDesc.m_AttributeType == BaseType_Bool2 ||
				attribDesc.m_AttributeType == BaseType_Bool3 ||
				attribDesc.m_AttributeType == BaseType_Bool4)
			{
				attribDesc.m_DefaultValue.x() = *(float*)(&attrib->DefaultValueB4());
				attribDesc.m_DefaultValue.yzw() = CFloat3::ZERO;
			}
			else if (	attribDesc.m_AttributeType == BaseType_Float ||
						attribDesc.m_AttributeType == BaseType_Float2 ||
						attribDesc.m_AttributeType == BaseType_Float3 ||
						attribDesc.m_AttributeType == BaseType_Float4)
			{
				attribDesc.m_DefaultValue = attrib->DefaultValueF4();
				attribDesc.m_MinValue = attrib->HasMin() ? attrib->MinValueF4() : fMin;
				attribDesc.m_MaxValue = attrib->HasMax() ? attrib->MaxValueF4() : fMax;
			}
			else if (	attribDesc.m_AttributeType == BaseType_I32 ||
						attribDesc.m_AttributeType == BaseType_Int2 ||
						attribDesc.m_AttributeType == BaseType_Int3 ||
						attribDesc.m_AttributeType == BaseType_Int4)
			{
				attribDesc.m_DefaultValue = *(CFloat4*)(&attrib->DefaultValueI4());
				attribDesc.m_MinValue = attrib->HasMin() ? *(CFloat4*)(&attrib->MinValueI4()) : *(CFloat4*)(&uMin);
				attribDesc.m_MaxValue = attrib->HasMax() ? *(CFloat4*)(&attrib->MaxValueI4()) : *(CFloat4*)(&uMax);
			}
			else
			{
				CLog::Log(PK_WARN, "Unsupported attribute type : %d", attribDesc.m_AttributeType);
			}
			::OnEffectAttributeFound(&attribDesc);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	BrowseSamplers(const CParticleAttributeList::_TypeOfSamplerList &samplerList)
{
	for (u32 i = 0; i < samplerList.Count(); ++i)
	{
		const CParticleAttributeSamplerDeclaration		*sampler = samplerList[i].Get();

		if (sampler != null)
		{
			CResourceDescriptor	*samplerData = sampler->AttribSamplerDefaultValue();
			SFxSamplerDesc		samplerDesc;

			if (samplerData == null)
			{
				SParticleDeclaration::SSampler::ESamplerType	type = (SParticleDeclaration::SSampler::ESamplerType)sampler->ExportedType();

				switch (type)
				{
				case SParticleDeclaration::SSampler::ESamplerType::Sampler_Geometry:
					samplerDesc.m_SamplerType = SamplerShape;
					break;
				case SParticleDeclaration::SSampler::ESamplerType::Sampler_Image:
					samplerDesc.m_SamplerType = SamplerImage;
					break;
				case SParticleDeclaration::SSampler::ESamplerType::Sampler_Text:
					samplerDesc.m_SamplerType = SamplerText;
					break;
				case SParticleDeclaration::SSampler::ESamplerType::Sampler_Curve:
					samplerDesc.m_SamplerType = SamplerCurve;
					break;
				default:
					samplerDesc.m_SamplerType = SamplerUnsupported;
					break;
				}
				CLog::Log(PK_ERROR, "Attribute Sampler doesn't hold a default value, contact PopcornFX Team for more informations, as this shouldn't happen.");
			}
			else
			{
				CResourceDescriptor_Shape	*shapeData = HBO::Cast<CResourceDescriptor_Shape>(samplerData);
				CResourceDescriptor_Image	*imageData = HBO::Cast<CResourceDescriptor_Image>(samplerData);
				CResourceDescriptor_Text	*textData = HBO::Cast<CResourceDescriptor_Text>(samplerData);
				CResourceDescriptor_Curve	*curveData = HBO::Cast<CResourceDescriptor_Curve>(samplerData);

				if (shapeData != null)
				{
					samplerDesc.m_SamplerType = SamplerShape;
					samplerDesc.m_ShapeRotation = Transforms::Quaternion::FromEuler(Units::DegreesToRadians<CFloat3>(shapeData->Orientation()));
					samplerDesc.m_ShapePosition = shapeData->Transforms().StrippedTranslations();
				}
				else if (imageData != null)
					samplerDesc.m_SamplerType = SamplerImage;
				else if (textData != null)
					samplerDesc.m_SamplerType = SamplerText;
				else if (curveData != null)
				{
					samplerDesc.m_CurveDimension = curveData->Order();
					samplerDesc.m_CurveKeyCount = curveData->Times().Count();
					samplerDesc.m_CurveTimes = curveData->Times().RawDataPointer();
					samplerDesc.m_CurveFloatTangents = curveData->FloatTangents().RawDataPointer();
					samplerDesc.m_CurveFloatValues = curveData->FloatValues().RawDataPointer();
					samplerDesc.m_SamplerType = SamplerCurve;
				}
				else
					samplerDesc.m_SamplerType = SamplerUnsupported;
			}
			samplerDesc.m_SamplerUsageFlags = sampler->UsageFlags();
			samplerDesc.m_SamplerName = sampler->ExportedName().Data();
			// Handle wide-char strings here
			samplerDesc.m_Description = ""; //  sampler->Description().MapDefault().Data();

			::OnEffectSamplerFound(&samplerDesc);
		}
	}
	return true;
}

//----------------------------------------------------------------------------
__PK_API_END
