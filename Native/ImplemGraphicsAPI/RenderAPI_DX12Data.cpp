//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_DX12Data.h"
#include "RuntimeManager.h"


#if (PK_BUILD_WITH_D3D12_SUPPORT != 0)

#include "UnityGraphicsAPI/IUnityGraphicsD3D12.h"
//----------------------------------------------------------------------------

#if defined(PK_DURANGO) || defined(PK_UNKNOWN1)
#define	PK_UNITY_IID_PPV_ARGS(ppType) IID_GRAPHICS_PPV_ARGS(ppType)
#else
#define	PK_UNITY_IID_PPV_ARGS(ppType) IID_PPV_ARGS(ppType)
#endif

//----------------------------------------------------------------------------

namespace
{
	static const u32	kBufferCount = SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER;
}

//----------------------------------------------------------------------------

using namespace PopcornFX;

extern IRenderAPIData	*CreateDX12Data();

class CDX12Data : public IRenderAPIData
{
public:
	CDX12Data()
	:	m_CurrentBuffIdx(0)
	{
	}
	~CDX12Data()
	{
		for (u32 iBuffer = 0; iBuffer < kBufferCount; ++iBuffer)
		{
			if (m_CommandList[iBuffer] != null)
				m_CommandList[iBuffer]->Release();

			if (m_CommandAllocator[iBuffer] != null)
				m_CommandAllocator[iBuffer]->Release();
		}

		if (m_CommandQueue != null)
			m_CommandQueue->Release();
	}

	bool			Init();

	virtual void	BeginFrame() override;
	virtual void	EndFrame() override;
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize) override;
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff) override;

private:
	u32							m_CurrentBuffIdx;

	ID3D12CommandAllocator		*m_CommandAllocator[kBufferCount];
	ID3D12GraphicsCommandList	*m_CommandList[kBufferCount];
	ID3D12CommandQueue			*m_CommandQueue;

	bool						m_NeedCommandReset[kBufferCount];
};

//----------------------------------------------------------------------------

IRenderAPIData	*CreateDX12Data()
{
	CDX12Data	*dx12Data = PK_NEW(CDX12Data);
	PK_ASSERT(dx12Data);

	if (!dx12Data->Init())
		PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Initialization failed");

	return dx12Data;
}

//----------------------------------------------------------------------------

bool	CDX12Data::Init()
{
	ID3D12Device	*device = CRuntimeManager::Instance().GetDeviceD3D12();
	if (device == null)
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed to retrieve device");
		return false;
	}

	for (u32 iBuffer = 0; iBuffer < kBufferCount; ++iBuffer)
	{
		HRESULT			result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, PK_UNITY_IID_PPV_ARGS(&m_CommandAllocator[iBuffer]));
		if (FAILED(result))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command allocator creation");
			return false;
		}

		result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, m_CommandAllocator[iBuffer], null, PK_UNITY_IID_PPV_ARGS(&m_CommandList[iBuffer]));
		if (FAILED(result))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command list creation");
			return false;
		}

		m_CommandList[iBuffer]->Close();
		m_NeedCommandReset[iBuffer] = true;
	}

	D3D12_COMMAND_QUEUE_DESC	queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH; // allow to avoid usage of fence
	HRESULT						result = device->CreateCommandQueue(&queueDesc, PK_UNITY_IID_PPV_ARGS(&m_CommandQueue));
	if (FAILED(result))
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command queue creation");
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------

void	CDX12Data::BeginFrame()
{
	m_CurrentBuffIdx = (m_CurrentBuffIdx + 1) % SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER;
}

//----------------------------------------------------------------------------

void	CDX12Data::EndFrame()
{
	if (!m_NeedCommandReset[m_CurrentBuffIdx])
	{
		HRESULT	result = m_CommandList[m_CurrentBuffIdx]->Close();
		m_NeedCommandReset[m_CurrentBuffIdx] = true;
		if (FAILED(result))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command list close");
			return;
		}

		m_CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&m_CommandList[m_CurrentBuffIdx]));
	}
}

//----------------------------------------------------------------------------

