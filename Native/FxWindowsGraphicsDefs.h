//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------
// forward-declare the rendering contexts/devices based on the platform and the
// selected rendering build options:

#if	defined(PK_WINDOWS) || defined(PK_UWP)
#	if	(PK_BUILD_WITH_GL_SUPPORT == 0) && (PK_BUILD_WITH_D3D9_SUPPORT == 0) && (PK_BUILD_WITH_D3D11_SUPPORT == 0)
#		define	PK_BUILD_WITH_D3D9_SUPPORT	1
#	endif

#	if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)

#	pragma warning(push)
#	pragma warning(disable : 4005) // '%m' : macro redefinition
#		include <d3d11.h>
#	pragma warning(pop)

	typedef ID3D11Device			D3DDevice;
	typedef ID3D11DeviceContext		D3DDeviceContext;
#	endif
#	if	(PK_BUILD_WITH_D3D9_SUPPORT != 0)
	struct	IDirect3DDevice9;
#	endif

#	if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)
#		include <d3d12.h>
#	endif
#endif
