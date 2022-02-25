//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PKUnity_ImageResourceManager.h"

#include <pk_imaging/include/im_image.h>
#include <pk_kernel/include/kr_refcounted_buffer.h>

#include "PKUnity_FileSystemController.h"

__PK_API_BEGIN

//----------------------------------------------------------------------------

CUnityResourceHandlerImage::CUnityResourceHandlerImage()
{
}

//----------------------------------------------------------------------------

CUnityResourceHandlerImage::~CUnityResourceHandlerImage()
{
}

//----------------------------------------------------------------------------

struct sHeader
{
	u32 width;
	u32 height;
	u32 dataSize;
	u32 format;
};

//----------------------------------------------------------------------------

PImage			CUnityResourceHandlerImage::_LoadImage(const CFilePackPath &resourcePath, IFileSystem *fs)
{
	PFileStream		fileView = fs->OpenStream(resourcePath, IFileSystem::Access_Read);

	if (fileView == null)
		return null;

	sHeader header;
	u64		sizeRead = fileView->Read(&header, sizeof(sHeader));

	if (!PK_VERIFY(sizeRead == sizeof(sHeader) && header.dataSize != 0))
		return null;

	CImage *img = PK_NEW(CImage);

	img->m_Flags = 0;
	img->m_Format = (CImage::EFormat)header.format;

	img->m_Frames.PushBack(CImageFrame());
	CImageFrame	&frame = img->m_Frames[0];

	frame.m_Mipmaps.PushBack(CImageMap());
	CImageMap	&map = frame.m_Mipmaps[0];

	map.m_Dimensions = CUint3(header.width, header.height, 1);
	map.m_RawBuffer = CRefCountedMemoryBuffer::AllocAligned(header.dataSize, 0x10);
	PK_ONLY_IF_ASSERTS(u64 byteRead = )fileView->Read(map.m_RawBuffer->Data<void>(), header.dataSize);

	PK_ASSERT(byteRead == header.dataSize);

	return img;
}

void	*CUnityResourceHandlerImage::Load(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,
										const CString			&resourcePath,
										bool					pathNotVirtual,
										const SResourceLoadCtl	&loadCtl,
										CMessageStream			&loadReport,
										SAsyncLoadStatus		*asyncLoadStatus)
{
	if (!resourcePath.Empty())
	{
		IFileSystem		*fs = resourceManager->FileController();

		PFilePack	pack;
		CString		virtPath;
		if (pathNotVirtual)
			virtPath = fs->PhysicalToVirtual(resourcePath, &pack);
		else
		{
			PK_ONLY_IF_ASSERTS(CString realPath = )fs->VirtualToPhysical(resourcePath, IFileSystem::Access_Read, &pack);
			virtPath = resourcePath;
		}

		if (!virtPath.Empty() && pack != null)
		{
			return Load(resourceManager, resourceTypeID, CFilePackPath(pack, virtPath), loadCtl, loadReport, asyncLoadStatus);
		}
	}

	if (asyncLoadStatus != null)
	{
		asyncLoadStatus->m_Resource = null;
		asyncLoadStatus->m_Done = true;
		asyncLoadStatus->m_Progress = 1.0f;
	}
	return null;
}

void	*CUnityResourceHandlerImage::Load(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,
										const CFilePackPath		&resourcePath,
										const SResourceLoadCtl	&loadCtl,
										CMessageStream			&,
										SAsyncLoadStatus		*asyncLoadStatus)
{
	(void)resourceManager; (void)resourceTypeID;
	PK_SCOPEDPROFILE();
	CString	fullPath = resourcePath.FullPath();

	if (!loadCtl.m_Reload)
	{
		PK_SCOPEDLOCK(m_Lock);
		SResourceEntry	*existingEntry = m_Images.Find(fullPath);
		if (existingEntry != null)
		{
			existingEntry->m_ReferenceCount++;
			return existingEntry->m_Image.Get();
		}
	}

	PImage	resource = _LoadImage(resourcePath, resourceManager->FileController());

	if (resource != null &&
		resource->m_Format != CImage::Format_Invalid)	// Format check : protection about codecs that might go wrong.
	{
		PImage	dstResource;
		{
			PK_SCOPEDLOCK(m_Lock);
			SResourceEntry	*existingEntry = m_Images.Find(fullPath);
			if (existingEntry == null)
			{
#ifdef	RESOURCE_IMAGE_LOG_ENABLED
				CLog::Log(PK_INFO, "[RESOURCE] : Loading Image resource %p : %s", resource.Get(), fullPath.Data());
#endif
				m_Images.Insert(fullPath, SResourceEntry(resource, 1));
			}
			else
			{
				existingEntry->m_ReferenceCount++;
				if (!loadCtl.m_Reload)
				{
#ifdef	RESOURCE_IMAGE_LOG_ENABLED
					CLog::Log(PK_INFO, "[RESOURCE] : Loading Image resource, reusing %p[%d] : %s", resource.Get(), existingEntry->m_ReferenceCount, fullPath.Data());
#endif
					resource = existingEntry->m_Image;
				}
				else
				{
					// replace the entry in the resource-list
					PK_ASSERT(existingEntry->m_Image != null);
					dstResource = existingEntry->m_Image;

#ifdef	RESOURCE_IMAGE_LOG_ENABLED
					CLog::Log(PK_INFO, "[RESOURCE] : Reloading Image resource, swapping {%p,%p}[%d] : %s", resource.Get(), dstResource.Get(), existingEntry->m_ReferenceCount, fullPath.Data());
#endif

					dstResource->m_OnReloading(dstResource.Get());

					PKSwap(dstResource->m_Flags, resource->m_Flags);
					PKSwap(dstResource->m_Format, resource->m_Format);
					PKSwap(dstResource->m_Frames, resource->m_Frames);
				}
			}
		}

		// do this outside the m_Lock, call to 'm_OnReloaded' might call Load() again (it's not extremely safe to do so but in practise it can happen)
		// avoid a deadlock by calling the callback-chain outside the scopedlock
		if (dstResource != null)
		{
			dstResource->m_OnReloaded(dstResource.Get());
			resource = dstResource;
		}

		void	*resourcePtr = resource.Get();
		if (asyncLoadStatus != null)
		{
			asyncLoadStatus->m_Resource = resourcePtr;
			asyncLoadStatus->m_Done = true;
			asyncLoadStatus->m_Progress = 1.0f;
		}
		return resourcePtr;
	}

	if (asyncLoadStatus != null)
	{
		asyncLoadStatus->m_Resource = null;
		asyncLoadStatus->m_Done = true;
		asyncLoadStatus->m_Progress = 1.0f;
	}
	return null;
}

