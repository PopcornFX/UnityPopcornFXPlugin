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

class CParticleRibbonDrawCall : public CParticleBaseDrawCall
{
public:
	CParticleRibbonDrawCall();
	virtual ~CParticleRibbonDrawCall();

	void			Setup(CFrameCollector *renderManager, const CParticleMaterialDescBillboard &materialDesc);
	bool			CanRender(const CParticleMaterialDescBillboard &materialDesc) const;
	void			AddToRender(const Drawers::SRibbon_DrawRequest &inDrawRequest, const CParticleMaterialDescBillboard &materialDesc);
	void			LaunchBillboarding(CRendererSubView &views);
	void			FinishBillboarding(CRendererSubView &views);

private:
	TArray<Drawers::SRibbon_DrawRequest>		m_DrawRequests;

	Drawers::CRibbon_CPU						m_BB;

	Drawers::CRibbon_Exec_FillSortIndices		m_Exec_Indices;
	Drawers::CRibbon_Exec_Positions				m_Exec_Positions;
	Drawers::CRibbon_Exec_Colors				m_Exec_Colors;
	Drawers::CRibbon_Exec_Texcoords				m_Exec_Texcoords;
	Drawers::CRibbon_Exec_UVRemap				m_Exec_TexcoordsScaleAndOffsets;
	Drawers::CRibbon_Exec_AlphaCursor			m_Exec_AlphaCursors;

	// currently unused?
	PPooledMallocBuffer							m_SortWorkingBuffer;
};

//----------------------------------------------------------------------------
