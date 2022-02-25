//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

#if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)

#if	defined(PK_DURANGO)
#	include <d3d12_x.h>
#elif defined(PK_UNKNOWN1)
#	include <d3d12_xs.h>
#else
#	include <d3d12.h>
#endif // defined(PK_DURANGO)

//-------------------------------------------------------------------------------------

class	CDX12BufferHandles : public CBufferAbstract
{
public:
	ID3D12Resource	*m_D3D12HostVisible[SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER];
	s32				m_D3D12SizeHostVisible[SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER];

	void	InitD3D12()
	{
		for (u32 iBuffer = 0; iBuffer < SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER; ++iBuffer)
		{
			m_D3D12HostVisible[iBuffer] = null;
			m_D3D12SizeHostVisible[iBuffer] = -1;
		}
	}

	void	DestroyD3D12()
	{
		for (u32 iBuffer = 0; iBuffer < SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER; ++iBuffer)
		{
			if (m_D3D12HostVisible[iBuffer] != null)
				m_D3D12HostVisible[iBuffer]->Release();
			m_D3D12HostVisible[iBuffer] = null;
			m_D3D12SizeHostVisible[iBuffer] = -1;
		}
	}

	CDX12BufferHandles()
	{
		InitD3D12();
	}

	~CDX12BufferHandles() override
	{
		DestroyD3D12();
	}
};

#endif // PK_BUILD_WITH_D3D12_SUPPORT

//-------------------------------------------------------------------------------------
