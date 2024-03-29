//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "PKUnity_InterfaceCommon.h"

#include <pk_particles/include/ps_scene.h>

extern "C"
{
	struct SPopcornRendererDesc;

	struct	SFxEventDesc
	{
		s32				m_Slot;
		const char		*m_EventName;

	}; 

	// Attributes:
	struct	SFxAttributeDesc
	{
		enum	EAttrDescFlag
		{
			HasMin = 0x01,
			HasMax = 0x02,
		};

		int				m_AttributeType;
		int				m_AttributeSemantic;
		int				m_AttributeDropMode;
		int				m_MinMaxFlag;

		const char		*m_AttributeName;
		const u16		*m_Description;
		const char		*m_DropNameList; // List of string `|` separated

		CFloat4			m_DefaultValue;
		CFloat4			m_MinValue;
		CFloat4			m_MaxValue;
	};

	// Samplers:
	struct SFxSamplerDesc
	{
		int				m_SamplerType;
		const char		*m_SamplerName;
		const u16		*m_Description;
		int				m_SamplerUsageFlags;

		// For sampler shape:
		CQuaternion		m_ShapeRotation;
		CFloat3			m_ShapePosition;

		// For sampler curve:
		u32				m_CurveDimension;
		u32				m_CurveKeyCount;
		const float		*m_CurveTimes;
		const float		*m_CurveFloatValues;
		const float		*m_CurveFloatTangents;

		SFxSamplerDesc()
		:	m_SamplerType(SamplerUnsupported)
		,	m_SamplerName(null)
		,	m_Description(null)
		,	m_SamplerUsageFlags(0)
		,	m_ShapeRotation(CQuaternion::IDENTITY)
		,	m_ShapePosition(0)
		,	m_CurveDimension(0)
		,	m_CurveKeyCount(0)
		,	m_CurveTimes(null)
		,	m_CurveFloatValues(null)
		,	m_CurveFloatTangents(null)
		{
		}
	};

	enum EAssetChangesType : int
	{
		Undefined = 0,
		Add,
		Remove,
		Update,
		Rename
	};

	struct SAssetChangesDesc
	{
		const char					*m_Path;
		const char					*m_PathOld;
		int							m_Type;
	};

	struct SAssetImportError
	{
		const char					*m_Path;
		const char					*m_ErrorMessage;
	};

	enum EUseInfoFlag : int
	{
		// Renderers:
		IsLinearTextureRenderer		= (1 << 0),
		IsTextureRenderer			= (1 << 1),
		IsMeshRenderer				= (1 << 2),
		// Samplers:
		IsMeshSampler				= (1 << 3),
		IsTextureSampler			= (1 << 4),

		IsLookupTexture				= (1 << 5),

		IsThumbnail					= (1 << 6), //Used by the Editor
		IsAnimatedThumbnail			= (1 << 7), //Used by the Editor
	};

	enum EPrintConsoleType
	{
		ErrorMessage,
		WarningMessage,
		InfoMessage
	};

	//----------------------------------------------------------------------------
	// Callbacks:
	//----------------------------------------------------------------------------

	// Import effect callbacks:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectDependencyFound(void *delegatePtr);
	void										OnEffectDependencyFound(const char *dependencyPath, int useInfoFlags);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectRendererFound(void *delegatePtr);
	void										OnEffectRendererFound(const void *effectRdrDesc, int type, int idx);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectRendererLink(void* delegatePtr);
	void										OnEffectRendererLink(int globalIdx, const char *currentQualityLevel, int uid);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectEventFound(void *delegatePtr);
	void										OnEffectEventFound(const SFxEventDesc *attribDesc);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectAttributeFound(void *delegatePtr);
	void										OnEffectAttributeFound(const SFxAttributeDesc *attribDesc);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectSamplerFound(void *delegatePtr);
	void										OnEffectSamplerFound(const SFxSamplerDesc *samplerDesc);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetEffectInfo(void *delegatePtr);
	void										OnGetEffectInfo(int usesMeshParticles);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectAboutToBake(void *delegatePtr);
	void										OnEffectAboutToBake(const char *effectName);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAssetChange(void *delegatePtr);
	void										OnAssetChange(const SAssetChangesDesc *assetChanges);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetAllAssetPath(void *delegatePtr);
	void										OnGetAllAssetPath(const char **pathArray, int size);

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnPkkgExtracted(void *delegatePtr);
	void										OnPkkgExtracted(const char *dirPath);

	void										ClearNativeToManagedEditorCallbacks();
}

//----------------------------------------------------------------------------
