//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "NativeToManaged.h"

#include <pk_kernel/include/kr_scheduler.h>

extern "C"
{
#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility push(default)
#endif

	// Fake file system manager:
	unsigned long long	(POPCORN_TO_MANAGED_CONVENTION	*_OnResourceLoad)(const char *path, void **handler) = null;
	unsigned long long	(POPCORN_TO_MANAGED_CONVENTION	*_OnResourceWrite)(const char *path, const void *data, u64 offset, u64 size) = null;

	// Particles to Unity interactions:
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRaycastStart)(int count, void **cmd) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRaycastEnd)() = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRaycastPack)(void **res) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnFxStopped)(int guid) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRaiseEvent)(int guid, int key, const char *eventName, unsigned int payloadCount, void* payloadDescs, void* payloadValues) = null;
	void				*(POPCORN_TO_MANAGED_CONVENTION	*_OnGetAudioWaveformData)(const char *name, int *nbSamples) = null;
	void				*(POPCORN_TO_MANAGED_CONVENTION	*_OnGetAudioSpectrumData)(const char *name, int *nbSamples) = null;

	// Rendering interactions:
	// Create renderers:
	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnSetupNewBillboardRenderer)(const SPopcornRendererDesc *rendererDesc, int idx) = null;
	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnSetupNewRibbonRenderer)(const SPopcornRendererDesc *rendererDesc, int idx) = null;
	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnSetupNewMeshRenderer)(const SMeshRendererDesc *rendererDesc, int idx) = null;
	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnSetupNewTriangleRenderer)(const SPopcornRendererDesc *rendererDesc, int idx) = null;

	ManagedBool			(POPCORN_TO_MANAGED_CONVENTION	*_OnResizeRenderer)(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnSetParticleCount)(int rendererGUID, int particleCount) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnSetRendererActive)(int rendererGUID, ManagedBool active) = null;

	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnSetMeshInstancesCount)(int rendererGUID, int submesh, int instancesCount) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnSetMeshInstancesBuffer)(int rendererGUID, int submesh, void *instanceBuffer) = null;

	void				(POPCORN_TO_MANAGED_CONVENTION *_OnSetLightsBuffer)(void *lightInfos, int count) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnSetSoundsBuffer)(void *soundInfos, int count) = null;

	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRetrieveCustomMaterialInfo)(int type, const void *rendererDesc, int idx, ManagedBool *hasCustomMaterial, int* customMaterialID) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnRetrieveRendererBufferInfo)(int rendererGUID, const SRetrieveRendererInfo *info) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnUpdateRendererBounds)(int rendererGUID, const SUpdateRendererBounds *bounds) = null;

	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnGetMeshCount)(int rendererGUID, int lod) = null;
	int					(POPCORN_TO_MANAGED_CONVENTION	*_OnGetMeshLODsCount)(int rendererGUID) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION	*_OnGetMeshBounds)(int rendererGUID, int submesh, void* bbox) = null;

	//----------------------------------------------------------------------------

	// Fake file system manager:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResourceLoad(void *delegatePtr)
	{
		_OnResourceLoad = (unsigned long long (POPCORN_TO_MANAGED_CONVENTION *)(const char *path, void **handler))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResourceWrite(void *delegatePtr)
	{
		_OnResourceWrite = (unsigned long long (POPCORN_TO_MANAGED_CONVENTION *)(const char *path, const void *data, u64 offset, u64 size))delegatePtr;
	}

	// Particles to Unity interactions:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastStart(void *delegatePtr)
	{
		_OnRaycastStart = (void (POPCORN_TO_MANAGED_CONVENTION *)(int count, void **cmd))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastEnd(void *delegatePtr)
	{
		_OnRaycastEnd = (void (POPCORN_TO_MANAGED_CONVENTION *)())delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaycastPack(void *delegatePtr)
	{
		_OnRaycastPack = (void (POPCORN_TO_MANAGED_CONVENTION *)(void** res))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnFxStopped(void *delegatePtr)
	{
		_OnFxStopped = (void (POPCORN_TO_MANAGED_CONVENTION *)(int guid))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRaiseEvent(void *delegatePtr)
	{
		_OnRaiseEvent = (void (POPCORN_TO_MANAGED_CONVENTION *)(int guid, int key, const char *eventName, unsigned int payloadCount, void* payloadDescs, void* payloadsValues))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAudioWaveformData(void *delegatePtr)
	{
		_OnGetAudioWaveformData = (void *(POPCORN_TO_MANAGED_CONVENTION *)(const char *name, int *nbSamples))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAudioSpectrumData(void *delegatePtr)
	{
		_OnGetAudioSpectrumData = (void *(POPCORN_TO_MANAGED_CONVENTION *)(const char *name, int *nbSamples))delegatePtr;
	}

	// Rendering interactions:
	// Create renderers:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewBillboardRenderer(void *delegatePtr)
	{
		_OnSetupNewBillboardRenderer = (int (POPCORN_TO_MANAGED_CONVENTION *)(const SPopcornRendererDesc *rendererDesc, int idx))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewRibbonRenderer(void *delegatePtr)
	{
		_OnSetupNewRibbonRenderer = (int (POPCORN_TO_MANAGED_CONVENTION *)(const SPopcornRendererDesc *rendererDesc, int idx))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewMeshRenderer(void *delegatePtr)
	{
		_OnSetupNewMeshRenderer = (int (POPCORN_TO_MANAGED_CONVENTION *)(const SMeshRendererDesc *rendererDesc, int idx))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetupNewTriangleRenderer(void *delegatePtr)
	{
		_OnSetupNewTriangleRenderer = (int (POPCORN_TO_MANAGED_CONVENTION *)(const SPopcornRendererDesc *rendererDesc, int idx))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnResizeRenderer(void *delegatePtr)
	{
		_OnResizeRenderer = (ManagedBool (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetParticleCount(void *delegatePtr)
	{
		_OnSetParticleCount = (void(POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int particleCount))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetRendererActive(void *delegatePtr)
	{
		_OnSetRendererActive = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, ManagedBool active))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetMeshInstancesCount(void *delegatePtr)
	{
		_OnSetMeshInstancesCount = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int submesh, int instanceCount))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetMeshInstancesBuffer(void *delegatePtr)
	{
		_OnSetMeshInstancesBuffer = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int submesh, void *instanceBuffer))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetLightsBuffer(void *delegatePtr)
	{
		_OnSetLightsBuffer = (void (POPCORN_TO_MANAGED_CONVENTION *)(void *lightInfos, int count))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnSetSoundsBuffer(void *delegatePtr)
	{
		_OnSetSoundsBuffer = (void (POPCORN_TO_MANAGED_CONVENTION*)(void *soundInfos, int count))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRetrieveCustomMaterialInfo(void *delegatePtr)
	{
		_OnRetrieveCustomMaterialInfo = (void (POPCORN_TO_MANAGED_CONVENTION *)(int type, const void *rendererDesc, int idx, ManagedBool *hasCustomMaterial, int* customMaterialID))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnRetrieveRendererBufferInfo(void *delegatePtr)
	{
		_OnRetrieveRendererBufferInfo = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, const SRetrieveRendererInfo *info))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnUpdateRendererBounds(void *delegatePtr)
	{
		_OnUpdateRendererBounds = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, const SUpdateRendererBounds *bounds))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshCount(void *delegatePtr)
	{
		_OnGetMeshCount = (int (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int lod))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshLODsCount(void* delegatePtr)
	{
		_OnGetMeshLODsCount = (int (POPCORN_TO_MANAGED_CONVENTION*)(int rendererGUID))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetMeshBounds(void *delegatePtr)
	{
		_OnGetMeshBounds = (void (POPCORN_TO_MANAGED_CONVENTION *)(int rendererGUID, int submesh, void* bbox))delegatePtr;
	}

	//----------------------------------------------------------------------------

	unsigned long long	OnResourceLoad(const char *path, void **handler)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnResourceLoad not called on main thread: callback ignored");
			return 0ULL;
		}
		if (PK_VERIFY(_OnResourceLoad != null))
		{
			return _OnResourceLoad(path, handler);
		}
		return 0ULL;
	}

	unsigned long long	OnResourceWrite(const char *path, const void *data, u64 offset, u64 size)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnResourceWrite not called on main thread: callback ignored");
			return 0ULL;
		}
		if (PK_VERIFY(_OnResourceWrite != null))
		{
			return _OnResourceWrite(path, data, offset, size);
		}
		return 0ULL;
	}

	void	OnRaycastStart(int count, void **cmd)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRaycastPack not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRaycastStart != null))
		{
			_OnRaycastStart(count, cmd);
		}
	}

	void	OnRaycastEnd()
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRaycastPack not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRaycastEnd != null))
		{
			_OnRaycastEnd();
		}
	}

	void	OnRaycastPack(void **res)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRaycastPack not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRaycastPack != null))
		{
			_OnRaycastPack(res);
		}
	}

	void	OnFxStopped(int guid)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnFxStopped not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnFxStopped != null))
		{
			_OnFxStopped(guid);
		}
	}

	void	OnRaiseEvent(unsigned int guid, unsigned int key, const char* eventName, unsigned int payloadCount, void* payloadDesc, void* payloadValues)
	{
		PK_SCOPEDPROFILE();
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRaiseEvent not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRaiseEvent != null))
		{
			_OnRaiseEvent(guid, key, eventName, payloadCount, payloadDesc, payloadValues);
		}
	}

	void	*OnGetAudioWaveformData(const char *name, int *nbSamples)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnGetAudioWaveformData not called on main thread: callback ignored");
			return null;
		}
		if (PK_VERIFY(_OnGetAudioWaveformData != null))
		{
			return _OnGetAudioWaveformData(name, nbSamples);
		}
		return null;
	}

	void	*OnGetAudioSpectrumData(const char *name, int *nbSamples)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnGetAudioSpectrumData not called on main thread: callback ignored");
			return null;
		}
		if (PK_VERIFY(_OnGetAudioSpectrumData != null))
		{
			return _OnGetAudioSpectrumData(name, nbSamples);
		}
		return null;
	}

	int	OnSetupNewBillboardRenderer(const SPopcornRendererDesc *rendererDesc, int idx)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetupNewBillboardRenderer not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnSetupNewBillboardRenderer != null))
		{
			return _OnSetupNewBillboardRenderer(rendererDesc, idx);
		}
		return -1;
	}

	int	OnSetupNewRibbonRenderer(const SPopcornRendererDesc *rendererDesc, int idx)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetupNewRibbonRenderer not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnSetupNewRibbonRenderer != null))
		{
			return _OnSetupNewRibbonRenderer(rendererDesc, idx);
		}
		return -1;
	}

	int	OnSetupNewMeshRenderer(const SMeshRendererDesc *rendererDesc, int idx)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetupNewMeshRenderer not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnSetupNewMeshRenderer != null))
		{
			return _OnSetupNewMeshRenderer(rendererDesc, idx);
		}
		return -1;
	}

	int	OnSetupNewTriangleRenderer(const SPopcornRendererDesc *rendererDesc, int idx)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetupNewTriangleRenderer not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnSetupNewTriangleRenderer != null))
		{
			return _OnSetupNewTriangleRenderer(rendererDesc, idx);
		}
		return -1;
	}

	ManagedBool	OnResizeRenderer(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnResizeRenderer not called on main thread: callback ignored");
			return ManagedBool_False;
		}
		if (PK_VERIFY(_OnResizeRenderer != null))
		{
			return _OnResizeRenderer(rendererGUID, particleCount, reservedVertexCount, reservedIndexCount);
		}
		return ManagedBool_False;
	}

	void	OnSetParticleCount(int rendererGUID, int particleCount)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetParticleCount not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetParticleCount != null))
		{
			_OnSetParticleCount(rendererGUID, particleCount);
		}
	}

	void	OnSetRendererActive(int rendererGUID, ManagedBool active)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetRendererActive not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetRendererActive != null))
		{
			_OnSetRendererActive(rendererGUID, active);
		}
	}

	void	OnSetMeshInstancesCount(int rendererGUID, int submesh, int instancesCount)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetMeshInstancesCount not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetMeshInstancesCount != null))
		{
			_OnSetMeshInstancesCount(rendererGUID, submesh, instancesCount);
		}
	}

	void	OnSetMeshInstancesBuffer(int rendererGUID, int submesh, void *instanceBuffer)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetMeshInstancesBuffer not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetMeshInstancesBuffer != null))
		{
			_OnSetMeshInstancesBuffer(rendererGUID, submesh, instanceBuffer);
		}
	}

	void	OnSetLightsBuffer(void *lightInfos, int count)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetLightsBuffer not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetLightsBuffer != null))
		{
			_OnSetLightsBuffer(lightInfos, count);
		}
	}

	void	OnSetSoundsBuffer(void *soundInfos, int count)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnSetSoundsBuffer not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnSetSoundsBuffer != null))
		{
			_OnSetSoundsBuffer(soundInfos, count);
		}
	}


	void	OnRetrieveCustomMaterialInfo(int type, const void *rendererDesc, int idx, ManagedBool *hasCustomMaterial, int *customMaterialID)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRetrieveCustomMaterialInfo not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRetrieveCustomMaterialInfo != null))
		{
			_OnRetrieveCustomMaterialInfo(type, rendererDesc, idx, hasCustomMaterial, customMaterialID);
		}
	}

	void	OnRetrieveRendererBufferInfo(int rendererGUID, const SRetrieveRendererInfo *info)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnRetrieveRendererBufferInfo not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnRetrieveRendererBufferInfo != null))
		{
			_OnRetrieveRendererBufferInfo(rendererGUID, info);
		}
	}

	void	OnUpdateRendererBounds(int rendererGUID, const SUpdateRendererBounds *bounds)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnUpdateRendererBounds not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnUpdateRendererBounds != null))
		{
			_OnUpdateRendererBounds(rendererGUID, bounds);
		}
	}

	int		OnGetMeshCount(int rendererGUID, int lod)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnGetMeshCount not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnGetMeshCount != null))
		{
			return _OnGetMeshCount(rendererGUID, lod);
		}
		return -1;
	}

	int	OnGetMeshLODsCount(int rendererGUID)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnGetMeshLODsCount not called on main thread: callback ignored");
			return -1;
		}
		if (PK_VERIFY(_OnGetMeshLODsCount != null))
		{
			return _OnGetMeshLODsCount(rendererGUID);
		}
		return -1;
	}

	void	OnGetMeshBounds(int rendererGUID, int submesh, void* bbox)
	{
		if (!PK_VERIFY(CCurrentThread::IsMainThread()))
		{
			CLog::Log(PK_ERROR, "OnGetMeshBounds not called on main thread: callback ignored");
			return;
		}
		if (PK_VERIFY(_OnGetMeshBounds != null))
		{
			_OnGetMeshBounds(rendererGUID, submesh, bbox);
		}
	}

	void	ClearNativeToManagedCallbacks()
	{
		_OnResourceLoad = null;
		_OnResourceWrite = null;
		_OnRaycastPack = null;
		_OnFxStopped = null;
		_OnRaiseEvent = null;
		_OnGetAudioWaveformData = null;
		_OnGetAudioSpectrumData = null;
		_OnSetupNewBillboardRenderer = null;
		_OnSetupNewRibbonRenderer = null;
		_OnSetupNewMeshRenderer = null;
		_OnSetupNewTriangleRenderer = null;
		_OnResizeRenderer = null;
		_OnSetParticleCount = null;
		_OnSetRendererActive = null;
		_OnSetMeshInstancesCount = null;
		_OnSetMeshInstancesBuffer = null;
		_OnSetLightsBuffer = null;
		_OnSetSoundsBuffer = null;
		_OnRetrieveCustomMaterialInfo = null;
		_OnRetrieveRendererBufferInfo = null;
		_OnUpdateRendererBounds = null;
	}

#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility pop
#endif
}
