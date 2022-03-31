//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "EditorOnly/EffectBaking.h"
#include "EditorOnly/EditorManagedToNative.h"
#include "EditorOnly/EditorNativeToManaged.h"
#include "EditorOnly/PackFxExplorer.h"

#include "PKUnity_FileSystemController.h"

#include "PKUnity_Plugins.h"
#include "PKUnity_ImageResourceManager.h"
#include "RuntimeManager.h"
#include "ResourceHandleDummy.h"

#include "pk_kernel/include/kr_refcounted_buffer.h"
#include "pk_kernel/include/kr_resources.h"

#include "pk_imaging/include/im_resource.h"
#include "pk_geometrics/include/ge_mesh_resource_handler.h"
#include "pk_geometrics/include/ge_rectangle_list.h"
#include "pk_particles/include/ps_font_metrics_resource.h"
#include "pk_particles/include/ps_vectorfield_resource.h"
#include "pk_base_object/include/hbo_context.h"

#include "PK-AssetBakerLib/AssetBaker_Oven.h"
#include "PK-AssetBakerLib/AssetBaker_Oven_HBO.h"
#include "PK-AssetBakerLib/AssetBaker_Oven_Mesh.h"
#include "PK-AssetBakerLib/AssetBaker_Oven_VectorField.h"
#include "PK-AssetBakerLib/AssetBaker_Oven_Texture.h"
#include "PK-AssetBakerLib/AssetBaker_Oven_StraightCopy.h"

__PK_API_BEGIN

#define	BAKE_DESTINATION_PATH	"Temp/PopcornFx/Baked/"

namespace
{
	const CStringView	kPopcornProjectExtension = "pkproj";

	//----------------------------------------------------------------------------

	class	CProjectSettingsFinder : public CFileDirectoryWalker
	{
	public:
		CProjectSettingsFinder(const CString &rootDir, IFileSystem *controller = null)
			: CFileDirectoryWalker(rootDir, IgnoreVirtualFS, controller)
		{
			SetFilePathValidator(ProjectSettingsPathValidator);
		}

		virtual void	FileNotifier(const CFilePack *pack, const char *fullPath, u32 fileFirstCharPos) override
		{
			(void)pack; (void)fileFirstCharPos;
			PK_ASSERT(CFilePath::IsPure(fullPath));
			PK_ASSERT(kPopcornProjectExtension == CFilePath::ExtractExtension(fullPath));

			if (!PK_VERIFY(m_ProjectSettingsPath == null))
			{
				CLog::Log(PK_ERROR, "Multiple pkproj found in RootDir (\"%s\").", fullPath);
			}
			else
			{
				m_ProjectSettingsPath = fullPath;
			}
		}

		virtual bool	DirectoryNotifier(const CFilePack *pack, const char *fullPath, u32 directoryFirstCharPos) override
		{
			(void)pack; (void)fullPath; (void)directoryFirstCharPos;
			return true;	// Recursive search
		}

		const CString	&ProjectSettingsPath() const { return m_ProjectSettingsPath; }

	private:
		static bool		ProjectSettingsPathValidator(const char *filePath) { return kPopcornProjectExtension == CFilePath::ExtractExtension(filePath); }

	private:
		CString		m_ProjectSettingsPath;
	};


}


//----------------------------------------------------------------------------

SBakeContext::SBakeContext()
	: m_BakeResourceMeshHandler(null)
	, m_BakeResourceImageHandler(null)
	, m_BakeResourceRectangleListHandler(null)
	, m_BakeResourceFontMetricsHandler(null)
	, m_BakeResourceVectorFieldHandler(null)
	, m_BakeFSController(null)
	, m_BakeResourceManager(null)
{
}

//----------------------------------------------------------------------------

SBakeContext::~SBakeContext()
{
	Destroy();
}

//----------------------------------------------------------------------------

