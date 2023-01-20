#pragma once

#include <AudioClient.h>
#include <mmdeviceapi.h>
#include <initguid.h>
#include <guiddef.h>
#include <mfapi.h>
#include <wrl/implements.h>

#include <pk_kernel/include/kr_threads_basics.h>

#include "wil/com.h"

#include <complex>

#include "../ILoopbackCapture.h"
#include "Common.h"

using namespace Microsoft::WRL;

class CLoopbackCapture :
	public RuntimeClass< RuntimeClassFlags< ClassicCom >, FtmBase, IActivateAudioInterfaceCompletionHandler >
{
public:
	CLoopbackCapture() = default;
	~CLoopbackCapture();

	HRESULT StartCaptureAsync(DWORD processId, bool includeProcessTree);
	HRESULT StopCaptureAsync();

	bool		UpdateAudioBuffer(u32 sampleCount);
	float		*GetAudioWaveform();
	float		*GetAudioSpectrum();
	void		SetVolumeMultiplier(float volume);

	METHODASYNCCALLBACK(CLoopbackCapture, StartCapture, OnStartCapture);
	METHODASYNCCALLBACK(CLoopbackCapture, StopCapture, OnStopCapture);
	METHODASYNCCALLBACK(CLoopbackCapture, SampleReady, OnSampleReady);
	METHODASYNCCALLBACK(CLoopbackCapture, FinishCapture, OnFinishCapture);

	// IActivateAudioInterfaceCompletionHandler
	STDMETHOD(ActivateCompleted)(IActivateAudioInterfaceAsyncOperation* operation);

private:
	// NB: All states >= Initialized will allow some methods
	// to be called successfully on the Audio Client
	enum class DeviceState
	{
		Uninitialized,
		Error,
		Initialized,
		Starting,
		Capturing,
		Stopping,
		Stopped,
	};

	HRESULT	OnStartCapture(IMFAsyncResult* pResult);
	HRESULT	OnStopCapture(IMFAsyncResult* pResult);
	HRESULT	OnFinishCapture(IMFAsyncResult* pResult);
	HRESULT	OnSampleReady(IMFAsyncResult* pResult);

	HRESULT	InitializeLoopbackCapture();
	HRESULT	OnAudioSampleRequested();

	HRESULT	ActivateAudioInterface(DWORD processId, bool includeProcessTree);
	HRESULT	FinishCaptureAsync();

	HRESULT	SetDeviceStateErrorIfFailed(HRESULT hr);

	void		_BasicFFT(std::complex<float> *inputs, std::complex<float> *outputs, u32 log2n);

	wil::com_ptr_nothrow<IAudioClient>			m_AudioClient;
	WAVEFORMATEX								m_CaptureFormat = {};
	u32											m_BufferFrames = 0;
	wil::com_ptr_nothrow<IAudioCaptureClient>	m_AudioCaptureClient;
	wil::com_ptr_nothrow<IMFAsyncResult>		m_SampleReadyAsyncResult;

	wil::unique_event_nothrow					m_SampleReadyEvent;
	MFWORKITEM_KEY								m_SampleReadyKey = 0;
	DWORD										m_dwQueueID = 0;
	Threads::CCriticalSection					m_CritSec;

	HRESULT										m_ActivateResult = E_UNEXPECTED;

	DeviceState									m_DeviceState = DeviceState::Uninitialized;
	Threads::CEvent								m_hActivateCompleted;
	Threads::CEvent								m_hCaptureStopped;

	ureg										m_AudioBufferValidSize;
	TArray<float>								m_AudioBuffer;
	TArray<float>								m_AudioBufferCopy;
	TArray<float>								m_SpectrumBuffer;
	TArray<std::complex<float> >				m_FFTInputData;
	TArray<std::complex<float> >				m_FFTOutputData;
	float										m_VolumeMultiplier = 1.0f;
};

class	CWindowLoopbackCapture : public ILoopbackCapture
{
public:
	CWindowLoopbackCapture() { }
	virtual ~CWindowLoopbackCapture() { }

	virtual bool	StartCapturing();
	virtual bool	StopCapturing();

	virtual bool	UpdateAudioBuffer(u32 sampleCount);
	virtual float	*GetAudioWaveform();
	virtual float	*GetAudioSpectrum();
	virtual void	SetVolumeMultiplier(float volume);

private:
	CLoopbackCapture	m_LoopbackAudio;
};
