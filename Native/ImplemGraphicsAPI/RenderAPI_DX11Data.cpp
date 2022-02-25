//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_DX11Data.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_D3D11_SUPPORT != 0)
//-------------------------------------------------------------------------------------

using namespace PopcornFX;

extern IRenderAPIData*	CreateDX11Data();

class	CDX11Data : public IRenderAPIData
{
public:
	CDX11Data()
		:	m_CurrentBuffIdx(0)
	{
	}

	virtual void	BeginFrame();
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);

private:
	u32				m_CurrentBuffIdx;
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateDX11Data()
{
	return PK_NEW(CDX11Data);
}

//-------------------------------------------------------------------------------------
#include <comdef.h>

#ifdef PK_DURANGO
typedef		ID3D11DeviceX*			PD3D11Device;
typedef		ID3D11DeviceContextX*	PD3D11Context;
#	define	GET_IMMEDIATE_CONTEXT(_device, _ctx, _error)	_device->GetImmediateContextX(&_ctx); if (_ctx == null) { PK_ASSERT_NOT_REACHED_MESSAGE("GetImmediateContext returned NULL"); _error; }
#else
typedef		ID3D11Device*			PD3D11Device;
typedef		ID3D11DeviceContext*	PD3D11Context;
#	define	GET_IMMEDIATE_CONTEXT(_device, _ctx, _error)	_device->GetImmediateContext(&_ctx); if (_ctx == null) { PK_ASSERT_NOT_REACHED_MESSAGE("GetImmediateContext returned NULL"); _error; }
#endif

void	CDX11Data::BeginFrame()
{
	m_CurrentBuffIdx = (m_CurrentBuffIdx + 1) % SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER;
}

void	*CDX11Data::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	(void)isIdxBuff; (void)fullSize; (void)mapSize;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());
	D3D11_MAPPED_SUBRESOURCE	mapped;

	CDX11BufferHandles			*bufferHandleDX11 = (CDX11BufferHandles*)(bufferHandle.m_Buffer.Get());
	ID3D11Buffer				*deviceLocalBuff = (ID3D11Buffer*)bufferHandleDX11->m_DeviceLocal;
	PD3D11Device				device = CRuntimeManager::Instance().GetDeviceD3D11();
	PD3D11Context				immediateCtx = null;

	GET_IMMEDIATE_CONTEXT(device, immediateCtx, return null);
	D3D11_BUFFER_DESC	deviceLocalDesc = {};
	deviceLocalBuff->GetDesc(&deviceLocalDesc);
	if ((deviceLocalDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) != 0)
	{
		immediateCtx->Map(deviceLocalBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	}
	else
	{
		int		idx = m_CurrentBuffIdx;


		// Resize the current host visible buffer:
		if (bufferHandleDX11->m_D3D11HostVisible[idx] == null ||
			bufferHandleDX11->m_D3D11SizeHostVisible[idx] < s32(fullSize))
		{
			if (bufferHandleDX11->m_D3D11HostVisible[idx] != null)
			{
				bufferHandleDX11->m_D3D11HostVisible[idx]->Release();
				bufferHandleDX11->m_D3D11HostVisible[idx] = null;
			}

			D3D11_BUFFER_DESC	desc = {};
			ID3D11Buffer		*buffer;
			desc.ByteWidth = fullSize;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = isIdxBuff ? D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER : D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			HRESULT hr = device->CreateBuffer(&desc, NULL, &buffer);
			if (FAILED(hr))
			{
				return null;
			}
			bufferHandleDX11->m_D3D11HostVisible[idx] = buffer;
			bufferHandleDX11->m_D3D11SizeHostVisible[idx] = fullSize;
		}
		ID3D11Buffer	*hostVisibleBuff = bufferHandleDX11->m_D3D11HostVisible[idx];
		HRESULT			hr = immediateCtx->Map(hostVisibleBuff, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped);
		if (FAILED(hr))
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Map on D3D11 buffer failed");
			return null;
		}
	}
	return mapped.pData;
}

//-------------------------------------------------------------------------------------

void	CDX11Data::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	(void)isIdxBuff;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	int							idx = m_CurrentBuffIdx;
	CDX11BufferHandles			*bufferHandleDX11 = (CDX11BufferHandles*)(bufferHandle.m_Buffer.Get());
	ID3D11Buffer				*deviceLocalBuff = (ID3D11Buffer*)bufferHandleDX11->m_DeviceLocal;
	PD3D11Device				device = CRuntimeManager::Instance().GetDeviceD3D11();
	PD3D11Context				immediateCtx = null;

	GET_IMMEDIATE_CONTEXT(device, immediateCtx, return);
	D3D11_BUFFER_DESC	deviceLocalDesc = {};
	deviceLocalBuff->GetDesc(&deviceLocalDesc);
	if ((deviceLocalDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) != 0)
	{
		immediateCtx->Unmap(deviceLocalBuff, 0);
	}
	else
	{
		ID3D11Buffer			*hostVisible = bufferHandleDX11->m_D3D11HostVisible[idx];

		immediateCtx->Unmap(hostVisible, 0);
		immediateCtx->CopyResource((ID3D11Buffer*)bufferHandleDX11->m_DeviceLocal, hostVisible);
	}
}

//-------------------------------------------------------------------------------------
#endif // D3D11_SUPPORT
