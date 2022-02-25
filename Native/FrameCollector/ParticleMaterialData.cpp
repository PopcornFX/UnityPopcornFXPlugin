//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "ParticleMaterialData.h"

#include "NativeToManaged.h"
#include "FrameCollector/FrameCollector.h"

#include <pk_kernel/include/kr_resources.h>
#include <pk_geometrics/include/ge_mesh_resource.h>

//----------------------------------------------------------------------------

CParticleMaterialDescFlags::CParticleMaterialDescFlags()
:	m_MaterialFlags(0)
,	m_UniformFlags(0)
{
}

//----------------------------------------------------------------------------

bool		CParticleMaterialDescFlags::HasMaterialFlags(u32 flags) const
{
	return (m_MaterialFlags & flags) == flags;
}

//----------------------------------------------------------------------------

bool		CParticleMaterialDescFlags::HasUniformFlags(u32 flags) const
{
	return (m_UniformFlags & flags) == flags;
}

//----------------------------------------------------------------------------

bool		CParticleMaterialDescFlags::NeedSort() const
{
	return !HasUniformFlags(UniformFlags::Is_Additive);
}

//----------------------------------------------------------------------------

bool		CParticleMaterialDescFlags::operator == (const CParticleMaterialDescFlags &oth) const
{
	return m_MaterialFlags == oth.m_MaterialFlags && m_UniformFlags == oth.m_UniformFlags;
}

//----------------------------------------------------------------------------

u32			CParticleMaterialDescFlags::CombineFlags()
{
	return (m_MaterialFlags << UniformFlags::UniformFlags_Count) | m_UniformFlags;
}

//----------------------------------------------------------------------------

CParticleMaterialDescBillboard::CParticleMaterialDescBillboard()
	: m_DrawOrder(0)
	, m_InvSoftnessDistance(1)
{
}

//----------------------------------------------------------------------------

void		CParticleMaterialDescBillboard::InitFromRenderer(const CParticleRenderer_Billboard &renderer, bool splitDoubleSidedMaterials)
{
	m_Flags.m_MaterialFlags = 0;
	m_Flags.m_MaterialFlags |= MaterialFlags::Has_Diffuse;
	m_Flags.m_MaterialFlags |= renderer.SoftAnimationBlending() ? MaterialFlags::Has_AnimBlend : 0;
	m_Flags.m_MaterialFlags |= !renderer.AlphaRemapper().Empty() ? MaterialFlags::Has_AlphaRemap : 0;
	m_Flags.m_MaterialFlags |= renderer.IsLit() ? MaterialFlags::Has_Lighting : 0;
	m_Flags.m_MaterialFlags |= renderer.IsSoft() ? MaterialFlags::Has_Soft : 0;
	m_Flags.m_MaterialFlags |= renderer.BillboardingMaterial() == CParticleRenderer_Billboard::AlphaBlend_Distortion ? MaterialFlags::Has_Distortion : 0;
	m_Flags.m_MaterialFlags |= !renderer.ColorField().Empty() ? MaterialFlags::Has_Color : 0;

	if (splitDoubleSidedMaterials)
	{
		m_Flags.m_MaterialFlags |= renderer.BillboardMode() == CParticleRenderer_Billboard::PlaneAligned ? MaterialFlags::Has_DoubleSided : 0;
	}

	m_Flags.m_UniformFlags = 0;

	if (renderer.BillboardingMaterial() == CParticleRenderer_Billboard::AlphaBlend_AdditiveSoft ||
		renderer.BillboardingMaterial() == CParticleRenderer_Billboard::AlphaBlend_AdditiveSoftLighting)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_AdditiveAlphaBlend;
	}
	else if (renderer.BillboardingMaterial() == CParticleRenderer_Billboard::Additive ||
		renderer.BillboardingMaterial() == CParticleRenderer_Billboard::Additive_Soft)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_Additive;
	}
	else if (renderer.BillboardingMaterial() == CParticleRenderer_Billboard::Additive_NoAlpha)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_AdditiveNoAlpha;
	}

	m_Diffuse = CStringId(renderer.Diffuse());
	m_AlphaRemap = CStringId(renderer.AlphaRemapper());
	m_UserData = CStringId(renderer.UserData());

	m_DrawOrder = renderer.DrawOrder();

	m_InvSoftnessDistance = 1.0f / renderer.SoftnessDistance();

	m_EffectNamePath = renderer.RawName();
}

