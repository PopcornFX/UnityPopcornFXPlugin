//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include "EditorOnly/EditorNativeToManaged.h"
#include "pk_base_object/include/hbo_object.h"
#include "pk_kernel/include/kr_threads.h"
#include "pk_kernel/include/kr_timers.h"
#include "pk_kernel/include/kr_resources.h"
#include "PK-AssetBakerLib/AssetBaker_Cookery.h"

__PK_API_BEGIN

class	CResourceHandlerMesh;
class	CResourceHandlerImage;
class	CResourceHandlerRectangleList;
class	CResourceHandlerFontMetrics;
class	CResourceHandlerVectorField;
class	CResourceManager;
class	CCookery;
class	COvenBakeConfig_Particle;

namespace	HBO {
	class	CContext;
}

class	SBakeContext
{
public:
	SBakeContext();
	~SBakeContext();
	PopcornFX::IResourceHandler				*m_BakeResourceMeshHandler;
	PopcornFX::IResourceHandler				*m_BakeResourceImageHandler;
	PopcornFX::IResourceHandler				*m_BakeResourceRectangleListHandler;
	PopcornFX::IResourceHandler				*m_BakeResourceFontMetricsHandler;
	PopcornFX::IResourceHandler				*m_BakeResourceVectorFieldHandler;
	PopcornFX::IFileSystem					*m_BakeFSController;
	PopcornFX::CResourceManager				*m_BakeResourceManager;
	PopcornFX::HBO::CContext				*m_BakeContext;

	CCookery								m_Cookery;
	PopcornFX::SBakeTarget					m_DstBakeTarget;
	PopcornFX::SBakeTarget					m_CacheBakeTarget;
	CString									m_EditorCacheDirectory;

	bool	Init();
	void	Destroy();
	bool	Reinit();

	void	_RemapResourcesPath(CString &path, bool &, CMessageStream &);
};

class	CEffectBaker : public CRefCountedObject
{
private:
	struct	SAssetChange
	{
		CString					m_EffectPath;
		CString					m_EffectPathOld;
		EAssetChangesType		m_Type;
		int						m_Try;
	};

	struct SDirectoryValidator
	{
		const CStringView		m_LibraryDir;
		const CStringView		m_EditorCacheDir;
		const CStringView		m_TemplatesDir;
		CProjectSettingsAssets	*m_AssetSettings;
		PFilePack				m_Pack;

		bool cmp(const char *rawPath)
		{
			CString relPath = CFilePath::Relativize(m_Pack->Path().Data(), rawPath);
			const CStringView path = CStringView::FromNullTerminatedString(rawPath);
			if (m_AssetSettings != null &&
				m_AssetSettings->IsPathIgnored(relPath.View()))
				return false;
			return !path.Contains(m_LibraryDir) && !path.Contains(m_EditorCacheDir) && !path.Contains(m_TemplatesDir);
		}

		SDirectoryValidator(const CString &library, const CString &editor, const CString &templat, CProjectSettingsAssets *assetSettings, PFilePack pack)
		:	m_LibraryDir(library)
		,	m_EditorCacheDir(editor)
		,	m_TemplatesDir(templat)
		,	m_AssetSettings(assetSettings)
		,	m_Pack(pack)
		{
		}
	};


public:
	CEffectBaker();
	~CEffectBaker();

	void				FileAdded(const CString &path);
	void				FileRemoved(const CString &path);
	void				FileChanged(const CString &path);
	void				FileChangedRelativePath(const CString &path);
	void				FileRenamed(const CString &oldPath, const CString &newPath);

	void				Initialize(const char *pKPackPath);
	void				Clear();

	void				Lock();
	void				Unlock();

	void				SetForceDeterminismFlag(bool enable);

	void				CancelAllFileChanges();
	int					PopFileChanges();
	bool				IsChangeRegistered(const CString &path, EAssetChangesType type);
	void				ReimportAssets(TArray<CString> &paths);
	void				GetAllAssetPath();
	bool				BakeAssetOrAddToRetryStack(SAssetChange &path);
	bool				BakeAsset(const CString &path, bool bakeDependencies);

	static void			OutputBakedResourceInCache(const CString path, TStaticCountedArray<CString, 4> &outputs);

	CString				GetTempDir(const char *dirPrefix = null);
	bool				ExtractPackage(IFileSystem *fs, const CString &srcArchiveAbsPath, const CString &dstDirAbsPath, CString &outPkprojAbsPath);
	bool				UpgradeProject(IFileSystem *fs, const CString &projectSettingsAbsPath);

	bool				ExtrackPkkg(const CString &srcArchiveAbsPath, bool runUpgrades = false);
	bool				DeleteFolderRec(const CString &absDirPath);

	SBakeContext		&GetBakeContextData() { return m_BakeContext; }
	CString				GetPopcornFXPackPath() { return m_PKPackPath; }
private:
	const int					m_MaxBakeRetry = 5;

	TArray<SAssetChange>		m_ToProcess;
	TArray<SAssetChange>		m_ToBake;
	CString						m_PKPackPath;
	PFilePack					m_PKSourcePack;
	PProjectSettings			m_ProjectSettings;
	CTimer						m_Timer;

	SBakeContext				m_BakeContext;

	Threads::CCriticalSection	m_Lock;

	COvenBakeConfig_Particle	*m_BakeConfigParticle;

	bool						m_ForceDeterminism = false;
};

//----------------------------------------------------------------------------


PK_DECLARE_REFPTRCLASS(EffectBaker);

//----------------------------------------------------------------------------
__PK_API_END
