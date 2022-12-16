//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RuntimeManager.h"
#include "FxStartup.h"
#include "PKUnity_Scene.h"
#include "FxEffect.h"
#include "PKUnity_FileSystemController.h"
#include "PKUnity_ImageResourceManager.h"
#include "PKUnity_Plugins.h"
#include "SingleThreadedPool.h"
#include "NativeToManaged.h"
#include "ResourceHandleDummy.h"

#if	PK_UNITY_EDITOR
#	include "EditorOnly/FileWatcher.h"
#	include "EditorOnly/MeshBaking.h"
#endif

#include <pk_version.h>
#include <pk_kernel/include/kr_plugins.h>
#include <pk_kernel/include/kr_static_config_flags.h>

// PopcornFX init
#include <pk_kernel/include/kr_init.h>
#include <pk_kernel/include/kr_log_listeners.h>
#include <pk_base_object/include/hb_init.h>
#include <pk_engine_utils/include/eu_init.h>
#include <pk_compiler/include/cp_init.h>
#include <pk_imaging/include/im_init.h>
#include <pk_geometrics/include/ge_init.h>
#include <pk_particles/include/ps_init.h>
#include <pk_particles_toolbox/include/pt_init.h>
#include <pk_kernel/include/kr_assert_internals.h>
#include <pk_kernel/include/kr_resources.h>
#include <pk_kernel/include/kr_log_listeners_file.h>
#include <pk_kernel/include/kr_thread_pool_default.h>

#include <pk_particles/include/ps_samplers.h>
#include <pk_particles/include/ps_samplers_classes.h>
#include <pk_maths/include/pk_maths_type_converters.h>
#include <pk_base_object/include/hbo_helpers.h>

//#include <pk_geometrics/include/ge_billboards.h>	// for CBillboarderConfig

#include <pk_render_helpers/include/rh_init.h>
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_skeletal.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_static.h>

#if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)
#	include "UnityGraphicsAPI/IUnityGraphicsD3D11.h"
#endif
#if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)
#	include	"UnityGraphicsAPI/IUnityGraphicsD3D12.h"
#endif
#if	(PK_BUILD_WITH_VULKAN_SUPPORT != 0)
#	include "UnityGraphicsAPI/IUnityGraphicsVulkan.h"
#	include "ImplemGraphicsAPI/RenderAPI_Data.h"
#endif
#if	defined(PK_ORBIS)
#	include "UnityGraphicsAPI/IUnityGraphicsPS4.h"
#	include <gnm.h>
#elif defined(PK_UNKNOWN2)
#	include "UnityGraphicsAPI/IUnityGraphicsUNKNOWN2.h"
#	include <UNKNOWN2>
#endif
#if	defined(PK_LINUX) || defined(PK_ANDROID) || defined(PK_IOS)
#include <unistd.h>
#endif
#if	defined(PK_NX)
#include <nvn/nvn_FuncPtrInline.h>
#include "UnityGraphicsAPI/IUnityGraphicsNvn.h"
#endif

#if	defined(PK_MACOSX)
namespace PKFX
{
	Assert::EResult AssertCatcher_LogThenNativeOsxAlert(PK_ASSERT_CATCHER_PARAMETERS);
}
#endif

__PK_API_BEGIN

//----------------------------------------------------------------------------
// Logs and Asserts handlers:
//----------------------------------------------------------------------------

#if	defined(PK_ANDROID) || defined(PK_IOS)
namespace
{
	const char			*g_AssertScriptFilePath = 0;

	Assert::EResult	CbAssertCatcherSkip(PK_ASSERT_CATCHER_PARAMETERS)
	{
		fprintf(stderr, __PK_ASSERT_MESSAGE(file, function, line, failed, expanded, message));

		char		cmd[2048];
		SNativeStringUtils::SPrintf(cmd, "%s %d \"%s\" \"%s\" \"%d\" \"%s\" \"%s\" \"%s\"",
			g_AssertScriptFilePath, getpid(), PK_ASSERT_CATCHER_ARGUMENTS);
		return Assert::Result_Skip;
	}
}
#elif	defined(PK_NX)
namespace
{
	const char			*g_AssertScriptFilePath = 0;

	Assert::EResult	CbAssertCatcherSkip(PK_ASSERT_CATCHER_PARAMETERS)
	{
		fprintf(stderr, __PK_ASSERT_MESSAGE(file, function, line, failed, expanded, message));

		char		cmd[2048];
		SNativeStringUtils::SPrintf(cmd, "%s \"%s\" \"%s\" \"%d\" \"%s\" \"%s\" \"%s\"",
			g_AssertScriptFilePath, PK_ASSERT_CATCHER_ARGUMENTS);
		return Assert::Result_Skip;
	}
}
#elif	defined(PK_LINUX)
namespace
{
	const char		*g_AssertScriptFilePath = 0;

	Assert::EResult	CbAssertCatcherBreaker(PK_ASSERT_CATCHER_PARAMETERS)
	{
		fprintf(stderr, __PK_ASSERT_MESSAGE(file, function, line, failed, expanded, message));
		return Assert::Result_Break;
	}

	Assert::EResult	CbAssertCatcher(PK_ASSERT_CATCHER_PARAMETERS)
	{
		fprintf(stderr, __PK_ASSERT_MESSAGE(file, function, line, failed, expanded, message));

		char		cmd[2048];
		SNativeStringUtils::SPrintf(cmd, "%s %d \"%s\" \"%s\" \"%d\" \"%s\" \"%s\" \"%s\"",
			g_AssertScriptFilePath, getpid(), file, function, line, message, failed, expanded);
		int			res = system(cmd);
		if (res >= 0)
		{
			if (WIFSIGNALED(res))
			{
				Assert::SetCatcher(CbAssertCatcherBreaker);
				return Assert::Result_Break;
			}
			else
			{
				int		status = WEXITSTATUS(res);
				switch (status)
				{
				case 1: return Assert::Result_Break;
				case 2: return Assert::Result_Ignore;
				case 3:
					Assert::SetCatcher(CbAssertCatcherBreaker);
					return Assert::Result_Break;
				}
			}
		}
		return Assert::Result_Skip;
	}
}
#endif

//----------------------------------------------------------------------------

void	AddDefaultLogListenersOverride(void *userHandle)
{
#if	(PK_LOG_ENABLED != 0)
	CRuntimeManager::SPopcornFXRuntimeData		*runtimeData = static_cast<CRuntimeManager::SPopcornFXRuntimeData*>(userHandle);
	runtimeData->m_UnityLogger = PK_NEW(CUnityLog);
	if (!PK_VERIFY(runtimeData->m_UnityLogger != null))
		return;
	CLog::AddGlobalListener(runtimeData->m_UnityLogger);
#endif	// (PK_LOG_ENABLED != 0)
}

//----------------------------------------------------------------------------
// Spectrum descriptor:
//----------------------------------------------------------------------------

CRuntimeManager::SSpectrumDescriptor::SSpectrumDescriptor(CStringId name)
:	m_RegisteredName(name)
,	m_Spectrum(null)
,	m_Waveform(null)
,	m_NeedsSpectrum(false)
,	m_NeedsWaveform(false)
,	m_HasSpectrum(false)
,	m_HasWaveform(false)
,	m_SpectrumSize(1024)
,	m_WaveformSize(1024)
{
	_CleanAudioPyramid(m_SpectrumPyramid);
	_CleanAudioPyramid(m_WaveformPyramid);
}

//----------------------------------------------------------------------------

CRuntimeManager::SSpectrumDescriptor::~SSpectrumDescriptor()
{
	PK_FREE(m_Spectrum);
	PK_FREE(m_Waveform);

	_CleanAudioPyramid(m_SpectrumPyramid);
	_CleanAudioPyramid(m_WaveformPyramid);
}

//----------------------------------------------------------------------------

void	CRuntimeManager::SSpectrumDescriptor::_CleanAudioPyramid(TArray<float*> &pyramid)
{
	for (u32 i = 1; i < pyramid.Count(); i++)
	{
		PK_FREE(pyramid[i]);
	}
	pyramid.Clean();
}

