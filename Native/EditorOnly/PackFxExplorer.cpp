//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PackFxExplorer.h"

//----------------------------------------------------------------------------

CPackExplorer::CPackExplorer(const CString &pack, IFileSystem *controller)
:	CFileDirectoryWalker(pack, IgnoreVirtualFS, controller)
,	m_Pack(pack)
{
}

//----------------------------------------------------------------------------

CPackExplorer::~CPackExplorer()
{
}

//----------------------------------------------------------------------------

void	CPackExplorer::Explore()
{
	if (!m_Pack.Empty())
	{
		Walk();
	}
}

//----------------------------------------------------------------------------

bool	CPackExplorer::DirectoryNotifier(const CFilePack *pack, const char *fullPath, u32 directoryFirstCharPos)
{
	(void)pack; (void)directoryFirstCharPos;
	const CString	filename = CFilePath::ExtractFilename(fullPath);

	// go full recursive unless a hidden directory somehow ended up in the pack
	return filename[0] != '.';
}

//----------------------------------------------------------------------------

void	CPackExplorer::FileNotifier(const CFilePack *pack, const char *fullPath, u32 fileFirstCharPos)
{
	(void)pack; (void)fileFirstCharPos;
	const char	*extension = CFilePath::ExtractExtension(fullPath);

	// add the effect to the list
	if (extension != null && !strcasecmp(extension, "pkfx"))
	{
		const CString	effectPath = CFilePath::Relativize(m_Pack.Data(), fullPath);
		m_EffectPaths.PushBack(effectPath);
	}
}

//----------------------------------------------------------------------------