void		CParticleMaterialDescBillboard::InitFromRenderer(const CParticleRenderer_Ribbon &renderer, bool splitDoubleSidedMaterials)
{
	m_Flags.m_MaterialFlags = 0;
	m_Flags.m_MaterialFlags |= MaterialFlags::Has_Diffuse;
	m_Flags.m_MaterialFlags |= renderer.Quality() == CParticleRenderer_Ribbon::CorrectDeformation ? MaterialFlags::Has_RibbonComplex : 0;
	m_Flags.m_MaterialFlags |= !renderer.AlphaRemapper().Empty() ? MaterialFlags::Has_AlphaRemap : 0;
	m_Flags.m_MaterialFlags |= renderer.IsLit() ? MaterialFlags::Has_Lighting : 0;
	m_Flags.m_MaterialFlags |= renderer.IsSoft() ? MaterialFlags::Has_Soft : 0;
	m_Flags.m_MaterialFlags |= renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::AlphaBlend_Distortion ? MaterialFlags::Has_Distortion : 0;
	m_Flags.m_MaterialFlags |= !renderer.ColorField().Empty() ? MaterialFlags::Has_Color : 0;

	if (splitDoubleSidedMaterials)
	{
		m_Flags.m_MaterialFlags |= renderer.BillboardMode() != CParticleRenderer_Ribbon::ViewposAligned ? MaterialFlags::Has_DoubleSided : 0;
	}

	m_Flags.m_UniformFlags = 0;

	if (renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::AlphaBlend_AdditiveSoft ||
		renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::AlphaBlend_AdditiveSoftLighting)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_AdditiveAlphaBlend;
	}
	else if (	renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::Additive ||
				renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::Additive_Soft)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_Additive;
	}
	else if (renderer.BillboardingMaterial() == CParticleRenderer_Ribbon::Additive_NoAlpha)
	{
		m_Flags.m_UniformFlags |= UniformFlags::Is_AdditiveNoAlpha;
	}
	m_Flags.m_UniformFlags |= renderer.RotateTexture() ? UniformFlags::Is_RotateTexture : 0;

	m_Diffuse = CStringId(renderer.Diffuse());
	m_AlphaRemap = CStringId(renderer.AlphaRemapper());
	m_UserData = CStringId(renderer.UserData());

	m_DrawOrder = renderer.DrawOrder();

	m_InvSoftnessDistance = 1.0f / renderer.SoftnessDistance();

	m_EffectNamePath = renderer.RawName();
}

bool		CParticleMaterialDescBillboard::operator == (const CParticleMaterialDescBillboard &oth) const
{
	return	m_Flags == oth.m_Flags &&
		m_Diffuse == oth.m_Diffuse &&
		m_AlphaRemap == oth.m_AlphaRemap &&
		m_UserData == oth.m_UserData &&
		m_DrawOrder == oth.m_DrawOrder &&
		m_InvSoftnessDistance == oth.m_InvSoftnessDistance;
}

//----------------------------------------------------------------------------

