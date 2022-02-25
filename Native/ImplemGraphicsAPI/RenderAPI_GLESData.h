//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

//-------------------------------------------------------------------------------------

#if	(PK_BUILD_WITH_GLES_SUPPORT != 0)

class	CGLESBufferHandles : CBufferAbstract
{
public:
	void	*m_GLESHostVisibleBuffer;
	s32		m_GLESHostVisibleSize;
	s32		m_GLESLastMappedSize;

	void	InitGLES()
	{
		m_GLESHostVisibleBuffer = null;
		m_GLESHostVisibleSize = -1;
		m_GLESLastMappedSize = -1;
	}

	void	DestroyGLES()
	{
		if (m_GLESHostVisibleBuffer != null)
		{
			PK_FREE(m_GLESHostVisibleBuffer);
			m_GLESHostVisibleBuffer = null;
			m_GLESHostVisibleSize = -1;
		}
	}

	CGLESBufferHandles()
	{
		InitGLES();
	}

	~CGLESBufferHandles() override
	{
		DestroyGLES();
	}
};

#endif	// (PK_BUILD_WITH_GLES_SUPPORT != 0)

//-------------------------------------------------------------------------------------
