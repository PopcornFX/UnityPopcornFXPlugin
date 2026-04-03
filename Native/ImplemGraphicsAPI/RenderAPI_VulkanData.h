//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)

//-------------------------------------------------------------------------------------

class	CVulkanBufferHandles : public CBufferAbstract
{
public:
	CVulkanBufferHandles()
	{
	}

	~CVulkanBufferHandles() override
	{
	}
};

#endif 	// (PK_BUILD_WITH_VULKAN_SUPPORT != 0)

//-------------------------------------------------------------------------------------
