//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "pk_imaging/include/im_image.h"
#include <pk_kernel/include/kr_resources.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	IFileSystem;

class	CUnityResourceHandlerImage : public IResourceHandler
{
protected:
	struct	SResourceEntry
	{
		PImage	m_Image;
		u32		m_ReferenceCount;

		SResourceEntry(const PImage &image, u32 refCount) : m_Image(image), m_ReferenceCount(refCount) {}
	};

	Threads::CCriticalSection			m_Lock;
	THashMap<SResourceEntry, CString>	m_Images;

	PImage			_LoadImage(const CFilePackPath &resourcePath, IFileSystem *fs);

public:
	CUnityResourceHandlerImage();
	virtual ~CUnityResourceHandlerImage();

	virtual void	*Load(	const CResourceManager	*resourceManager,
							u32						resourceTypeID,	// used to check we are called with the correct type
							const CString			&resourcePath,
							bool					pathNotVirtual,
							const SResourceLoadCtl	&loadCtl,
							CMessageStream			&loadReport,
							SAsyncLoadStatus		*asyncLoadStatus) override;	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress

	virtual void	*Load(	const CResourceManager	*resourceManager,
							u32						resourceTypeID,	// used to check we are called with the correct type
							const CFilePackPath		&resourcePath,
							const SResourceLoadCtl	&loadCtl,
							CMessageStream			&loadReport,
							SAsyncLoadStatus		*asyncLoadStatus) override;	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress

	virtual void	Unload(	const CResourceManager	*resourceManager,
							u32						resourceTypeID,	// used to check we are called with the correct type
							void					*resource) override;

	virtual void	AppendDependencies(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,
										void					*resource,
										TArray<CString>			&outResourcePaths) const override;

	virtual void	AppendDependencies(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,
										const CString			&resourcePath,
										bool					pathNotVirtual,
										TArray<CString>			&outResourcePaths) const override;

	virtual void	AppendDependencies(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,
										const CFilePackPath		&resourcePath,
										TArray<CString>			&outResourcePaths) const override;

	virtual void	BroadcastResourceChanged(const CResourceManager *resourceManager, const CFilePackPath &resourcePath) override;
};

//----------------------------------------------------------------------------
__PK_API_END
