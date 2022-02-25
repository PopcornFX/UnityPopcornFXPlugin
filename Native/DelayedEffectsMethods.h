//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "ManagedToNative.h"

__PK_API_BEGIN

// Managed to native delayed callbacks:

struct	SPreloadFx
{
	CString		m_Path;
	bool		m_UsesMeshRenderer;
};

struct	SStartFx
{
	int			m_Guid;
	float		m_Delay;
	float		m_Prewarm;
};

struct	STerminateFx
{
	int			m_Guid;
};

struct	SStopFx
{
	int			m_Guid;
};

struct	SKillFx
{
	int			m_Guid;
};

struct	SOnFxStopped
{
	int			m_Guid;

	bool	operator==(const SOnFxStopped &oth) const { return m_Guid == oth.m_Guid; }
};

struct	STransformAllParticles
{
	CFloat4x4	m_Transform;
};

struct	SUpdateCamDesc
{
	int			m_CamID;
	SCamDesc	m_Desc;
	bool		m_Update;
};

// Native to managed delayed callbacks:

struct SOnSetupNewBillboardRenderer
{
	SPopcornRendererDesc	m_RendererDesc;
	int						*m_ReturnValue;
};

struct SOnSetupNewRibbonRenderer
{
	SPopcornRendererDesc	m_RendererDesc;
	int						*m_ReturnValue;
};

struct SOnResizeRenderer
{
	int						m_RendererGUID;
	int						m_ParticleCount;
	int						m_ReservedVertexCount;
	int						m_ReservedIndexCount;
	SRetrieveRendererInfo	m_BufferInfo;
	bool					*m_DelayedResult;
};

struct SOnSetParticleCount
{
	int				m_RendererGUID;
	int				m_ParticleCount;
};

struct SOnSetRendererActive
{
	int				m_RendererGUID;
	ManagedBool		m_Active;
};

struct SOnUpdateRendererBounds
{
	int						m_RendererGUID;
	SUpdateRendererBounds	m_Bounds;
};

__PK_API_END
