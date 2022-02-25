//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

//-------------------------------------------------------------------------------------

#if	(PK_BUILD_WITH_METAL_SUPPORT != 0)

class	CMetalBufferHandles : public CBufferAbstract
{
public:
	u32 	m_MappedSize;

	void	InitMetal()
	{
		m_MappedSize = 0;
	}

	CMetalBufferHandles()
	{
		InitMetal();
	}

	~CMetalBufferHandles() override {}
};

#endif	// (PK_BUILD_WITH_METAL_SUPPORT != 0)

//-------------------------------------------------------------------------------------