bool	SBakeContext::Init()
{
	PK_ASSERT(m_BakeResourceMeshHandler == null);
	PK_ASSERT(m_BakeResourceImageHandler == null);
	PK_ASSERT(m_BakeResourceVectorFieldHandler == null);
	PK_ASSERT(m_BakeResourceFontMetricsHandler == null);
	PK_ASSERT(m_BakeResourceRectangleListHandler == null);
	PK_ASSERT(m_BakeFSController == null);
	PK_ASSERT(m_BakeResourceManager == null);

	m_BakeResourceImageHandler = PK_NEW(CResourceHandlerImage);
	m_BakeResourceMeshHandler = PK_NEW(CResourceHandlerMesh);
	m_BakeResourceVectorFieldHandler = PK_NEW(CResourceHandlerVectorField);
	m_BakeResourceRectangleListHandler = PK_NEW(CResourceHandlerRectangleList);
	m_BakeResourceFontMetricsHandler = PK_NEW(CResourceHandlerFontMetrics);

	if (!PK_VERIFY(m_BakeResourceMeshHandler != null) ||
		!PK_VERIFY(m_BakeResourceImageHandler != null) ||
		!PK_VERIFY(m_BakeResourceRectangleListHandler != null) ||
		!PK_VERIFY(m_BakeResourceFontMetricsHandler != null) ||
		!PK_VERIFY(m_BakeResourceVectorFieldHandler != null))
		return false;

	m_BakeFSController = File::NewInternalFileSystem();
	if (!PK_VERIFY(m_BakeFSController != null))
		return false;

	m_BakeResourceManager = PK_NEW(PopcornFX::CResourceManager(m_BakeFSController));
	if (!PK_VERIFY(m_BakeResourceManager != null))
		return false;
	m_BakeResourceManager->RegisterHandler<PopcornFX::CResourceMesh>(m_BakeResourceMeshHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CImage>(m_BakeResourceImageHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CRectangleList>(m_BakeResourceRectangleListHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CFontMetrics>(m_BakeResourceFontMetricsHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CVectorField>(m_BakeResourceVectorFieldHandler);
	m_BakeResourceManager->m_RemapResourcePath = FastDelegate<void(CString &, bool &, CMessageStream &)>(this, &SBakeContext::_RemapResourcesPath);

	m_BakeContext = PK_NEW(PopcornFX::HBO::CContext(m_BakeResourceManager));
	if (!PK_VERIFY(m_BakeContext != null))
		return false;

	// register the oven's HBO bake-config classes:
	COvenBakeConfig_Base::RegisterHandler();
	COvenBakeConfig_HBO::RegisterHandler();
	COvenBakeConfig_Mesh::RegisterHandler();
	COvenBakeConfig_Texture::RegisterHandler();
	COvenBakeConfig_TextureAtlas::RegisterHandler();
	COvenBakeConfig_VectorField::RegisterHandler();
	COvenBakeConfig_ParticleCompiler::RegisterHandler();
	COvenBakeConfig_Particle::RegisterHandler();
	COvenBakeConfig_StraightCopy::RegisterHandler();
	COvenBakeConfig_Audio::RegisterHandler();
	return true;
}

//----------------------------------------------------------------------------

void	SBakeContext::Destroy()
{
	if (m_BakeResourceManager != null)
	{
		if (m_BakeResourceMeshHandler != null)
			m_BakeResourceManager->UnregisterHandler<PopcornFX::CResourceMesh>(m_BakeResourceMeshHandler);
		if (m_BakeResourceImageHandler != null)
			m_BakeResourceManager->UnregisterHandler<PopcornFX::CImage>(m_BakeResourceImageHandler);
		if (m_BakeResourceRectangleListHandler != null)
			m_BakeResourceManager->UnregisterHandler<PopcornFX::CRectangleList>(m_BakeResourceRectangleListHandler);
		if (m_BakeResourceFontMetricsHandler != null)
			m_BakeResourceManager->UnregisterHandler<PopcornFX::CFontMetrics>(m_BakeResourceFontMetricsHandler);
		if (m_BakeResourceVectorFieldHandler != null)
			m_BakeResourceManager->UnregisterHandler<PopcornFX::CVectorField>(m_BakeResourceVectorFieldHandler);
	}
	PK_SAFE_DELETE(m_BakeResourceMeshHandler);
	PK_SAFE_DELETE(m_BakeResourceImageHandler);
	PK_SAFE_DELETE(m_BakeResourceVectorFieldHandler);
	PK_SAFE_DELETE(m_BakeResourceFontMetricsHandler);
	PK_SAFE_DELETE(m_BakeResourceRectangleListHandler);
	PK_SAFE_DELETE(m_BakeContext);
	PK_SAFE_DELETE(m_BakeFSController);
	PK_SAFE_DELETE(m_BakeResourceManager);

	// unregister the oven's HBO bake-config classes:
	COvenBakeConfig_Audio::UnregisterHandler();
	COvenBakeConfig_StraightCopy::UnregisterHandler();
	COvenBakeConfig_Particle::UnregisterHandler();
	COvenBakeConfig_ParticleCompiler::UnregisterHandler();
	COvenBakeConfig_VectorField::UnregisterHandler();
	COvenBakeConfig_TextureAtlas::UnregisterHandler();
	COvenBakeConfig_Texture::UnregisterHandler();
	COvenBakeConfig_Mesh::UnregisterHandler();
	COvenBakeConfig_HBO::UnregisterHandler();
	COvenBakeConfig_Base::UnregisterHandler();
}

//----------------------------------------------------------------------------

bool	SBakeContext::Reinit()
{
	Destroy();
	return Init();
}

//----------------------------------------------------------------------------


void	SBakeContext::_RemapResourcesPath(CString &path, bool &, CMessageStream &)
{
	if (path.EndsWith(".fbx", CaseInsensitive))
		path = CFilePath::StripExtension(path) + ".pkmm";
	if (path.EndsWith(".fga", CaseInsensitive))
		path = CFilePath::StripExtension(path) + ".pkvf";

	TStaticCountedArray<CString, 4>		outputPaths;
	CEffectBaker::OutputBakedResourceInCache(path, outputPaths);
	if (!outputPaths.Empty())
		path = m_EditorCacheDirectory / path;
}

//----------------------------------------------------------------------------

CEffectBaker::CEffectBaker()
	: m_PKSourcePack(null)
	, m_ProjectSettings(null)
	, m_BakeConfigParticle(null)
{
	m_BakeContext.Init();
}

//----------------------------------------------------------------------------

CEffectBaker::~CEffectBaker()
{
}

//----------------------------------------------------------------------------

void			CEffectBaker::FileAdded(const CString &path)
{
	if (path.EndsWith(".pkfx") == true)
	{
		SAssetChange	effect;

		if (IsChangeRegistered(path, EAssetChangesType::Add) == true)
			return;
		CString		cleanPath = path.Extract(m_PKPackPath.SlashAppended().Length(), path.Length());
		CFilePath::Purify(cleanPath);
		effect.m_EffectPath = cleanPath;
		effect.m_Type = EAssetChangesType::Add;
		effect.m_Try = 0;

		PK_SCOPEDLOCK(m_Lock);
		m_ToBake.PushBack(effect);
		m_Timer.Reset();
	}
}

//----------------------------------------------------------------------------

void			CEffectBaker::FileRemoved(const CString &path)
{
	if (path.EndsWith(".pkfx") == true)
	{
		SAssetChange	effect;

		if (IsChangeRegistered(path, EAssetChangesType::Remove) == true)
			return;
		CString		cleanPath = path.Extract(m_PKPackPath.SlashAppended().Length(), path.Length());
		CFilePath::Purify(cleanPath);
		effect.m_EffectPath = cleanPath;
		effect.m_Type = EAssetChangesType::Remove;

		PK_SCOPEDLOCK(m_Lock);
		m_ToProcess.PushBack(effect);
		m_Timer.Reset();
	}
}

//----------------------------------------------------------------------------

void			CEffectBaker::FileChanged(const CString &path)
{
	if (path.EndsWith(".pkfx") == true)
	{
		SAssetChange	effect;

		if (IsChangeRegistered(path, EAssetChangesType::Update) == true)
			return;
		CString		cleanPath = path.Extract(m_PKPackPath.SlashAppended().Length(), path.Length());
		CFilePath::Purify(cleanPath);

		CEffectBaker::FileChangedRelativePath(cleanPath);
	}
}

void			CEffectBaker::FileChangedRelativePath(const CString &path)
{
	if (path.EndsWith(".pkfx") == true)
	{
		SAssetChange	effect;

		effect.m_EffectPath = path;
		effect.m_Type = EAssetChangesType::Update;
		effect.m_Try = 0;

		PK_SCOPEDLOCK(m_Lock);
		m_ToBake.PushBack(effect);
		m_Timer.Reset();
	}
}

//----------------------------------------------------------------------------

void			CEffectBaker::FileRenamed(const CString &oldPath, const CString &newPath)
{
	if (newPath.EndsWith(".pkfx") == true)
	{
		SAssetChange	effect;

		if (IsChangeRegistered(newPath, EAssetChangesType::Rename) == true)
			return;

		CString		cleanPath = newPath.Extract(m_PKPackPath.SlashAppended().Length(), newPath.Length());
		CFilePath::Purify(cleanPath);
		CString		cleanOldPath = oldPath.Extract(m_PKPackPath.SlashAppended().Length(), oldPath.Length());
		CFilePath::Purify(cleanPath);

		effect.m_EffectPath = cleanPath;
		effect.m_EffectPathOld = cleanOldPath;
		effect.m_Type = EAssetChangesType::Rename;
		effect.m_Try = 0;

		PK_SCOPEDLOCK(m_Lock);
		m_ToBake.PushBack(effect);
		m_Timer.Reset();
	}
}

//----------------------------------------------------------------------------

void			CEffectBaker::Initialize(const char *pKPackPath)
{
	m_BakeContext.m_BakeContext->UnloadAllFiles();
	m_BakeContext.m_BakeFSController->UnmountAllPacks();

	if (pKPackPath == null)
		return;

	IFileSystem				*fs = m_BakeContext.m_BakeFSController;
	HBO::CContext			*context = m_BakeContext.m_BakeContext;

	// Find source pack root path:
	CProjectSettingsFinder	finder(pKPackPath, fs);
	finder.Walk();

	const CString	projectSettingsFilePath = finder.ProjectSettingsPath();
	if (projectSettingsFilePath.Empty() ||
		!fs->Exists(projectSettingsFilePath, true))
	{
		CLog::Log(PK_ERROR, "Could not find Project Settings in pack \"%s\"", pKPackPath);
		return;
	}

	const CString	fileBuffer = fs->BufferizeToString(projectSettingsFilePath, true);
	if (fileBuffer == null)
	{
		CLog::Log(PK_ERROR, "Failed to read project settings file at \"%s\"", projectSettingsFilePath.Data());
		return;
	}

	CConstMemoryStream				stream(fileBuffer.Data(), fileBuffer.Length());

	m_ProjectSettings = CProjectSettings::LoadFromStream(stream, context);
	if (m_ProjectSettings == null)
	{
		CLog::Log(PK_ERROR, "Failed to load project settings file at \"%s\"", projectSettingsFilePath.Data());
		return;
	}

	PProjectSettingsGeneral		general = m_ProjectSettings->General();
	const CString				relRoot = general->RootDir();

	m_PKPackPath = relRoot.Compare(".") ? CString(pKPackPath) : CString(pKPackPath) + "/" + relRoot;
	m_PKSourcePack = m_BakeContext.m_BakeFSController->MountPack(m_PKPackPath);

	CFilePath::Purify(m_PKPackPath);

	// Initialize the cookery:
	m_BakeContext.m_EditorCacheDirectory = general->EditorCacheDir();
	CString		cacheFolder = m_PKPackPath / m_BakeContext.m_EditorCacheDirectory;
	m_BakeContext.m_DstBakeTarget = PopcornFX::SBakeTarget("Unity_Dst", BAKE_DESTINATION_PATH);
	m_BakeContext.m_CacheBakeTarget = PopcornFX::SBakeTarget("Unity_Cache", cacheFolder);

	Mem::Reinit(m_BakeContext.m_Cookery);

	m_BakeContext.m_Cookery.SetHBOContext(m_BakeContext.m_BakeContext);
	if (!m_BakeContext.m_Cookery.TurnOn())
	{
		CLog::Log(PK_WARN, "Couldn't initialize the cookery, TurnOn Failed.");
		return;
	}

	const CGuid	ovenIdHBO = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_HBO));
	const CGuid	ovenIdMesh = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_Mesh));
	const CGuid	ovenIdTexture = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_Texture));
	const CGuid	ovenIdTextureAtlas = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_TextureAtlas));
	const CGuid	ovenIdVectorField = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_VectorField));

	COven_Particle	*ovenParticle = PK_NEW(COven_Particle);
	const CGuid	ovenIdParticle = m_BakeContext.m_Cookery.RegisterOven(ovenParticle);

	const CGuid	ovenIdStraightCopy = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_StraightCopy));
	const CGuid	ovenIdAudio = m_BakeContext.m_Cookery.RegisterOven(PK_NEW(COven_Audio));
	if (!ovenIdHBO.Valid() || !ovenIdMesh.Valid() || !ovenIdTexture.Valid() || !ovenIdTextureAtlas.Valid() ||
		!ovenIdVectorField.Valid() || !ovenIdParticle.Valid() || !ovenIdStraightCopy.Valid() ||
		!ovenIdAudio.Valid())
	{
		CLog::Log(PK_WARN, "Couldn't initialize the cookery, RegisterOven Failed.");
		return;
	}

	// map all known extensions to the appropriate oven:
	m_BakeContext.m_Cookery.MapOven("fbx", ovenIdStraightCopy);		// FBX mesh
	m_BakeContext.m_Cookery.MapOven("pkmm", ovenIdMesh);			// PopcornFX multi-mesh
	m_BakeContext.m_Cookery.MapOven("dds", ovenIdTexture);			// dds image
	m_BakeContext.m_Cookery.MapOven("png", ovenIdTexture);			// png image
	m_BakeContext.m_Cookery.MapOven("jpg", ovenIdTexture);			// jpg image
	m_BakeContext.m_Cookery.MapOven("jpeg", ovenIdTexture);			// jpg image
	m_BakeContext.m_Cookery.MapOven("tga", ovenIdTexture);			// tga image
	m_BakeContext.m_Cookery.MapOven("tif", ovenIdTexture);			// tiff image
	m_BakeContext.m_Cookery.MapOven("tiff", ovenIdTexture);			// tiff image
	m_BakeContext.m_Cookery.MapOven("pkm", ovenIdTexture);			// pkm image
	m_BakeContext.m_Cookery.MapOven("pvr", ovenIdTexture);			// pvrtc image
	m_BakeContext.m_Cookery.MapOven("exr", ovenIdTexture);			// exr image
	m_BakeContext.m_Cookery.MapOven("hdr", ovenIdTexture);			// hdr image
	m_BakeContext.m_Cookery.MapOven("txt", ovenIdStraightCopy);		// misc
	m_BakeContext.m_Cookery.MapOven("fga", ovenIdVectorField);		// FGA vector-field
	m_BakeContext.m_Cookery.MapOven("pkfm", ovenIdStraightCopy);	// PopcornFX font
	m_BakeContext.m_Cookery.MapOven("pkvf", ovenIdStraightCopy);	// PopcornFX vector-field
	m_BakeContext.m_Cookery.MapOven("pkat", ovenIdTextureAtlas);	// PopcornFX atlas definition
	m_BakeContext.m_Cookery.MapOven("pksc", ovenIdStraightCopy);	// PopcornFX simulation cache
	m_BakeContext.m_Cookery.MapOven("pkbo", ovenIdHBO);				// PopcornFX base object
	m_BakeContext.m_Cookery.MapOven("pkan", ovenIdHBO);				// PopcornFX Animation
	m_BakeContext.m_Cookery.MapOven("pksa", ovenIdHBO);				// PopcornFX Skeletal Animation
	m_BakeContext.m_Cookery.MapOven("pkfx", ovenIdParticle);		// PopcornFX Effect
	m_BakeContext.m_Cookery.MapOven("mp3", ovenIdAudio);			// mp3 sound
	m_BakeContext.m_Cookery.MapOven("wav", ovenIdAudio);			// wav sound
	m_BakeContext.m_Cookery.MapOven("ogg", ovenIdAudio);			// ogg sound

	if (!PK_VERIFY(m_BakeContext.m_Cookery.m_DstPackPaths.PushBack(m_BakeContext.m_DstBakeTarget).Valid()))
		return;

	m_BakeContext.m_Cookery.AddOvenFlags(PopcornFX::COven::Flags_BakeMemoryVersion);

	PopcornFX::PBaseObjectFile	configFile = m_BakeContext.m_Cookery.m_BaseConfigFile;
	PK_FOREACH(it, configFile->ObjectList())
	{
		PopcornFX::COvenBakeConfig_Particle	*config = PopcornFX::HBO::Cast<PopcornFX::COvenBakeConfig_Particle>(*it);
		if (config != null)
		{
			if (m_ForceDeterminism)
				config->SetCompilerSwitches("--determinism");
			config->SetCompile(true);
			config->SetRemoveEditorNodes(true);
			config->SetBakeMode(PopcornFX::COvenBakeConfig_HBO::Bake_SaveAsBinary);

			m_BakeConfigParticle = config;
			continue;
		}
		PopcornFX::COvenBakeConfig_Base	*configBase = PopcornFX::HBO::Cast<PopcornFX::COvenBakeConfig_Base>(*it);
		if (configBase != null)
		{
			continue;
		}
	}
}

