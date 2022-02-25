//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_resources.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CResourceHandlerDummy : public IResourceHandler
{
public:
	CResourceHandlerDummy() { }
	virtual ~CResourceHandlerDummy() { }

	virtual void	*Load(const CResourceManager *, u32, const CString &, bool, const SResourceLoadCtl &, CMessageStream &, SAsyncLoadStatus *) override
	{
		return null;
	}

	virtual void	*Load(const CResourceManager *, u32, const CFilePackPath &, const SResourceLoadCtl &, CMessageStream &, SAsyncLoadStatus *) override
	{
		return null;
	}

	virtual void	Unload(const CResourceManager *, u32, void *) override
	{
	}

	virtual void	AppendDependencies(const CResourceManager *, u32, void *, TArray<CString> &) const override
	{
	}

	virtual void	AppendDependencies(const CResourceManager *, u32, const CString &, bool, TArray<CString> &) const override
	{
	}

	virtual void	AppendDependencies(const CResourceManager *, u32, const CFilePackPath &, TArray<CString> &) const override
	{
	}

	virtual void	BroadcastResourceChanged(const CResourceManager *, const CFilePackPath &) override
	{
	}
};

//----------------------------------------------------------------------------
__PK_API_END

