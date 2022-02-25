//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#ifndef	__FX_PACK_EXPLORER_H__
#define	__FX_PACK_EXPLORER_H__

#include <pk_kernel/include/kr_file_directory_walker.h>

using namespace PopcornFX;

//----------------------------------------------------------------------------

class	CPackExplorer : public CFileDirectoryWalker, public CRefCountedObject
{
protected:
	CString				m_Pack;
	TArray<CString>		m_EffectPaths;

	virtual bool		DirectoryNotifier(const CFilePack *pack, const char *fullPath, u32 directoryFirstCharPos) override;
	virtual void		FileNotifier(const CFilePack *pack, const char *fullPath, u32 fileFirstCharPos) override;

public:
	CPackExplorer(const CString &pack, IFileSystem *fileSystem = null);
	~CPackExplorer();

	void						Explore();	// will start scanning

	const CString				&Pack() const { return m_Pack; }
	TMemoryView<const CString>	EffectPaths() const { return m_EffectPaths; }
};


//----------------------------------------------------------------------------

#endif	// __FX_PACK_EXPLORER_H__