//----------------------------------------------------------------------------
// PKFX manager:
//----------------------------------------------------------------------------

TMemoryView<const float * const>	CRuntimeManager::GetSpectrumPyramid(CStringId name, u32 &outBaseCount)
{
	PK_SCOPEDLOCK(m_SpectrumLock);

	SSpectrumDescriptor	*kFoundDesc = null;
	for (u32 i = 0; i < m_SpectrumDescriptorList.Count(); i++)
	{
		SSpectrumDescriptor	*desc = &m_SpectrumDescriptorList[i];
		if (desc->m_RegisteredName == name)
		{
			kFoundDesc = desc;
		}
	}

	if (kFoundDesc == null)
	{
		CGuid kID = m_SpectrumDescriptorList.PushBack(SSpectrumDescriptor(name));
		if (!kID.Valid())
			goto	_notFound;
		kFoundDesc = &m_SpectrumDescriptorList[kID];
	}

	kFoundDesc->m_NeedsSpectrum = true;
	if (kFoundDesc->m_HasSpectrum)
	{
		PK_ASSERT(!kFoundDesc->m_SpectrumPyramid.Empty());
		outBaseCount = kFoundDesc->m_SpectrumSize;
		return TMemoryView<const float * const>(kFoundDesc->m_SpectrumPyramid.RawDataPointer(), kFoundDesc->m_SpectrumPyramid.Count());
	}

_notFound:
	outBaseCount = 0;
	return TMemoryView<const float * const>();
}

//----------------------------------------------------------------------------

TMemoryView<const float * const>	CRuntimeManager::GetWaveformPyramid(CStringId name, u32 &outBaseCount)
{
	PK_SCOPEDLOCK(m_SpectrumLock);

	SSpectrumDescriptor	*kFoundDesc = null;
	for (u32 i = 0; i < m_SpectrumDescriptorList.Count(); i++)
	{
		SSpectrumDescriptor	*desc = &m_SpectrumDescriptorList[i];
		if (desc->m_RegisteredName == name)
		{
			kFoundDesc = desc;
		}
	}

	if (kFoundDesc == null)
	{
		CGuid kID = m_SpectrumDescriptorList.PushBack(SSpectrumDescriptor(name));
		if (!kID.Valid())
			goto	_notFound;
		kFoundDesc = &m_SpectrumDescriptorList[kID];
	}

	kFoundDesc->m_NeedsWaveform = true;
	if (kFoundDesc->m_HasWaveform)
	{
		PK_ASSERT(!kFoundDesc->m_WaveformPyramid.Empty());
		outBaseCount = kFoundDesc->m_WaveformSize;
		return TMemoryView<const float * const>(kFoundDesc->m_WaveformPyramid.RawDataPointer(), kFoundDesc->m_WaveformPyramid.Count());
	}

_notFound:
	outBaseCount = 0;
	return TMemoryView<const float * const>();
}

//----------------------------------------------------------------------------