CParticleMaterialDescMesh::SMeshDescriptorData::SMeshDescriptorData()
	: m_RendererGUID(-1)
	, m_MeshInstanceBufferSize(0)
	, m_MeshInstanceBuffer(null)
{
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDescMesh::SMeshDescriptorData::InitFromRenderer(const CParticleRenderer_Mesh_Descriptor &meshDescriptor, const CString &userData, s32 drawOrder, const CString &effectName)
{
	if (meshDescriptor.Mesh() != null && !meshDescriptor.Mesh().Empty())
	{
		const bool														needUvs = true;// !uvs.Empty();
		const bool														needVertexColor = true;// !vertexColors.Empty();

		m_Flags.m_MaterialFlags = 0;
		m_Flags.m_MaterialFlags |= needVertexColor ? MaterialFlags::Has_Color : 0;
		m_Flags.m_MaterialFlags |= needUvs ? MaterialFlags::Has_Diffuse : 0;
		m_Flags.m_MaterialFlags |= meshDescriptor.CastShadow() ? MaterialFlags::Has_CastShadow : 0;

		if (meshDescriptor.Material() == CParticleRenderer_Mesh_Descriptor::Solid_Lighting)
			m_Flags.m_MaterialFlags |= MaterialFlags::Has_Lighting;
		else if (meshDescriptor.Material() == CParticleRenderer_Mesh_Descriptor::Distortion)
			m_Flags.m_MaterialFlags |= MaterialFlags::Has_Distortion;

		m_Flags.m_UniformFlags = 0;
		if (meshDescriptor.Material() == CParticleRenderer_Mesh_Descriptor::Additive)
			m_Flags.m_UniformFlags |= UniformFlags::Is_Additive;
		else if (meshDescriptor.Material() == CParticleRenderer_Mesh_Descriptor::Additive_NoAlpha)
			m_Flags.m_UniformFlags |= UniformFlags::Is_AdditiveNoAlpha;

		SMeshRendererDesc	desc;

		desc.m_SubMeshID = meshDescriptor.SubMeshId();
		desc.m_MeshAsset = meshDescriptor.Mesh().Data();
		desc.m_UserData = userData.Data();
		desc.m_EffectNames = effectName.Data();
		desc.m_MaterialFlags = m_Flags.m_MaterialFlags;
		desc.m_UniformFlags = m_Flags.m_UniformFlags;
		desc.m_DrawOrder = drawOrder;
		desc.m_DiffuseMap = meshDescriptor.Diffuse().Data();
		desc.m_NormalMap = meshDescriptor.Normal().Data();
		desc.m_SpecularMap = meshDescriptor.Specular().Data();

		if (!PK_VERIFY(_OnSetupNewMeshRenderer != null))
			return false;
		m_RendererGUID = _OnSetupNewMeshRenderer(&desc);
	}
	return true;
}

//----------------------------------------------------------------------------

CStringId	CParticleMaterialDescMesh::m_DiffuseColorStringId = CStringId::Null;

//----------------------------------------------------------------------------

void		CParticleMaterialDescMesh::InitFromRenderer(const CParticleRenderer_Mesh &renderer)
{
	if (m_DiffuseColorStringId == CStringId::Null)
	{
		m_DiffuseColorStringId = CStringId("DiffuseColor");
	}

	m_UserData = CStringId(renderer.UserData());
	m_EffectNamePath = renderer.RawName();
	m_DrawOrder = renderer.DrawOrder();
	const u32	descriptorCount = renderer.Meshes().Count();
	if (!m_MeshDescriptors.Resize(descriptorCount))
		return;
	for (u32 i = 0; i < descriptorCount; ++i)
	{
		const CParticleRenderer_Mesh_Descriptor	&descriptor = *renderer.Meshes()[i];

		if (!m_MeshDescriptors[i].InitFromRenderer(descriptor, m_UserData.ToString(), m_DrawOrder, m_EffectNamePath))
			return;
	}
}

//----------------------------------------------------------------------------

template<>
void		CParticleDrawer_Manager::GameThread_SetupRenderer<CParticleRenderer_Billboard>(const CParticleRenderer_Billboard *renderer, const CParticleMedium *)
{
	m_WasEmpty = true;
	m_MaterialDescBillboard.InitFromRenderer(*renderer, m_FrameCollector->GetSplitDoubleSidedMaterials());
	// Notify the frame collector that this material was preloaded so that it can pre-allocate the VBOs and IBOs
	m_FrameCollector->AddPreloadedBillboardMaterial(m_MaterialDescBillboard);
}

//----------------------------------------------------------------------------

template<>
void		CParticleDrawer_Manager::GameThread_SetupRenderer<CParticleRenderer_Ribbon>(const CParticleRenderer_Ribbon *renderer, const CParticleMedium *)
{
	m_WasEmpty = true;
	m_MaterialDescBillboard.InitFromRenderer(*renderer, m_FrameCollector->GetSplitDoubleSidedMaterials());
	// Notify the frame collector that this material was preloaded so that it can pre-allocate the VBOs and IBOs
	m_FrameCollector->AddPreloadedRibbonMaterial(m_MaterialDescBillboard);
}

//----------------------------------------------------------------------------

template<>
void		CParticleDrawer_Manager::GameThread_SetupRenderer<CParticleRenderer_Mesh>(const CParticleRenderer_Mesh *renderer, const CParticleMedium *hboMed)
{
	m_WasEmpty = true;
	m_MaterialDescMesh.InitFromRenderer(*renderer);
	// Notify the frame collector that this material was preloaded so that it can pre-allocate the VBOs and IBOs
	m_FrameCollector->AddPreloadedMeshMaterial(m_MaterialDescMesh);
}

//----------------------------------------------------------------------------