//----------------------------------------------------------------------------

void			CEffectBaker::Clear()
{
	PK_SCOPEDLOCK(m_Lock);
	m_ToProcess.Clear();
}

//----------------------------------------------------------------------------

void				CEffectBaker::Lock()
{
	m_Lock.Lock();
}

void				CEffectBaker::Unlock()
{
	m_Lock.Unlock();
}

void			CEffectBaker::SetForceDeterminismFlag(bool enable)
{
	m_ForceDeterminism = enable;
}

//----------------------------------------------------------------------------

void			CEffectBaker::CancelAllFileChanges()
{
	m_ToBake.Clear();
	m_ToProcess.Clear();
}

//----------------------------------------------------------------------------

int				CEffectBaker::PopFileChanges()
{
	SAssetChangesDesc	assetChange;

	assetChange.m_Type = EAssetChangesType::Undefined;

	float			time = static_cast<float>(m_Timer.Read());
	const float		kBufferingTime = 0.2f;

	if (!m_ToBake.Empty())
		::OnEffectAboutToBake(m_ToBake.Last().m_EffectPath.Data());

	if (m_BakeConfigParticle != null)
	{
		if (m_ForceDeterminism)
			m_BakeConfigParticle->SetCompilerSwitches("--determinism");
	}

	if (m_ToBake.Count() != 0 && time > kBufferingTime)
	{
		SAssetChange effect = m_ToBake.Pop();

		if (BakeAssetOrAddToRetryStack(effect) == true)
		{
			if (IsChangeRegistered(effect.m_EffectPath, effect.m_Type) == false)
				m_ToProcess.PushBack(effect);
		}
		m_Timer.Reset();
		m_Timer.Pause();
	}
	if (m_ToProcess.Count() != 0)
	{
		SAssetChange bakedEffect = m_ToProcess.Pop();
		assetChange.m_Type = bakedEffect.m_Type;
		assetChange.m_Path = bakedEffect.m_EffectPath.Data();
		assetChange.m_PathOld = bakedEffect.m_EffectPathOld.Data();

		::OnAssetChange(&assetChange);
	}
	return m_ToBake.Count() + m_ToProcess.Count();
}