void	*CDX12Data::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	(void)mapSize; (void)isIdxBuff;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	ID3D12Resource			*deviceLocalBuff = (ID3D12Resource*)bufferHandle.m_Buffer->m_DeviceLocal;

	D3D12_HEAP_PROPERTIES	heapProperties;
	D3D12_HEAP_FLAGS		heapFlags;
	HRESULT					result = deviceLocalBuff->GetHeapProperties(&heapProperties, &heapFlags);
	if (FAILED(result))
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed to retrieve heap properties");
		return null;
	}

	void	*mapped;
	if (heapProperties.Type == D3D12_HEAP_TYPE_UPLOAD)
	{
		D3D12_RANGE		readRange = { };
		readRange.Begin = 0;
		readRange.End = 0;
		result = deviceLocalBuff->Map(0, &readRange, reinterpret_cast<void**>(&mapped));
		if (FAILED(result))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on D3D12 buffer mapping");
			return null;
		}
	}
	else
	{
		CDX12BufferHandles		*bufferHandleDX12 = static_cast<CDX12BufferHandles*>(bufferHandle.m_Buffer.Get());

		// Resize the current host visible buffer:
		if (bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx] == null ||
			bufferHandleDX12->m_D3D12SizeHostVisible[m_CurrentBuffIdx] < s32(fullSize))
		{
			if (bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx] != null)
			{
				bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx]->Release();
				bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx] = null;
			}

			ID3D12Device	*device = CRuntimeManager::Instance().GetDeviceD3D12();
			PK_ASSERT(device != null);

			D3D12_HEAP_PROPERTIES	bufferHeapProperties = { };
			bufferHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
			bufferHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			bufferHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			bufferHeapProperties.CreationNodeMask = heapProperties.CreationNodeMask;
			bufferHeapProperties.VisibleNodeMask = heapProperties.VisibleNodeMask;

			D3D12_RESOURCE_DESC		bufferDesc = { };
			bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			bufferDesc.Alignment = 0;
			bufferDesc.Width = fullSize;
			bufferDesc.Height = 1;
			bufferDesc.DepthOrArraySize = 1;
			bufferDesc.MipLevels = 1;
			bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
			bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			bufferDesc.SampleDesc.Count = 1;
			bufferDesc.SampleDesc.Quality = 0;

			ID3D12Resource	*buffer;
			result = device->CreateCommittedResource(&bufferHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, null, PK_UNITY_IID_PPV_ARGS(&buffer));
			if (FAILED(result))
			{
				PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on buffer creation");
				return null;
			}

			bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx] = buffer;
			bufferHandleDX12->m_D3D12SizeHostVisible[m_CurrentBuffIdx] = fullSize;
		}

		ID3D12Resource	*hostVisibleBuff = bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx];
		if (hostVisibleBuff == null)
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Null host buffer");
			return null;
		}

		D3D12_RANGE		readRange = { };
		readRange.Begin = 0;
		readRange.End = 0;
		result = hostVisibleBuff->Map(0, &readRange, reinterpret_cast<void**>(&mapped));
		if (FAILED(result))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on D3D12 buffer mapping");
			return null;
		}
	}

	PK_ASSERT(mapped != null);
	return mapped;
}

//----------------------------------------------------------------------------

void	CDX12Data::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	(void)isIdxBuff;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	ID3D12Resource			*deviceLocalBuff = (ID3D12Resource*)bufferHandle.m_Buffer->m_DeviceLocal;

	D3D12_HEAP_PROPERTIES	heapProperties;
	D3D12_HEAP_FLAGS		heapFlags;
	HRESULT					result = deviceLocalBuff->GetHeapProperties(&heapProperties, &heapFlags);
	if (FAILED(result))
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed to retrieve heap properties");
		return;
	}

	if ((heapProperties.Type & D3D12_HEAP_TYPE_UPLOAD) != 0)
	{
		D3D12_RANGE		writeRange = { };
		writeRange.Begin = 0;
		writeRange.End = 0;
		deviceLocalBuff->Unmap(0, &writeRange);
	}
	else
	{
		CDX12BufferHandles	*bufferHandleDX12 = static_cast<CDX12BufferHandles*>(bufferHandle.m_Buffer.Get());
		ID3D12Resource		*hostVisibleBuff = bufferHandleDX12->m_D3D12HostVisible[m_CurrentBuffIdx];

		if (hostVisibleBuff == null)
			return;

		D3D12_RANGE			writeRange = { };
		writeRange.Begin = 0;
		writeRange.End = 0;
		hostVisibleBuff->Unmap(0, &writeRange);

		ID3D12Device	*device = CRuntimeManager::Instance().GetDeviceD3D12();
		PK_ASSERT(device != null);

		if (m_NeedCommandReset[m_CurrentBuffIdx])
		{
			result = m_CommandAllocator[m_CurrentBuffIdx]->Reset();
			if (FAILED(result))
			{
				PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command allocator reset");
				return;
			}

			result = m_CommandList[m_CurrentBuffIdx]->Reset(m_CommandAllocator[m_CurrentBuffIdx], null);
			if (FAILED(result))
			{
				PK_ASSERT_NOT_REACHED_MESSAGE("RenderAPI: Failed on command list reset");
				return;
			}

			m_NeedCommandReset[m_CurrentBuffIdx] = false;
		}

		m_CommandList[m_CurrentBuffIdx]->CopyResource(deviceLocalBuff, hostVisibleBuff);
	}
}

//----------------------------------------------------------------------------
#endif // PK_BUILD_WITH_D3D12_SUPPORT
