//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_render_helpers/include/frame_collector/legacy/rh_frame_collector_legacy.h>

class	IRenderAPIData;

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SUnityRenderContext
{
	IRenderAPIData	*m_RenderApiData;
	bool			m_FreeUnusedBatches;
	u32				m_FrameCountBeforeFreeingUnusedBatches;

	SUnityRenderContext()
	:	m_RenderApiData(null)
	,	m_FreeUnusedBatches(false)
	,	m_FrameCountBeforeFreeingUnusedBatches(0)
	{
	}
};

struct	SUnityDrawOutputs
{
};

struct	SViewUserData : public PopcornFX::SSceneView
{
	CGuid		m_CamSlotIdxInMedCol;
	CGuid		m_CamSlotIdxInMeshMedCol;
};

class	CUnityParticleBatchTypes
{
public:
	typedef SUnityRenderContext		CRenderContext;
	typedef SUnityDrawOutputs		CFrameOutputData;
	typedef SViewUserData			CViewUserData;

	enum { kMaxQueuedCollectedFrame = 2U };
};

// Frame collector:
typedef TFrameCollector<CUnityParticleBatchTypes> 	CUnityFrameCollector;

// Camera data with custom user data:
typedef SViewUserData				SUnitySceneView;

//----------------------------------------------------------------------------
__PK_API_END