void	CUnityResourceHandlerImage::Unload(	const CResourceManager	*resourceManager,
										u32						resourceTypeID,	// used to check we are called with the correct type
										void					*resource)
{
	(void)resourceManager;
	PK_SCOPEDPROFILE();
	u32		imageResourceTypeID = TResourceRouter<CImage>::ResourceTypeID();
	PK_ASSERT(resourceTypeID == imageResourceTypeID);
	if (resourceTypeID != imageResourceTypeID)
		return;

	PImage	resourcePtr = static_cast<CImage*>(resource);

	{
		PK_SCOPEDLOCK(m_Lock);
		THashMap<SResourceEntry, CString>::Iterator	it = m_Images.Begin();
		THashMap<SResourceEntry, CString>::Iterator	end = m_Images.End();

		u32	refsLeft = 0;
		CString	fullPath;
		while (it != end)
		{
			if (it->m_Image == resourcePtr)
			{
				fullPath = it.Key();
				refsLeft = --(it->m_ReferenceCount);
				break;
			}
			++it;
		}

		if (!fullPath.Empty() && refsLeft == 0)
		{
#ifdef	RESOURCE_IMAGE_LOG_ENABLED
			CLog::Log(PK_INFO, "[RESOURCE] : Unloading Image resource %p \"%s\"", resource, fullPath.Data());
#endif
			m_Images.Remove(fullPath);
		}
#ifdef	RESOURCE_IMAGE_LOG_ENABLED
		else if (refsLeft != 0)
			CLog::Log(PK_INFO, "[RESOURCE] : Unloading Image resource %p[%d] \"%s\"", resource, refsLeft, fullPath.Data());
		else if (fullPath.Empty())
			CLog::Log(PK_INFO, "[RESOURCE] : ERROR NOT FOUND : Unloading Image resource %p \"%s\"", resource, fullPath.Data());
#endif
	}
}

void	CUnityResourceHandlerImage::AppendDependencies(	const CResourceManager	*resourceManager,
													u32						resourceTypeID,
													void					*resource,
													TArray<CString>			&outResourcePaths) const
{
	(void)resourceManager; (void)resourceTypeID; (void)resource; (void)outResourcePaths;
	PK_ASSERT(resourceTypeID == TResourceRouter<CImage>::ResourceTypeID());
}

void	CUnityResourceHandlerImage::AppendDependencies(	const CResourceManager	*resourceManager,
													u32						resourceTypeID,
													const CString			&resourcePath,
													bool					pathNotVirtual,
													TArray<CString>			&outResourcePaths) const
{
	(void)resourceManager; (void)resourceTypeID; (void)resourcePath; (void)pathNotVirtual; (void)outResourcePaths;
	PK_ASSERT(resourceTypeID == TResourceRouter<CImage>::ResourceTypeID());
}

void	CUnityResourceHandlerImage::AppendDependencies(	const CResourceManager	*resourceManager,
													u32						resourceTypeID,
													const CFilePackPath		&resourcePath,
													TArray<CString>			&outResourcePaths) const
{
	(void)resourceManager; (void)resourceTypeID; (void)resourcePath; (void)outResourcePaths;
	PK_ASSERT(resourceTypeID == TResourceRouter<CImage>::ResourceTypeID());
}

void	CUnityResourceHandlerImage::BroadcastResourceChanged(const CResourceManager *resourceManager, const CFilePackPath &resourcePath)
{
	PK_SCOPEDPROFILE();
	if (resourcePath.Empty())
		return;

	CString		fullPath = resourcePath.FullPath();
	bool		hasResource = false;
	{
		PK_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		hasResource = (foundResource != null);
	}

	if (hasResource)
	{
#ifdef	RESOURCE_IMAGE_LOG_ENABLED
		CLog::Log(PK_INFO, "[RESOURCE] : Reloading image resource \"%s\"", fullPath.Data());
#endif

		// reload
		PImage	resource = _LoadImage(resourcePath, resourceManager->FileController());

		if (resource != null)
		{
			PK_SCOPEDLOCK(m_Lock);
			SResourceEntry	*foundResource = m_Images.Find(fullPath);
			if (foundResource != null)	// could have been Unload-ed in the meantime
			{
				// replace the entry in the resource-list
				PK_ASSERT(foundResource->m_Image != null);
				CImage	*dstImage = foundResource->m_Image.Get();
				dstImage->m_OnReloading(dstImage);

				//				if (resource != null)
				{
					PKSwap(dstImage->m_Flags, resource->m_Flags);
					PKSwap(dstImage->m_Format, resource->m_Format);
					PKSwap(dstImage->m_Frames, resource->m_Frames);
					dstImage->m_OnReloaded(dstImage);
				}
				//				else
				//					dstImage->m_OnReloaded(null);
			}
		}
	}
}

__PK_API_END