bool	CEffectBaker::IsChangeRegistered(const CString &path, EAssetChangesType type)
{
	for (u32 i = 0; i < m_ToProcess.Count(); ++i)
	{
		if (m_ToProcess[i].m_Type == type && path.Compare(m_ToProcess[i].m_EffectPath))
			return true;
	}
	for (u32 i = 0; i < m_ToBake.Count(); ++i)
	{
		if (m_ToBake[i].m_Type == type && path.Compare(m_ToBake[i].m_EffectPath))
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------

void	CEffectBaker::ReimportAssets(TArray<CString> &paths)
{
	for (u32 i = 0; i < paths.Count(); ++i)
	{
		FileChangedRelativePath(paths[i]);
	}
}

void	CEffectBaker::GetAllAssetPath()
{
	IFileSystem						*fs = m_BakeContext.m_BakeFSController;
	CPackExplorer					packExplorer(m_PKSourcePack->Path(), fs);
	CPackExplorer					effectExplorer(m_PKSourcePack->Path(), fs);
	PProjectSettingsGeneral			general = m_ProjectSettings->General();
	const CString					libraryDir = general->LibraryDir();
	const CString					editorCacheDir = general->EditorCacheDir();
	const CString					templatesDir = general->PresetsDir();
	SDirectoryValidator				directoryValidator(libraryDir, editorCacheDir, templatesDir, m_ProjectSettings->Assets(), m_PKSourcePack);
	CPackExplorer::PathValidator	directoryPathValidator = CPackExplorer::PathValidator(&(directoryValidator), &SDirectoryValidator::cmp);

	effectExplorer.SetDirectoryPathValidator(directoryPathValidator);
	effectExplorer.Explore();

	TMemoryView<const CString> paths = effectExplorer.EffectPaths();

	TArray<const char*> pathChar(paths.Count());
	for (u32 i = 0; i < paths.Count(); ++i)
	{
		pathChar[i] = paths[i].Data();
	}
	::OnGetAllAssetPath(pathChar.RawDataPointer(), paths.Count());
}

//----------------------------------------------------------------------------

bool	CEffectBaker::BakeAssetOrAddToRetryStack(SAssetChange &assetInfo)
{
	CUnityLog	*logger = CRuntimeManager::Instance().GetLogger();

	if (logger != null)
		logger->StartLogs();

	CLog::Log(PK_INFO, "Baking asset '%s' and its dependencies...", assetInfo.m_EffectPath.Data());

	if (BakeAsset(assetInfo.m_EffectPath, true) == false)
	{
		//On slow HDD, some bake failed is due to file not written entirely,
		//Add to Failed stack and retry later;
		++assetInfo.m_Try;
		if (assetInfo.m_Try < m_MaxBakeRetry)
		{
			m_ToBake.PushBack(assetInfo);
			if (logger != null)
				logger->InvalidateLogs();
		}
		else
		{
			if (logger != null)
				logger->ValidateLogs();
		}
		return false;
	}
	if (logger != null)
		logger->ValidateLogs();
	return true;
}

//----------------------------------------------------------------------------

bool	CEffectBaker::BakeAsset(const CString &path, bool bakeDependencies)
{
	if (bakeDependencies)
	{
		TArray<CString>	dependencies;
		CUnityLog	*logger = CRuntimeManager::Instance().GetLogger();

		// Cannot call 'GetAssetDependencies' without loading the dependent resources that are not baked yet
		// That's why we ignore the logs here to avoid the loading errors.
		if (logger != null)
			logger->StartIgnoringLogs();
		m_BakeContext.m_Cookery.GetAssetDependencies(path, dependencies);
		if (logger != null)
			logger->StopIgnoringLogs();
		for (u32 i = 0; i < dependencies.Count(); i++)
		{
			CString dependency = dependencies[i];
			if (dependency == null ||
				dependency.EndsWith(".pkma", CaseInsensitive) ||
				dependency.EndsWith(".pkfx", CaseInsensitive) ||
				dependency.EndsWith(".pkbo", CaseInsensitive))
			{
				continue;
			}
			if (dependency.EndsWith(".gr2", CaseInsensitive))
			{
				CLog::Log(PK_ERROR, "Asset Dependency of '%s' is a gr2 file and is not supported by the plugin", path.Data());
				return false;
			}
			if (BakeAsset(dependency, false) == false)
			{
				CLog::Log(PK_ERROR, "Asset Dependency of '%s' failed baking:	'%s'", path.Data(), dependency.Data());
				return false;
			}
		}
	}

	bool								needsBaking = false;
	TStaticCountedArray<CString, 4>		outputPaths;

	OutputBakedResourceInCache(path, outputPaths);
	if (!outputPaths.Empty())
	{
		SFileTimes	srcTimestamp;

		if (!m_BakeContext.m_BakeFSController->Timestamps(path, srcTimestamp))
		{
			CLog::Log(PK_ERROR, "Couldn't bake resource '%s':", path.Data());
			return false;
		}

		for (u32 i = 0; i < outputPaths.Count() && !needsBaking; ++i)
		{
			SFileTimes	dstTimestamp;

			if (!m_BakeContext.m_BakeFSController->Timestamps(m_BakeContext.m_CacheBakeTarget.m_TargetPath / outputPaths[i], dstTimestamp))
				needsBaking = true; // Cached bake file does not exist
			else if (dstTimestamp.m_LastWriteTime < srcTimestamp.m_LastWriteTime)
				needsBaking = true; // Cached bake file is not up to date
		}
		// Bake in source pack cache directory:
		m_BakeContext.m_Cookery.m_DstPackPaths.Last() = m_BakeContext.m_CacheBakeTarget;
	}
	else
	{
		needsBaking = true;
		m_BakeContext.m_Cookery.m_DstPackPaths.Last() = m_BakeContext.m_DstBakeTarget;
	}

	if (needsBaking)
	{
		PopcornFX::CMessageStream	bakerErrors;
		// bake the real asset
		if (!m_BakeContext.m_Cookery.BakeAsset(path, m_BakeContext.m_Cookery.m_BaseConfigFile, bakerErrors))
		{
			CLog::Log(PK_ERROR, "Couldn't bake effect '%s':", path.Data());
			bakerErrors.Log();
			return false;
		}
	}

	if (!outputPaths.Empty())
	{
		// Copy the baked files in the bake directory:
		for (u32 i = 0; i < outputPaths.Count(); ++i)
		{
			const CString	sourcePath = m_BakeContext.m_CacheBakeTarget.m_TargetPath / outputPaths[i]; // Already an absolute path
			const CString	targetPath = m_BakeContext.m_DstBakeTarget.m_TargetPath / outputPaths[i];
			const CString	targetDirectory = CFilePath::StripFilename(targetPath);
			m_BakeContext.m_BakeFSController->CreateDirectoryChainIFN(targetDirectory, true);
			m_BakeContext.m_BakeFSController->FileCopy(sourcePath, targetPath, true);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

void	CEffectBaker::OutputBakedResourceInCache(const CString path, TStaticCountedArray<CString, 4> &outputs)
{
	if (path.EndsWith(".fbx", CaseInsensitive))
	{
		outputs.PushBack(path);
		outputs.PushBack(CFilePath::StripExtension(path) + ".pkmm");
		outputs.PushBack(CFilePath::StripExtension(path) + ".pkan");
		outputs.PushBack(CFilePath::StripExtension(path) + ".pksa");
	}
	else if (path.EndsWith(".fga", CaseInsensitive))
	{
		outputs.PushBack(CFilePath::StripExtension(path) + ".pkvf");
	}
	else if (	path.EndsWith(".pkmm", CaseInsensitive) ||
				path.EndsWith(".pkan", CaseInsensitive) ||
				path.EndsWith(".pksa", CaseInsensitive) ||
				path.EndsWith(".pkvf", CaseInsensitive))
	{
		outputs.PushBack(path);
	}
}

//----------------------------------------------------------------------------
__PK_API_END

