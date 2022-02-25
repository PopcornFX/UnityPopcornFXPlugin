//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_refcounted_buffer.h>

#include <pk_drawers/include/dr_billboard.h>
#include <pk_drawers/include/dr_billboard_cpu.h>
#include <pk_drawers/include/dr_ribbon.h>
#include <pk_drawers/include/dr_ribbon_cpu.h>

#include "PKUnity_Tasks.h"
#include "FrameCollector/ParticleMaterialData.h"
#include "ParticleBaseDrawCall.h"

using namespace		PopcornFX;

class	CFrameCollector;

//----------------------------------------------------------------------------

class CParticleBillboardDrawCall : public CParticleBaseDrawCall
{
public:
	CParticleBillboardDrawCall();
	virtual ~CParticleBillboardDrawCall();

	void			Setup(CFrameCollector *renderManager, const CParticleMaterialDescBillboard &materialDesc);
	bool			CanRender(const CParticleMaterialDescBillboard &materialDesc) const;
	void			AddToRender(const Drawers::SBillboard_DrawRequest &inDrawRequest, const CParticleMaterialDescBillboard &materialDesc);
	void			LaunchBillboarding(CRendererSubView &views);
	void			FinishBillboarding(CRendererSubView &views);

private:
	TArray<Drawers::SBillboard_DrawRequest>		m_DrawRequests;

	Drawers::CBillboard_CPU						m_BB;

	Drawers::CBillboard_Exec_Indices			m_Exec_Indices;
	Drawers::CBillboard_Exec_PositionsNormals	m_Exec_PNT;
	Drawers::CBillboard_Exec_Colors				m_Exec_Colors;
	Drawers::CBillboard_Exec_Texcoords			m_Exec_Texcoords;
	Drawers::CBillboard_Exec_AlphaCursor		m_Exec_AlphaCursors;

	u32											m_SortWorkingBufferSize;
	void										*m_SortWorkingBuffer;
};

//----------------------------------------------------------------------------
