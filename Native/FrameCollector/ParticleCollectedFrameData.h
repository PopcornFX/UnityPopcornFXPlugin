//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_containers_array.h>

#include <pk_drawers/include/dr_billboard.h>
#include <pk_drawers/include/dr_ribbon.h>
#include <pk_drawers/include/dr_mesh.h>

#include "ParticleMaterialData.h"

using namespace		PopcornFX;

/*
One frame "standalone" particle data to be
collected on the GameThread
and send to the RenderThread to be rendered
*/
struct SParticleCollectedFrameToRender
{
	bool				m_Rendered;
	u32					m_CollectedFrameId;

	u32					m_TotalDrawRequestsCount;

	// one to one correspondence between m_*DrawRequests and m_*MaterialDescs
	Drawers::CBillboard_DrawRequests			m_BillboardDrawRequests;
	TArray<CParticleMaterialDescBillboard>		m_BillboardMaterialDescs;

	Drawers::CRibbon_DrawRequests				m_RibbonDrawRequests;
	TArray<CParticleMaterialDescBillboard>		m_RibbonMaterialDescs;

	~SParticleCollectedFrameToRender()
	{
		Clear();
	}

	bool		Empty() const
	{
		return m_TotalDrawRequestsCount == 0;
	}

	void		Clear()
	{
		m_Rendered = false;
		m_TotalDrawRequestsCount = 0;

		m_BillboardDrawRequests.Clear();
		m_BillboardMaterialDescs.Clear();

		m_RibbonDrawRequests.Clear();
		m_RibbonMaterialDescs.Clear();
	}
};
