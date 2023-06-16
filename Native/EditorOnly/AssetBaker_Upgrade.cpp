//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "AssetBaker.h"
#include <upg_upgrader.h>
#include <pk_version.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Project upgrade
//
//----------------------------------------------------------------------------

static SEngineVersion	_GetFileVersion(IFileSystem *fs, const CString &filepath)
{
	if (!fs->Exists(filepath, true))
		return SEngineVersion();

	const CString	fileBuffer = fs->BufferizeToString(filepath, true);	// Overkill, but don't care that much. pkproj files are pretty small anyway. Could read the first 100 bytes or something instead.
	if (fileBuffer == null)
		return SEngineVersion();

	return CRawBaseObjectFile::ExtractVersionIFP(fileBuffer);
}

//----------------------------------------------------------------------------

bool	CEffectBaker::UpgradeProject(IFileSystem *fs, const CString &projectSettingsAbsPath)
{
	CLog::Log(PK_INFO, "Upgrading project \"%s\"...\n", projectSettingsAbsPath.Data());
	const bool				forceUpgradeToContinue = true;	// Force-continue on error
	const SEngineVersion	currentV = BuildInfo().m_Version;

	PK_ASSERT(!projectSettingsAbsPath.Contains("\\"));
	if (!fs->Exists(projectSettingsAbsPath, true))
	{
		CLog::Log(PK_ERROR, "Pack not found '%s'\n", projectSettingsAbsPath.Data());
		return false;
	}

	// NOTE: We pass an IFileSystem in here for the operations we do outside of the upgrader,
	// but the upgrader needs to be modified to support a custom FS pointer to be "clean" and
	// not hardcoded with the default FS (even though it should default to the default FS if
	// no override is provided).
	CUpgrader		upgrader;
	CUpgradeState	upgradeState;

	const SEngineVersion	versionStart = _GetFileVersion(fs, projectSettingsAbsPath);

	if (!upgrader.Initialize(versionStart, currentV, m_BakeContext.m_BakeFSController))
		return false;
	upgradeState.SetFileSystem(m_BakeContext.m_BakeFSController);

	if (!upgrader.UpgradeProject(projectSettingsAbsPath, upgradeState, forceUpgradeToContinue /*, fs*/))
	{
		CLog::Log(PK_ERROR, "Upgrading Project '%s' failed\n", projectSettingsAbsPath.Data());
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
__PK_API_END
