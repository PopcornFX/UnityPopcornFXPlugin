//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_render_helpers/include/frame_collector/rh_frame_collector.h>

class	IRenderAPIData;

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SUnityRenderContext
{
	IRenderAPIData	*m_RenderApiData;
};

struct	SUnityDrawOutputs
{
};

struct	SViewUserData
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
typedef TSceneView<SViewUserData>					SSceneView;

//----------------------------------------------------------------------------
__PK_API_END