static float	*_BuildAudioPyramid(const float *srcBuffer, u32 bufferSize, float *dstBuffer, TArray<float*> &pyramid)
{
	PK_ASSERT(srcBuffer != null);

	// lazy-allocation
	if (dstBuffer == null)
	{
		CRuntimeManager::SSpectrumDescriptor::_CleanAudioPyramid(pyramid);

		const u32	baseAllocSize = bufferSize;
		// allocate two double borders to avoid checking for overflow during sampling w/ Cubic or Linear filters
		const u32	baseByteCount = (2 + baseAllocSize + 2) * sizeof(*dstBuffer);
		dstBuffer = (float*)PK_MALLOC_ALIGNED(baseByteCount, 0x80);
		if (dstBuffer == null)
			return null;
		Mem::Clear(dstBuffer, baseByteCount);

		bool		success = true;
		const u32	pyramidSize = IntegerTools::Log2(bufferSize) + 1;
		if (pyramid.Resize(pyramidSize))
		{
			u32	currentCount = bufferSize;
			for (u32 i = 1; i < pyramidSize; i++)
			{
				currentCount >>= 1;
				PK_ASSERT(currentCount != 0);
				const u32	mipByteCount = (2 + currentCount + 2) * sizeof(float);
				pyramid[i] = (float*)PK_MALLOC_ALIGNED(mipByteCount, 0x10);
				if (pyramid[i] != null)
					Mem::Clear(pyramid[i], mipByteCount);
				success &= (pyramid[i] != null);
			}
		}
		pyramid[0] = dstBuffer;

		if (!success)
		{
			PK_FREE(dstBuffer);
			CRuntimeManager::SSpectrumDescriptor::_CleanAudioPyramid(pyramid);
			return null;
		}
	}

	PK_ASSERT(dstBuffer != null);
	float	*realDataPtr = dstBuffer + 2;	// ptr to the first real element, skipping the two-element border
	memcpy(realDataPtr, srcBuffer, bufferSize * sizeof(*dstBuffer));

	{
		const float	firstEntry = realDataPtr[0];
		const float	lastEntry  = realDataPtr[bufferSize - 1];
		realDataPtr[-1] = firstEntry; // duplicate the first entry in the two start borders
		realDataPtr[-2] = firstEntry;
		realDataPtr[bufferSize + 0] = lastEntry; // duplicate the last entry in the two end borders
		realDataPtr[bufferSize + 1] = lastEntry;
	}

	// right, rebuild the spectrum pyramid:
	if (!pyramid.Empty())
	{
		u32	currentCount = bufferSize;
		for (u32 i = 1; i < pyramid.Count(); i++)
		{
			const float	* __restrict src = 2 + pyramid[i - 1];
			float		* __restrict dst = 2 + pyramid[i];
			currentCount >>= 1;

			// downsample
			for (u32 j = 0; j < currentCount; j++)
			{
				dst[j] = 0.5f * (src[j*2 + 0] + src[j*2 + 1]);
			}

			const float	firstEntry = dst[0];
			const float	lastEntry  = dst[currentCount - 1];
			dst[-1] = firstEntry;				// duplicate the first entry in the two start borders
			dst[-2] = firstEntry;
			dst[currentCount + 0] = lastEntry;	// duplicate the last entry in the two end borders
			dst[currentCount + 1] = lastEntry;
		}
	}

	return dstBuffer;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::Update(float dt)
{
	// here, handle the effect's velocity (used by effects using the 'InheritInitialVelocity' feature.
	// loop through all effect instances, and compute the instantaneous velocity using the dt and the previous/cur transforms:
	TSlotArray<CPKFXEffect*>::Iterator it = m_Effects.Begin();
	for (; it != m_Effects.End(); ++it)
	{
		(*it)->Update(dt);
	}
	// Update the audio spectrum:
	{
		PK_SCOPEDLOCK(m_SpectrumLock);

		for (u32 i = 0; i < m_SpectrumDescriptorList.Count(); i++)
		{
			SSpectrumDescriptor	*desc = &m_SpectrumDescriptorList[i];

			desc->m_HasSpectrum = false;
			if (desc->m_NeedsSpectrum)
			{
				const u32		prevSize = desc->m_SpectrumSize;
				const float		*data = (const float *)::OnGetAudioSpectrumData(desc->m_RegisteredName.ToString().Data(), (int*)&desc->m_SpectrumSize);
				if (data != null)
				{
					if (prevSize != desc->m_SpectrumSize)
					{
						PK_FREE(desc->m_Spectrum);
						CRuntimeManager::SSpectrumDescriptor::_CleanAudioPyramid(desc->m_SpectrumPyramid);
					}

					desc->m_Spectrum = _BuildAudioPyramid(data, desc->m_SpectrumSize, desc->m_Spectrum, desc->m_SpectrumPyramid);
					desc->m_HasSpectrum = (desc->m_Spectrum != null);
				}
				else
				{
					CLog::Log(PK_ERROR, "_OnGetAudioSpectrumDataFn returned null");
				}
			}

			desc->m_HasWaveform = false;
			if (desc->m_NeedsWaveform)
			{
				const u32		prevSize = desc->m_WaveformSize;
				const float		*data = (const float *)::OnGetAudioWaveformData(desc->m_RegisteredName.ToString().Data(), (int*)&desc->m_WaveformSize);
				if (data != null)
				{
					if (prevSize != desc->m_WaveformSize)
					{
						PK_FREE(desc->m_Waveform);
						CRuntimeManager::SSpectrumDescriptor::_CleanAudioPyramid(desc->m_WaveformPyramid);
					}

					desc->m_Waveform = _BuildAudioPyramid(data, desc->m_WaveformSize, desc->m_Waveform, desc->m_WaveformPyramid);
					desc->m_HasWaveform = (desc->m_Waveform != null);
				}
				else
				{
					CLog::Log(PK_ERROR, "_OnGetAudioWaveformDataFn returned null");
				}
			}
		}
	} // SCOPEDLOCK
}

//----------------------------------------------------------------------------

CRuntimeManager				*CRuntimeManager::m_Instance = null;
const SPopcornFxSettings	*CRuntimeManager::SPopcornFXRuntimeData::m_Settings = null;

//----------------------------------------------------------------------------

CRuntimeManager::CRuntimeManager()
	: m_PopcornFXRuntimeData(null)
	, m_Unloading(false)
	, m_IsUnitTesting(false)
	, m_UnityInterfaces(null)
	, m_UnityGraphics(null)
	, m_DeviceType(kUnityGfxRendererNull)
#if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)
	, m_DeviceD3D11(null)
	, m_ContextD3D11(null)
#endif
#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	, m_UnityVulkan(null)
#endif
#if	defined(PK_ORBIS)
	, m_ContextPS4(null)
#elif defined(PK_UNKNOWN2)
	, m_ContextUNKNOWN2(null)
#endif
	, m_HasBackgroundTasksToKick(false)
{
}

//----------------------------------------------------------------------------

CRuntimeManager::~CRuntimeManager()
{
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::IsInstanceInitialized()
{
	return m_Instance != null;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::InitializeInstanceIFN(const SPopcornFxSettings *settings)
{
	if (!IsInstanceInitialized())
	{
		// Startup PopcornFX:
		SPopcornFXRuntimeData	*runtimeData = new SPopcornFXRuntimeData();

		if (!PK_VERIFY(runtimeData != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate SPopcornFXRuntimeData");
			return false;
		}

		SPopcornFXRuntimeData::m_Settings = settings;
		runtimeData->PopcornFXStartup();
		SPopcornFXRuntimeData::m_Settings = null; // We avoid keeping a pointer to invalid settings after the startup

		// Create the CPKFX manager instance:
		m_Instance = PK_NEW(CRuntimeManager());

		if (!PK_VERIFY(m_Instance != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CRuntimeManager");
			return false;
		}
		if (settings != null)
			m_Instance->m_IsUnitTesting = (settings->m_IsUnitTesting == ManagedBool_True);
		// Setup the runtime data:
		m_Instance->m_PopcornFXRuntimeData = runtimeData;
		// Create the scene instance:
		m_Instance->m_ParticleScene = PK_NEW(CPKFXScene());
		if (!PK_VERIFY(m_Instance->m_ParticleScene != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate the PKFXScene");
			return false;
		}
		// Initialize the scene instance:
		if (!PK_VERIFY(m_Instance->m_ParticleScene->InitializeInstanceIFN(settings)))
		{
			CLog::Log(PK_ERROR, "Could not initialize the PKFXScene");
			return false;
		}
	}
	return true;
}

void	CRuntimeManager::SetMaxCameraCount(int count)
{
	if (!PK_VERIFY(count > 0))
		return;
	CParticleMediumCollection		*medCol = m_ParticleScene->GetParticleMediumCollection(false);
	CParticleMediumCollection		*medColMeshes = m_ParticleScene->GetParticleMediumCollection(true);
	TArray<SUnitySceneView>				&sceneViews = m_ParticleScene->SceneViewsForUpdate();

	for (u32 i = 0; i < sceneViews.Count(); ++i)
	{
		medCol->UnregisterView(sceneViews[i].m_UserData.m_CamSlotIdxInMedCol);
		if (medCol != medColMeshes)
			medColMeshes->UnregisterView(sceneViews[i].m_UserData.m_CamSlotIdxInMeshMedCol);
	}

	if (!sceneViews.Resize(count))
		return;

	for (u32 i = 0; i < sceneViews.Count(); ++i)
	{
		sceneViews[i].m_UserData.m_CamSlotIdxInMedCol = medCol->RegisterView();
		if (medCol != medColMeshes)
			sceneViews[i].m_UserData.m_CamSlotIdxInMeshMedCol = medColMeshes->RegisterView();
	}
}

//----------------------------------------------------------------------------

CRuntimeManager	&CRuntimeManager::Instance()
{
	PK_ASSERT(m_Instance != null && m_Instance->m_ParticleScene != null);
	return *m_Instance;
}

//----------------------------------------------------------------------------

SMeshDataToFill	*CRuntimeManager::CreateMeshDataToFill(int vertexCount, int indexCount, int bonesCount, int vertexAttributes)
{
	// Create the vertex layout:
	u32				sizeToAlloc = sizeof(SMeshDataToFill);

	sizeToAlloc += sizeof(u32) * indexCount + 0x10; // Index buffer size
	// The position is always here:
	sizeToAlloc += vertexCount * sizeof(CFloat4) + 0x10; // + 0x10 for alignment
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Normal))
		sizeToAlloc += vertexCount * sizeof(CFloat4) + 0x10; // + 0x10 for alignment
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Tangent))
		sizeToAlloc += vertexCount * sizeof(CFloat4) + 0x10; // + 0x10 for alignment
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Texcoord))
		sizeToAlloc += vertexCount * sizeof(CFloat2) + 0x10; // + 0x10 for alignment
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Color))
		sizeToAlloc += vertexCount * sizeof(CFloat4) + 0x10; // + 0x10 for alignment
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_BonesIndicesAndWeights))
	{
		// 4 indices and 4 weights per vertex
		sizeToAlloc += vertexCount * 4 * sizeof(u32) + 0x10;
		sizeToAlloc += vertexCount * 4 * sizeof(float) + 0x10;
	}

	// All in one alloc:
	void		*bufferPtr = PK_MALLOC_ALIGNED(sizeToAlloc, 0x10);

	if (!PK_VERIFY(bufferPtr != null))
		return null;

	SMeshDataToFill			*meshSamplerToFill = static_cast<SMeshDataToFill*>(bufferPtr);

	Mem::Construct(*meshSamplerToFill);
	meshSamplerToFill->m_IdxCount = indexCount;
	meshSamplerToFill->m_VtxCount = vertexCount;
	bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(SMeshDataToFill));
	bufferPtr = Mem::Align(bufferPtr, 0x10);
	meshSamplerToFill->m_Indices = static_cast<u32*>(bufferPtr);
	bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(u32) * indexCount);
	bufferPtr = Mem::Align(bufferPtr, 0x10);
	meshSamplerToFill->m_Positions = static_cast<CFloat3*>(bufferPtr);
	bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(CFloat3) * vertexCount);
	bufferPtr = Mem::Align(bufferPtr, 0x10);
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Normal))
	{
		meshSamplerToFill->m_Normals = static_cast<CFloat3*>(bufferPtr);
		bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(CFloat3) * vertexCount);
		bufferPtr = Mem::Align(bufferPtr, 0x10);
	}
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Tangent))
	{
		meshSamplerToFill->m_Tangents = static_cast<CFloat4*>(bufferPtr);
		bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(CFloat4) * vertexCount);
		bufferPtr = Mem::Align(bufferPtr, 0x10);
	}
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Texcoord))
	{
		meshSamplerToFill->m_UVs = static_cast<CFloat2*>(bufferPtr);
		bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(CFloat2) * vertexCount);
		bufferPtr = Mem::Align(bufferPtr, 0x10);
	}
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_Color))
	{
		meshSamplerToFill->m_Colors = static_cast<CFloat4*>(bufferPtr);
		bufferPtr = Mem::AdvanceRawPointer(bufferPtr, sizeof(CFloat4) * vertexCount);
		bufferPtr = Mem::Align(bufferPtr, 0x10);
	}
	if (vertexAttributes & (1 << EMeshVertexAttributes::Attrib_BonesIndicesAndWeights))
	{
		meshSamplerToFill->m_VtxBonesWeights = static_cast<float*>(bufferPtr);
		bufferPtr = Mem::AdvanceRawPointer(bufferPtr, 4 * sizeof(float) * vertexCount);
		bufferPtr = Mem::Align(bufferPtr, 0x10);
		meshSamplerToFill->m_VtxBonesIndices = static_cast<u32*>(bufferPtr);
	}
	else
	{
		meshSamplerToFill->m_VtxBonesWeights = null;
		meshSamplerToFill->m_VtxBonesIndices = null;
	}
	meshSamplerToFill->m_BonesCount = bonesCount;
	return meshSamplerToFill;
}

