//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_Data.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
//-------------------------------------------------------------------------------------

#define VK_NO_PROTOTYPES
#include "UnityGraphicsAPI/IUnityGraphicsVulkan.h"

#define UNITY_USED_VULKAN_API_FUNCTIONS(apply) \
	apply(vkFlushMappedMemoryRanges);

#define VULKAN_DEFINE_API_FUNCPTR(func) static PFN_##func func
VULKAN_DEFINE_API_FUNCPTR(vkGetInstanceProcAddr);
UNITY_USED_VULKAN_API_FUNCTIONS(VULKAN_DEFINE_API_FUNCPTR);
#undef VULKAN_DEFINE_API_FUNCPTR

static void LoadVulkanAPI(PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkInstance instance)
{
	if (!vkGetInstanceProcAddr && getInstanceProcAddr)
		vkGetInstanceProcAddr = getInstanceProcAddr;
#define LOAD_VULKAN_FUNC(fn) if (!fn) fn = (PFN_##fn)vkGetInstanceProcAddr(instance, #fn)
	UNITY_USED_VULKAN_API_FUNCTIONS(LOAD_VULKAN_FUNC);
#undef LOAD_VULKAN_FUNC
}

using namespace PopcornFX;

class	CVulkanData : public IRenderAPIData
{
public:
	CVulkanData();
	virtual	~CVulkanData();

	virtual void	BeginFrame();
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateVulkanData()
{
	return PK_NEW(CVulkanData);
}

//-------------------------------------------------------------------------------------

CVulkanData::CVulkanData()
	: IRenderAPIData()
{
	IUnityGraphicsVulkan *graphicVulkan = CRuntimeManager::Instance().GetUnityGraphicsVulkan();
	if (PK_VERIFY(graphicVulkan != null))
		return;
	UnityVulkanInstance		instance = graphicVulkan->Instance();
	LoadVulkanAPI(instance.getInstanceProcAddr, instance.instance);

	UnityVulkanPluginEventConfig config_1;
	config_1.graphicsQueueAccess = kUnityVulkanGraphicsQueueAccess_DontCare;
	config_1.renderPassPrecondition = kUnityVulkanRenderPass_EnsureInside;
	config_1.flags = kUnityVulkanEventConfigFlag_EnsurePreviousFrameSubmission | kUnityVulkanEventConfigFlag_ModifiesCommandBuffersState;
	graphicVulkan->ConfigureEvent(1, &config_1);
}

CVulkanData::~CVulkanData()
{

}

void	CVulkanData::BeginFrame()
{
}

void	*CVulkanData::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	(void)isIdxBuff; (void)fullSize; (void)mapSize;

	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());
	IUnityGraphicsVulkan *graphicVulkan = CRuntimeManager::Instance().GetUnityGraphicsVulkan();
	if (!PK_VERIFY(graphicVulkan != null))
		return null;

	UnityVulkanRecordingState recordingState;

	if (!graphicVulkan->CommandRecordingState(&recordingState, kUnityVulkanGraphicsQueueAccess_DontCare))
		return null;

	UnityVulkanBuffer bufferInfo;
	if (!graphicVulkan->AccessBuffer(bufferHandle.m_Buffer->m_DeviceLocal, 0, 0, kUnityVulkanResourceAccess_ObserveOnly, &bufferInfo))
		return null;

	if (!bufferInfo.memory.mapped)
		return null;

	// We don't want to start modifying a resource that might still be used by the GPU,
	// so we can use kUnityVulkanResourceAccess_Recreate to recreate it while still keeping the old one alive if it's in use.
	UnityVulkanBuffer recreatedBuffer;
	if (!graphicVulkan->AccessBuffer(bufferHandle.m_Buffer->m_DeviceLocal, VK_PIPELINE_STAGE_HOST_BIT, VK_ACCESS_HOST_WRITE_BIT, kUnityVulkanResourceAccess_Recreate, &recreatedBuffer))
		return null;

	return recreatedBuffer.memory.mapped;
}

//-------------------------------------------------------------------------------------

void	CVulkanData::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	(void)isIdxBuff;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());
	IUnityGraphicsVulkan *graphicVulkan = CRuntimeManager::Instance().GetUnityGraphicsVulkan();
	if (!PK_VERIFY(graphicVulkan != null))
		return;
	VkDevice			device = graphicVulkan->Instance().device;
	UnityVulkanBuffer	buffer;

	if (!graphicVulkan->AccessBuffer(bufferHandle.m_Buffer->m_DeviceLocal, 0, 0, kUnityVulkanResourceAccess_ObserveOnly, &buffer))
		return;

	if (!(buffer.memory.flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
	{
		VkMappedMemoryRange range;
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.pNext = null;
		range.memory = buffer.memory.memory;
		range.offset = buffer.memory.offset; // size and offset also must be multiple of nonCoherentAtomSize
		range.size = buffer.memory.size;
		vkFlushMappedMemoryRanges(device, 1, &range);
	}
}

//-------------------------------------------------------------------------------------
#endif // VULKAN_SUPPORT
