//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "RenderAPI_Data.h"

#if (PK_BUILD_WITH_D3D11_SUPPORT != 0)

#if	defined(PK_DURANGO)
#	include <d3d11_x.h>
#else
#	include <d3d11.h>
#endif // defined(PK_DURANGO)

//-------------------------------------------------------------------------------------

class	CDX11BufferHandles : public CBufferAbstract
{
public:
	ID3D11Buffer	*m_D3D11HostVisible[SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER];
	s32				m_D3D11SizeHostVisible[SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER];

	void	InitD3D11()
	{
		for (u32 i = 0; i < SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER; ++i)
		{
			m_D3D11HostVisible[i] = null;
			m_D3D11SizeHostVisible[i] = -1;
		}
	}

	void	DestroyD3D11()
	{
		for (u32 i = 0; i < SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER; ++i)
		{
			if (m_D3D11HostVisible[i] != null)
				m_D3D11HostVisible[i]->Release();
			m_D3D11HostVisible[i] = null;
			m_D3D11SizeHostVisible[i] = -1;
		}
	}

	CDX11BufferHandles()
	{
		InitD3D11();
	}

	~CDX11BufferHandles() override
	{
		DestroyD3D11();
	}
};

#endif // PK_BUILD_WITH_D3D11_SUPPORT

//-------------------------------------------------------------------------------------
