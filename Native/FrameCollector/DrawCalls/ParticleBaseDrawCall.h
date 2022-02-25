//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "PKUnity_Tasks.h"
#include "FrameCollector/ParticleMaterialData.h"
#include "NativeToManaged.h"

#include <pk_kernel/include/kr_refcounted_buffer.h>

#include <pk_drawers/include/dr_billboard.h>
#include <pk_drawers/include/dr_billboard_cpu.h>
#include <pk_drawers/include/dr_ribbon.h>
#include <pk_drawers/include/dr_ribbon_cpu.h>
#include <pk_drawers/include/dr_mesh.h>
#include <pk_drawers/include/dr_renderers_walker.h>

using namespace		PopcornFX;

// TODO : implement an actual pool!
typedef CRefCountedMemoryBuffer		CPooledMallocBuffer;
typedef PRefCountedMemoryBuffer		PPooledMallocBuffer;

class	CFrameCollector;

//----------------------------------------------------------------------------

class CParticleBaseDrawCall
{
public:
	CParticleBaseDrawCall();
	virtual ~CParticleBaseDrawCall();

	void			Clear();

	void			GameThread_UpdateVertexAndIndexCount(const Drawers::SBillboard_DrawRequest &dr);
	void			GameThread_UpdateVertexAndIndexCount(const Drawers::SRibbon_DrawRequest &dr);
	void			GameThread_UpdateVertexAndIndexCount(const Drawers::SMesh_DrawRequest &dr);

	void			GameThread_CreateUnityRenderers(bool delayCallbacks);
	void			GameThread_AddRendererNameToUnity(const CString &rendererName);
	void			GameThread_ResizeUnityRenderers(bool delayUnityCallbacks, bool hasBounds);

	bool			RenderThread_AllocBuffers(u32 vertexCount, u32 indexCount);
	void			RenderThread_CleanUnusedIndices();

protected:
	void			RenderThread_FillIndexBuffer();

	bool							m_UnityCallbacksDelayed; // If one of the unity callback has been delayed we skip the rendering

	// Those are used to resize the draw call VBOs and IBOs on the game thread:
	u32								m_GameThreadVertexCount;
	u32								m_GameThreadIndexCount;
	CAABB							m_GameThreadBBox;

	Drawers::ERendererClass			m_RendererType;

	CFrameCollector					*m_FrameCollector;

	CParticleMaterialDescBillboard	m_MaterialDesc;

	bool							m_DrawCallReady;

	SParticleBuffers				m_ParticleBuffers;

	u32								m_StartCleanIndexByteOffset;
	u32								m_CleanIndexByteCount;

	ManagedBool						m_LargeIndices;				// The final idx buffer uses large indices

	u32								m_VertexCount;
	u32								m_IndexCount;

	void			VertexBufferSAO2AOSPrepareCopy();
};

//----------------------------------------------------------------------------