//----------------------------------------------------------------------------

CPKFXScene	&CRuntimeManager::GetScene() const
{
	return *m_ParticleScene;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::PopcornFXChangeSettings(const SPopcornFxSettings &settings)
{
	settings.PrettyPrintSettings();
	CRuntimeManager::SPopcornFXRuntimeData::m_Settings = &settings;
	m_PopcornFXRuntimeData->m_GPUBillboarding = settings.m_EnableGPUBillboarding == ManagedBool_True ? true : false;
	Scheduler::SetThreadPool(_CreateCustomThreadPool());
	CRuntimeManager::SPopcornFXRuntimeData::m_Settings = null;

	m_IsUnitTesting = (settings.m_IsUnitTesting == ManagedBool_True);
#if	(PK_LOG_ENABLED != 0)
	if (settings.m_EnableFileLog == ManagedBool_True)
	{
		if (m_PopcornFXRuntimeData->m_FileLogger == null)
		{
			FILE	*file = fopen("popcorn.htm", "a+");
			if (file != null)
			{
				fclose(file);
				m_PopcornFXRuntimeData->m_FileLogger = PK_NEW(CLogListenerFile("popcorn.htm", "popcorn-engine logfile"));
				if (PK_VERIFY(m_PopcornFXRuntimeData->m_FileLogger != null))
					CLog::AddGlobalListener(m_PopcornFXRuntimeData->m_FileLogger);
			}
		}
	}
	else
	{
		if (m_PopcornFXRuntimeData->m_FileLogger != null)
		{
			CLog::RemoveGlobalListener(m_PopcornFXRuntimeData->m_FileLogger);
			PK_SAFE_DELETE(m_PopcornFXRuntimeData->m_FileLogger);
		}
	}
#else
	if (settings.m_EnableFileLog == ManagedBool_True)
	{
		// Loggers disabled in this build configuration (usually retail builds)
		// Notify the user somehow? probably not.
	}
#endif	// (PK_LOG_ENABLED != 0)

	return m_ParticleScene->PopcornFXChangeSettings(settings);
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::PopcornFXShutdown()
{
	m_SpectrumDescriptorList.Clean();

	if (m_UnityGraphics != null)
	{
		m_UnityGraphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
	}

	m_Unloading = true;
	File::DefaultFileSystem()->UnmountAllPacks();
	m_Unloading = false;

	// We keep a reference on the PopcornFX runtime data before deleting it:
	SPopcornFXRuntimeData	*runtimeData = m_PopcornFXRuntimeData;

	// We delete the current instance:
	PK_SAFE_DELETE(m_Instance);

	if (runtimeData != null)
	{
		runtimeData->PopcornFXShutdown();
		delete runtimeData;
		runtimeData = null;
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::SetUnityInterfaces(IUnityInterfaces* unityInterfaces)
{
	m_UnityInterfaces = unityInterfaces;
	if (m_UnityInterfaces != null)
	{
		m_UnityGraphics = m_UnityInterfaces->Get<IUnityGraphics>();

		if (!PK_VERIFY(m_UnityGraphics != null))
		{
			CLog::Log(PK_ERROR, "m_UnityInterfaces->Get<IUnityGraphics>() returned null");
			return false;
		}
		m_UnityGraphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
#if defined(PK_NX)
		m_UnityNVNGraphics = m_UnityInterfaces->Get<IUnityGraphicsNvn_v2>();
#elif defined(PK_ORBIS)
		m_PS4Interface = m_UnityInterfaces->Get<IUnityGraphicsPS4>();
#elif defined(PK_UNKNOWN2)
		m_UNKNOWN2Interface = m_UnityInterfaces->Get<IUnityGraphicsUNKNOWN2>();
#endif
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::SetUnityRendererType(UnityGfxRenderer type)
{
	m_DeviceType = type;
}

//----------------------------------------------------------------------------

IUnityInterfaces	*CRuntimeManager::GetUnityInterfaces()
{
	return m_UnityInterfaces;
}

//----------------------------------------------------------------------------

IUnityGraphics	*CRuntimeManager::GetUnityGraphics()
{
	return m_UnityGraphics;
}

//----------------------------------------------------------------------------

#if	defined(PK_NX)
IUnityGraphicsNvn	*CRuntimeManager::GetNVNUnityGraphics()
{
	return m_UnityNVNGraphics;
}
#endif

//----------------------------------------------------------------------------

bool	CRuntimeManager::UnityAPIDeviceInitialize()
{
	m_DeviceType = m_UnityGraphics->GetRenderer();

	switch (m_DeviceType)
	{
#if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)
	case	kUnityGfxRendererD3D11:
		{
			IUnityGraphicsD3D11	*d3d11Interface = m_UnityInterfaces->Get<IUnityGraphicsD3D11>();
			m_DeviceD3D11 = (D3DDevice*)d3d11Interface->GetDevice();
#if defined(PK_DURANGO)
			m_DeviceD3D11->GetImmediateContextX(&m_ContextD3D11);
#else
			m_DeviceD3D11->GetImmediateContext(&m_ContextD3D11);
#endif
			m_ContextD3D11->Release(); // Not sure if we should do that or release this handle on the kUnityGfxDeviceEventShutdown
		}
		break;
#endif // BUILD_WITH_D3D11_SUPPORT
#if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)
	case	kUnityGfxRendererD3D12:
		{
			IUnityGraphicsD3D12v5		*d3d12Interface = m_UnityInterfaces->Get<IUnityGraphicsD3D12v5>();
			m_DeviceD3D12 = (ID3D12Device*)d3d12Interface->GetDevice();
		}
		break;
#endif
#if	defined(PK_ORBIS)
	case	kUnityGfxRendererPS4:
		{
			CLog::Log(PK_INFO, "Initialize Orbis Graphics API");
			m_PS4Interface = m_UnityInterfaces->Get<IUnityGraphicsPS4>();
			m_ContextPS4 = (GnmPS4ContextLW*)m_PS4Interface->GetGfxContext();
		}
		break;
#endif // PK_ORBIS
#if	defined(PK_UNKNOWN2)
	case	kUnityGfxRendererUNKNOWN2:
		{
			CLog::Log(PK_INFO, "Initialize UNKNOWN2 Graphics API");
			m_UNKNOWN2Interface = m_UnityInterfaces->Get<IUnityGraphicsUNKNOWN2>();
			m_ContextUNKNOWN2 = (UNKNOWN2ContextLW*)m_UNKNOWN2Interface->GetGfxContext();
		}
	break;
#endif // PK_UNKNOWN2
#if	(PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	case	kUnityGfxRendererVulkan:
	{
		CLog::Log(PK_INFO, "Initialize Vulkan Graphics API");
		m_UnityVulkan = m_UnityInterfaces->Get<IUnityGraphicsVulkan>();
		break;
	}
#endif
	default:
		break;
	}
	return m_ParticleScene->UnityAPIDeviceIsInitialized(m_DeviceType);
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::UnityAPIDeviceUnInitialize()
{
	if (m_UnityGraphics != null)
		m_DeviceType = m_UnityGraphics->GetRenderer();

	switch (m_DeviceType)
	{
#if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)
	case	kUnityGfxRendererD3D11:
	{
		m_ContextD3D11 = null;
		m_DeviceD3D11 = null;
	}
	break;
#endif // BUILD_WITH_D3D11_SUPPORT
#if	defined(PK_ORBIS)
	case	kUnityGfxRendererPS4:
	{
		m_PS4Interface = null;
		m_ContextPS4 = null;
	}
	break;
#endif //PK_ORBIS
#if	defined(PK_UNKNOWN2)
	case	kUnityGfxRendererUNKNOWN2:
	{
		m_UNKNOWN2Interface = null;
		m_ContextUNKNOWN2 = null;
	}
	break;
#endif //PK_UNKNOWN2
#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	case	kUnityGfxRendererVulkan:
		m_UnityVulkan = null;
		break;
#endif
	case	kUnityGfxRendererNull:
		return false;
	default:
		break;
	}
	return true;
}

//----------------------------------------------------------------------------

UnityGfxRenderer	CRuntimeManager::GetDeviceType() const
{
	return m_DeviceType;
}

#if	(PK_BUILD_WITH_D3D11_SUPPORT != 0)

//----------------------------------------------------------------------------

D3DDevice	*CRuntimeManager::GetDeviceD3D11() const
{
	return m_DeviceD3D11;
}


//----------------------------------------------------------------------------

D3DDeviceContext	*CRuntimeManager::GetContextD3D11() const
{
	return m_ContextD3D11;
}

#elif	defined(PK_ORBIS)

//----------------------------------------------------------------------------

GnmPS4ContextLW	*CRuntimeManager::GetContextPS4() const
{
	return m_ContextPS4;
}

IUnityGraphicsPS4	*CRuntimeManager::GetOrbisUnityGraphics()
{
	return m_PS4Interface;
}

#elif	defined(PK_UNKNOWN2)

//----------------------------------------------------------------------------

UNKNOWN2ContextLW		*CRuntimeManager::GetContextUNKNOWN2() const
{
	return m_ContextUNKNOWN2;
}

IUnityGraphicsUNKNOWN2	*CRuntimeManager::GetUNKNOWN2UnityGraphics()
{
	return m_UNKNOWN2Interface;
}
#endif

//----------------------------------------------------------------------------

#if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)
ID3D12Device	*CRuntimeManager::GetDeviceD3D12() const
{
	return m_DeviceD3D12;
}
#endif

//----------------------------------------------------------------------------

const TArray<float*>	*CRuntimeManager::GetSpectrum(CStringId name) const
{
	for (u32 i = 0; i < m_SpectrumDescriptorList.Count(); i++)
	{
		if (m_SpectrumDescriptorList[i].m_RegisteredName == name)
			return &m_SpectrumDescriptorList[i].m_SpectrumPyramid;
	}
	return null;
}

//----------------------------------------------------------------------------

IFileSystem	*CRuntimeManager::GetFileSystem() const
{
	return File::DefaultFileSystem();
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::LoadPack()
{
	File::DefaultFileSystem()->MountPack("./");
	return true;
}


//----------------------------------------------------------------------------

CGuid	CRuntimeManager::FxRegister(CPKFXEffect *fx)
{
	CGuid	slot;
	PK_ASSERT(fx != null);
	if (fx != null)
	{
		slot = m_Effects.Insert(fx);
		fx->SetGUID(slot);
	}
	return slot;
}

//----------------------------------------------------------------------------

CPKFXEffect		*CRuntimeManager::FxGet(CGuid guid)
{
	if (PK_VERIFY(guid < m_Effects.Count()))
	{
		return m_Effects[guid];
	}
	return null;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::ClearAllInstances(bool managedIsCleared)
{
	if (managedIsCleared)
		m_Unloading = true;
	for (u32 i = 0; i < m_Effects.Count(); ++i)
	{
		if (m_Effects[i] != null)
			OnFxStopped(i);
	}
	m_PreloadedFx.Clear();
	// If the managed side is cleared, we need to remove all the pending callbacks:
	if (managedIsCleared)
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		m_OnResizeRenderer.Clear();
		m_OnSetParticleCount.Clear();
		m_OnSetRendererActive.Clear();
		m_OnUpdateRendererBounds.Clear();
		m_OnFxStopped.Clear();
	}
	if (managedIsCleared)
		m_Unloading = false;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::ClearFxInstances(const char *fxPath)
{
	CString pathToClear = fxPath;

	for (u32 i = 0; i < m_Effects.Count(); ++i)
	{
		if (m_Effects[i] != null && m_Effects[i]->Path() == pathToClear)
		{
			m_Effects[i]->KillFX();
			OnFxStopped(i);
		}
	}
	m_PreloadedFx.Remove(CStringId(pathToClear));
	CParticleEffect::Unload(pathToClear);
}

//----------------------------------------------------------------------------

void	CRuntimeManager::SceneMeshClear()
{
	m_ParticleScene->UnsetCollisionMesh();
}

//----------------------------------------------------------------------------

void	CRuntimeManager::PreloadFxIFN(const char *fxPath, bool usesMeshRenderer)
{
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		_ExecPreloadFxIFN(fxPath, usesMeshRenderer);
	}
	else
	{
		SPreloadFx		callback;

		callback.m_Path = fxPath;
		callback.m_UsesMeshRenderer = usesMeshRenderer;
		m_ToPreload.PushBack(callback);
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::StartFx(int guid, float dt, float prewarm)
{
	PK_ASSERT(guid >= 0);
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		return _ExecStartFx(guid, dt, prewarm);
	}
	else
	{
		SStartFx	callback;

		callback.m_Guid = guid;
		callback.m_Delay = dt;
		callback.m_Prewarm = prewarm;
		m_ToStart.PushBack(callback);
		return true;
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::TerminateFx(int guid)
{
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		return _ExecTerminateFx(guid);
	}
	else
	{
		STerminateFx	callback;

		callback.m_Guid = guid;
		m_ToTerminate.PushBack(callback);
		return true;
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::StopFx(int guid)
{
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		return _ExecStopFx(guid);
	}
	else
	{
		SStopFx		callback;

		callback.m_Guid = guid;
		m_ToStop.PushBack(callback);
		return true;
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::KillFx(int guid)
{
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		return _ExecKillFx(guid);
	}
	else
	{
		SKillFx		callback;

		callback.m_Guid = guid;
		m_ToKill.PushBack(callback);
		return true;
	}
}

//----------------------------------------------------------------------------

void	CRuntimeManager::ExecDelayedManagedToNativeMethods()
{
	PK_SCOPEDPROFILE();

	PK_FOREACH(it, m_ToPreload)
	{
		_ExecPreloadFxIFN(it->m_Path.Data(), it->m_UsesMeshRenderer);
	}
	m_ToPreload.Clear();

	PK_FOREACH(it, m_ToStart)
	{
		_ExecStartFx(it->m_Guid, it->m_Delay, it->m_Prewarm);
	}
	m_ToStart.Clear();

	PK_FOREACH(it, m_ToTerminate)
	{
		_ExecTerminateFx(it->m_Guid);
	}
	m_ToTerminate.Clear();

	PK_FOREACH(it, m_ToStop)
	{
		_ExecStopFx(it->m_Guid);
	}
	m_ToStop.Clear();

	PK_FOREACH(it, m_ToKill)
	{
		_ExecKillFx(it->m_Guid);
	}
	m_ToKill.Clear();

#if 0	// NOTE(Julien): 'TransformAllParticles' is a no-op in v2.0
	PK_FOREACH(it, m_TransformAllParticles)
	{
		_ExecTransformAllParticles(it->m_Transform);
	}
	m_TransformAllParticles.Clear();
#endif

	PK_FOREACH(it, m_UpdateCamDesc)
	{
		_ExecUpdateCamDesc(it->m_CamID, it->m_Desc, it->m_Update);
	}
	m_UpdateCamDesc.Clear();
}

//----------------------------------------------------------------------------

bool		CRuntimeManager::OnResizeRenderer(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount, const SRetrieveRendererInfo *info, bool *delayedResult)
{
	if (CCurrentThread::IsMainThread())
	{
		// Can execute the C# call directly:
		ManagedBool		resized = ::OnResizeRenderer(rendererGUID, particleCount, reservedVertexCount, reservedIndexCount);
		if (resized)
			OnRetrieveRendererBufferInfo(rendererGUID, info);
		*delayedResult = true;
		return true;
	}
	else
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		// Can only execute those callbacks on the main thread, delay this call:
		if (!PK_VERIFY(m_OnResizeRenderer.PushBack().Valid()))
			return false;

		SOnResizeRenderer	&cb = m_OnResizeRenderer.Last();

		cb.m_RendererGUID = rendererGUID;
		cb.m_ParticleCount = particleCount;
		cb.m_ReservedVertexCount = reservedVertexCount;
		cb.m_ReservedIndexCount = reservedIndexCount;
		cb.m_BufferInfo = *info;
		cb.m_DelayedResult = delayedResult;
		return false;
	}
}

//----------------------------------------------------------------------------

bool			CRuntimeManager::OnSetParticleCount(int rendererGUID, int particleCount)
{
	if (CCurrentThread::IsMainThread())
	{
		// Can execute the C# call directly:
		::OnSetParticleCount(rendererGUID, particleCount);
		return true;
	}
	else
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		// Can only execute those callbacks on the main thread, delay this call:
		if (!m_OnSetParticleCount.PushBack().Valid())
			return false;

		SOnSetParticleCount	&cb = m_OnSetParticleCount.Last();

		cb.m_RendererGUID = rendererGUID;
		cb.m_ParticleCount = particleCount;
		return false;
	}
}

//----------------------------------------------------------------------------

bool			CRuntimeManager::OnSetRendererActive(int rendererGUID, ManagedBool active)
{
	if (CCurrentThread::IsMainThread())
	{
		// Can execute the C# call directly:
		::OnSetRendererActive(rendererGUID, active);
		return true;
	}
	else
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		// Can only execute those callbacks on the main thread, delay this call:
		if (!m_OnSetRendererActive.PushBack().Valid())
			return false;

		SOnSetRendererActive	&cb = m_OnSetRendererActive.Last();

		cb.m_RendererGUID = rendererGUID;
		cb.m_Active = active;
		return false;
	}
}

//----------------------------------------------------------------------------

bool			CRuntimeManager::OnUpdateRendererBounds(int rendererGUID, const SUpdateRendererBounds *bounds)
{
	if (CCurrentThread::IsMainThread())
	{
		// Can execute the C# call directly:
		::OnUpdateRendererBounds(rendererGUID, bounds);
		return true;
	}
	else
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		// Can only execute those callbacks on the main thread, delay this call:
		if (!m_OnUpdateRendererBounds.PushBack().Valid())
			return false;

		SOnUpdateRendererBounds	&cb = m_OnUpdateRendererBounds.Last();

		cb.m_RendererGUID = rendererGUID;
		cb.m_Bounds = *bounds;
		return false;
	}
}

//----------------------------------------------------------------------------

void	CRuntimeManager::OnFxStopped(int guid)
{
	if (CCurrentThread::IsMainThread())
	{
		{
			// Need to remove from this list to avoid double-free in the case of a terminate or kill
			PK_SCOPEDLOCK(m_Native2ManagedLock);
			SOnFxStopped	callback;
			callback.m_Guid = guid;
			m_OnFxStopped.RemoveElement(callback);
		}
		_ExecOnFxStopped(CGuid(checked_numcast<u32>(guid)));
	}
	else
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		SOnFxStopped	callback;

		callback.m_Guid = guid;
		m_OnFxStopped.PushBack(callback);
	}
}

//----------------------------------------------------------------------------

void			CRuntimeManager::ExecDelayedNativeToManagedMethods()
{
	{
		PK_SCOPEDLOCK(m_Native2ManagedLock);
		// Copy:
		m_GameThread_OnResizeRenderer = m_OnResizeRenderer;
		m_GameThread_OnSetParticleCount = m_OnSetParticleCount;
		m_GameThread_OnSetRendererActive = m_OnSetRendererActive;
		m_GameThread_OnUpdateRendererBounds = m_OnUpdateRendererBounds;
		m_GameThread_OnFxStopped = m_OnFxStopped;
		// Clear:
		m_OnResizeRenderer.Clear();
		m_OnSetParticleCount.Clear();
		m_OnSetRendererActive.Clear();
		m_OnUpdateRendererBounds.Clear();
		m_OnFxStopped.Clear();
	}

	PK_FOREACH(cb, m_GameThread_OnResizeRenderer)
	{
		ManagedBool		resized = ::OnResizeRenderer(cb->m_RendererGUID, cb->m_ParticleCount, cb->m_ReservedVertexCount, cb->m_ReservedIndexCount);
		if (resized)
			::OnRetrieveRendererBufferInfo(cb->m_RendererGUID, &cb->m_BufferInfo);
		*(cb->m_DelayedResult) = true;
	}
	m_GameThread_OnResizeRenderer.Clear();
	PK_FOREACH(cb, m_GameThread_OnSetParticleCount)
	{
		::OnSetParticleCount(cb->m_RendererGUID, cb->m_ParticleCount);
	}
	m_GameThread_OnSetParticleCount.Clear();
	PK_FOREACH(cb, m_GameThread_OnSetRendererActive)
	{
		::OnSetRendererActive(cb->m_RendererGUID, cb->m_Active);
	}
	m_GameThread_OnSetRendererActive.Clear();
	PK_FOREACH(cb, m_GameThread_OnUpdateRendererBounds)
	{
		::OnUpdateRendererBounds(cb->m_RendererGUID, &cb->m_Bounds);
	}
	m_GameThread_OnUpdateRendererBounds.Clear();
	PK_FOREACH(it, m_GameThread_OnFxStopped)
	{
		_ExecOnFxStopped(it->m_Guid);
	}
	m_GameThread_OnFxStopped.Clear();
}

//----------------------------------------------------------------------------

CUnityLog			*CRuntimeManager::GetLogger() const
{
#if	(PK_LOG_ENABLED != 0)
	return m_PopcornFXRuntimeData->m_UnityLogger;
#else
	return null;
#endif
}

//----------------------------------------------------------------------------

void			CRuntimeManager::SetMaxLogStack(u32 maxLogStack)
{
#if	(PK_LOG_ENABLED != 0)
	if (PK_VERIFY(m_PopcornFXRuntimeData->m_UnityLogger != null))
		m_PopcornFXRuntimeData->m_UnityLogger->SetMaxLogsCount(maxLogStack);
#endif
}

//----------------------------------------------------------------------------

PMeshNew		CRuntimeManager::LoadPkmm(const CString &pkmmVirtualPath)
{
	PK_ASSERT(CCurrentThread::IsMainThread());
	PMeshNew					outMeshNew;
	TResourcePtr<CResourceMesh>	resMesh = Resource::DefaultManager()->Load<CResourceMesh>(pkmmVirtualPath);

	if (resMesh == null || resMesh->Empty())
	{
		CLog::Log(PK_ERROR, "[MeshBaker::LoadPkmm] \"%s\": fail to load resource", pkmmVirtualPath.Data());
	}
	else
	{
		const u32	BatchCount = resMesh->BatchList().Count();

		PK_ASSERT(BatchCount > 0);
		if (BatchCount == 0)
		{
			CLog::Log(PK_WARN, "[MeshBaker::LoadPkmm] \"%s\" is an empty mesh", pkmmVirtualPath.Data());
			outMeshNew = PK_NEW(CMeshNew);
		}
		else
		{
			CResourceMeshBatch	*batch = resMesh->BatchList()[0].Get();

			PK_ASSERT(batch != null);
			if (batch == null)
			{
				CLog::Log(PK_WARN, "[MeshBaker::LoadPkmm] \"%s\" invalid mesh", pkmmVirtualPath.Data());
			}
			else
			{
				outMeshNew = batch->RawMesh();
				PK_ASSERT(outMeshNew != null);
			}
		}
	}
	if (outMeshNew != null)
		CLog::Log(PK_INFO, "[MeshBaker::LoadPkmm] \"%s\" OK (primitives count: %d)", pkmmVirtualPath.Data(), outMeshNew->TriangleBatch().PrimitiveCount());
	return outMeshNew;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::BeforeUpdate()
{
	TSlotArray<CPKFXEffect*>::Iterator it = m_Effects.Begin();
	for (; it != m_Effects.End(); ++it)
	{
		(*it)->WaitSwitchIFN();
	}
}

//----------------------------------------------------------------------------

void	CRuntimeManager::AfterUpdate()
{
	TSlotArray<CPKFXEffect*>::Iterator it = m_Effects.Begin();
	for (; it != m_Effects.End(); ++it)
	{
		(*it)->AsyncSwitchIFN();
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::KickBackgroundTasksIFN()
{
	if (m_HasBackgroundTasksToKick)
	{
		Threads::PAbstractPool	pool = Scheduler::ThreadPool();
		if (!PK_VERIFY(pool != null))
		{
			CLog::Log(PK_ERROR, "Could not add background task to thread pool");
			return false;
		}
		pool->KickTasks(true);
		m_HasBackgroundTasksToKick = false;
	}
	return true;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::UpdateCamDesc(int camID, const SCamDesc &desc, bool update)
{
	if (!m_ParticleScene->GetParticleMediumCollection(false)->UpdatePending())
	{
		_ExecUpdateCamDesc(camID, desc, update);
	}
	else
	{
		SUpdateCamDesc		callback;

		callback.m_CamID = camID;
		callback.m_Desc = desc;
		callback.m_Update = update;
		m_UpdateCamDesc.PushBack(callback);
	}
}

//----------------------------------------------------------------------------

IFileSystem	*CRuntimeManager::_CreateCustomFileSystem()
{
	return PK_NEW(CUnityPseudoFileSystem);
}

//----------------------------------------------------------------------------

Threads::PAbstractPool	CRuntimeManager::_CreateCustomThreadPool()
{
	volatile const SPopcornFxSettings	*settings = CRuntimeManager::SPopcornFXRuntimeData::m_Settings;
	bool								success = true;

	if (settings == null || settings->m_SingleThreadedExecution) // Single threaded thread pool?
	{
		return PK_NEW(CSingleThreadedPool()); // Return single threaded pool if the config is not set yet or if it is forced as single threaded
	}

	const u32					*affinityMasks = settings->m_WorkerAffinities;
	u32							affinityCount = settings->m_WorkerCount;
	bool						hasExplicitAffinities = false;
	CThreadManager::EPriority	workersPriority = CThreadManager::Priority_High;
	const u32					kWorkerAffinities[] =
#if		defined(PK_ORBIS)
		// Default worker affinities on Orbis (and UNKNOWN2?):
	{
		1 << 2,
		1 << 3, // our main thread
		1 << 4,
		1 << 5,
	};
#elif	defined(PK_LUMIN)
		// Default worker affinities on Lumin:
	{
		1 << 2,
		1 << 3,
		1 << 4,
	};
#else
	{ 0 };
#endif

	if (settings->m_OverrideThreadPool) // Do we need to override the thread pool with the user configuration?
	{
		if (affinityMasks != null)
		{
			for (u32 i = 0; i < affinityCount; i++)
				hasExplicitAffinities |= (affinityMasks[i] != 0);
		}
	}
	else // Otherwise we provide some default configurations for some platforms
	{
#if		defined(PK_ORBIS) || defined(PK_UNKNOWN2)
		affinityMasks = kWorkerAffinities;
		affinityCount = PK_ARRAY_COUNT(kWorkerAffinities);
		workersPriority = CThreadManager::Priority_BackgroundLow;
		hasExplicitAffinities = true;
#elif	defined(PK_LUMIN)
		affinityMasks = kWorkerAffinities;
		affinityCount = PK_ARRAY_COUNT(kWorkerAffinities);
		hasExplicitAffinities = true;
#endif
	}

	PWorkerThreadPool	pool = PK_NEW(CWorkerThreadPool);

	if (PK_VERIFY(pool != null))
	{
		if (hasExplicitAffinities)	// user specified explicit worker affinities: override our own scheduling entirely.
		{
			CLog::Log(PK_INFO, "Found explicit user affinity masks for worker-threads. Overriding default thread-pool creation.");
			const u32				cpuCoresMask = CPU::Caps().ProcessAffinity().AffinityBlock32(0);
			CGenericAffinityMask	affinityMask;
			if (settings->m_WorkerCount != 0)
			{
				for (int i = 0; i < settings->m_WorkerCount; i++)
				{
					if (settings->m_WorkerAffinities[i] != 0)
					{
						affinityMask.Clear();
						affinityMask.SetAffinityBlock32(0, settings->m_WorkerAffinities[i] & cpuCoresMask);

						success &= (null != pool->AddWorker(workersPriority, &affinityMask));
					}
				}
			}
			else
			{
				for (u32 i = 0; i < affinityCount; i++)
				{
					if (kWorkerAffinities[i] != 0)
					{
						affinityMask.Clear();
						affinityMask.SetAffinityBlock32(0, kWorkerAffinities[i] & cpuCoresMask);

						success &= (null != pool->AddWorker(workersPriority, &affinityMask));
					}
				}
			}

			if (!success)
				CLog::Log(PK_ERROR, "Could not create some of the explicitely affinitized worker-threads.");

			success = true;	// still keep what has been created. don't fail here.
		}
		else
		{
#if defined(PK_DESKTOP) || defined(PK_MOBILE)
			// Let the OS shedule our workers
			// leave 2 core for main thread and render thread
			u32		processorCount = PKMax(CPU::Caps().ProcessAffinity().NumBitsSet(), 3U) - 2U;

			success = pool->AddFullAffinityWorkers(processorCount, CPU::Caps().ProcessAffinity(), workersPriority);
#else
			success = pool->AddNUMAWorkers();
#endif
		}

		if (!success)
			return null;

		pool->StartWorkers();
	}
	return pool;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::_ExecPreloadFxIFN(const char *fxPath, bool usesMeshRenderer)
{
	const CString	pathStr = fxPath;
	const CStringId	pathId = CStringId(pathStr);

	if (!m_PreloadedFx.Contains(CStringId(pathId))) // Fx not loaded yet.
	{
		PBaseObjectFile		file = HBO::g_Context->FindFile(pathStr);
		PParticleEffect		effect = CParticleEffect::Load(pathStr);

		if (effect != null)
		{
			CParticleMediumCollection	*medCol = m_ParticleScene->GetParticleMediumCollection(usesMeshRenderer);

			if (effect->Install(medCol))
			{
				CLog::Log(PK_INFO, "Preload Effect OK: %s", pathStr.Data());
				m_PreloadedFx.Insert(pathId, pathStr);
			}
			else
				CLog::Log(PK_ERROR, "Preloading failed \"%s\"", pathStr.Data());
		}
		else
		{
			CLog::Log(PK_ERROR, "Preloading failed \"%s\"", pathStr.Data());
		}
	}
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::_ExecStartFx(int guid, float delay, float prewarm)
{
	CPKFXEffect		*fx = FxGet(guid);
	if (fx != null)
	{
		fx->RunFX(delay, prewarm);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::_ExecTerminateFx(int guid)
{
	CPKFXEffect		*fx = FxGet(guid);
	if (fx != null)
	{
		fx->TerminateFX();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::_ExecStopFx(int guid)
{
	CPKFXEffect		*fx = FxGet(guid);
	if (fx != null)
	{
		fx->StopFX();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::_ExecKillFx(int guid)
{
	CPKFXEffect		*fx = FxGet(guid);
	PK_ASSERT(fx != null);
	if (fx != null)
	{
		fx->KillFX();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

void	CRuntimeManager::_ExecUpdateCamDesc(int camID, SCamDesc desc, bool update)
{
	(void)update;
	CParticleMediumCollection		*medCol = m_ParticleScene->GetParticleMediumCollection(false);
	CParticleMediumCollection		*medColMeshes = m_ParticleScene->GetParticleMediumCollection(true);
	TArray<SUnitySceneView>				&sceneViews = m_ParticleScene->SceneViewsForUpdate();

	PK_ASSERT(s32(sceneViews.Count()) >= camID);
	if (sceneViews.Count() < (u32)(camID + 1))
		return;

	SUnitySceneView	&sceneView = sceneViews[camID];

	const CFloat4x4 &camV2W = desc.m_ViewMatrix.Inverse();
	const CFloat4x4	&camW2V = desc.m_ViewMatrix;
	const CFloat4x4 &camV2P = desc.m_ProjectionMatrix;
	const CFloat4x4 &camP2V = desc.m_ProjectionMatrix.Inverse();
	const CFloat4x4	camW2P = camW2V * camV2P;
	const CFloat4x4	camP2W = camP2V * camV2W;

	sceneView.m_InvViewMatrix = camV2W;


	// We update the "view" namespace in the script
	if (sceneView.m_UserData.m_CamSlotIdxInMedCol.Valid())
	{
		medCol->UpdateView(	sceneView.m_UserData.m_CamSlotIdxInMedCol,
							camW2V,
							camV2W,
							camW2P,
							camP2W,
							CUint2(desc.m_ViewportWidth, desc.m_ViewportHeight));
	}
	if (sceneView.m_UserData.m_CamSlotIdxInMeshMedCol.Valid())
	{
		medColMeshes->UpdateView(	sceneView.m_UserData.m_CamSlotIdxInMeshMedCol,
									camW2V,
									camV2W,
									camW2P,
									camP2W,
									CUint2(desc.m_ViewportWidth, desc.m_ViewportHeight));
	}
}

//----------------------------------------------------------------------------

void		CRuntimeManager::_ExecOnFxStopped(CGuid fxId)
{
	if (PK_VERIFY(fxId < m_Effects.Count()))
	{
		if (PK_VERIFY(m_Effects[fxId] != null))
		{
			if (!m_Unloading)
				::OnFxStopped(fxId);
			PK_DELETE(m_Effects[fxId]);
			m_Effects.Remove(fxId);
		}
	}
}

//----------------------------------------------------------------------------

CRuntimeManager::CBackgroundTask::CBackgroundTask()
{
	m_ExecutionFilter = ExecFilter_BackgroundTask;
}

//----------------------------------------------------------------------------

CRuntimeManager::CBackgroundTask::~CBackgroundTask()
{
}

//----------------------------------------------------------------------------

void	CRuntimeManager::CBackgroundTask::_VirtualLaunch(Threads::SThreadContext &threadContext)
{
	(void)threadContext;
	m_Cb();
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::SPopcornFXRuntimeData::PopcornFXStartup()
{
#ifdef	PK_DEBUG
	const bool	debugMode = true;
#else
	const bool	debugMode = false;
#endif

#if	(PK_LOG_ENABLED != 0)
	m_UnityLogger = null;
	m_FileLogger = null;
#endif

	if (m_Settings != null)
	{
		m_GPUBillboarding = m_Settings->m_EnableGPUBillboarding == ManagedBool_True ? true : false;
	}
	SDllVersion			engineVersion(PK_VERSION_MAJOR, PK_VERSION_MINOR, PK_VERSION_PATCH, PK_VERSION_REVID, debugMode);
	CPKKernel::Config	configKernel;

	// The log is handled by the Unity log listener: it stacks the logs until they are unstacked
	configKernel.m_AddDefaultLogListeners = &AddDefaultLogListenersOverride;
	configKernel.m_UserHandle = this;
#if		defined(PK_MOBILE)
	configKernel.m_AssertCatcher = &CbAssertCatcherSkip;
#elif	defined(PK_MACOSX)
	configKernel.m_AssertCatcher = &PKFX::AssertCatcher_LogThenNativeOsxAlert;
#elif	defined(PK_LINUX)
	g_AssertScriptFilePath = "./assertmessage.sh";
	if (g_AssertScriptFilePath != 0)
	{
		if (access(g_AssertScriptFilePath, X_OK) == 0)
		{
			int		res = system(g_AssertScriptFilePath);
			if (res == 0)
			{
				printf("PKFX: Assert script catcher \"%s\" enabled\n", g_AssertScriptFilePath);
				configKernel.m_AssertCatcher = &CbAssertCatcher;
			}
			else
				fprintf(stderr, "PKFX: Assert script catcher \"%s\" not valid\n", g_AssertScriptFilePath);
		}
		else
			fprintf(stderr, "PKFX: Assert script catcher \"%s\" not found or permission denied\n", g_AssertScriptFilePath);
	}
#endif

	configKernel.m_CreateThreadPool = &_CreateCustomThreadPool;
	configKernel.m_NewFileSystem = &_CreateCustomFileSystem;

	if (CPKKernel::Startup(engineVersion, configKernel) &&
		CPKBaseObject::Startup(engineVersion, CPKBaseObject::Config()) &&
		CPKEngineUtils::Startup(engineVersion, CPKEngineUtils::Config()) &&
		CPKCompiler::Startup(engineVersion, CPKCompiler::Config()) &&
		CPKGeometrics::Startup(engineVersion, CPKGeometrics::Config()) &&
		CPKImaging::Startup(engineVersion, CPKImaging::Config()) &&
		CPKParticles::Startup(engineVersion, CPKParticles::Config()) &&
		ParticleToolbox::Startup() &&
		CPKRenderHelpers::Startup(engineVersion, CPKRenderHelpers::Config()) &&
		PK_VERIFY(Kernel::CheckStaticConfigFlags(Kernel::g_BaseStaticConfig, SKernelConfigFlags())))
	{
#if		PK_UNITY_EDITOR
		PKFX::RegisterPlugins(PKFX::EPlugin_Editor);
#else
		PKFX::RegisterPlugins(PKFX::EPlugin_Runtime);
#endif

		m_ImageHandler = PK_NEW(CUnityResourceHandlerImage);
		PK_ASSERT(m_ImageHandler != null);
		Resource::DefaultManager()->RegisterHandler<CImage>(m_ImageHandler);

		CCoordinateFrame::SetGlobalFrame(Frame_LeftHand_Y_Up);

		SkeletalAnimationTexture::Startup();
		BasicRendererProperties::Startup();
		VertexAnimationRendererProperties::Startup();

		PK_LOG_MODULE_INIT_START;
		PK_LOG_MODULE_INIT_END;
		CLog::Log(PK_INFO, "PopcornFX Runtime initialisation OK");
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CRuntimeManager::SPopcornFXRuntimeData::PopcornFXShutdown()
{
	PKFX::UnregisterPlugins();

	PK_SAFE_DELETE(m_ImageHandler);

	BasicRendererProperties::Shutdown();
	SkeletalAnimationTexture::Shutdown();

	PK_LOG_MODULE_RELEASE_START;
	PK_LOG_MODULE_RELEASE_END;

	CPKRenderHelpers::Shutdown();
	ParticleToolbox::Shutdown();
	CPKParticles::Shutdown();
	CPKGeometrics::Shutdown();
	CPKImaging::Shutdown();
	CPKCompiler::Shutdown();
	CPKEngineUtils::Shutdown();
	CPKBaseObject::Shutdown();
	CPKKernel::Shutdown();
	return true;
}

//----------------------------------------------------------------------------
__PK_API_END
