//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)

#include "UnityGraphicsAPI/IUnityGraphicsVulkan.h"

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
