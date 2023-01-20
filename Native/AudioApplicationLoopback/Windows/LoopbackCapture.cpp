
#include "precompiled.h"

#include <shlobj.h>
#include <wchar.h>
#include <iostream>
#include <audioclientactivationparams.h>

#include "LoopbackCapture.h"

HRESULT CLoopbackCapture::SetDeviceStateErrorIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		m_DeviceState = DeviceState::Error;
	}
	return hr;
}

HRESULT CLoopbackCapture::InitializeLoopbackCapture()
{
	if (m_DeviceState != DeviceState::Uninitialized)
		return NOERROR;
	{
		PK_SCOPEDLOCK(m_CritSec);
		if (!PK_VERIFY(m_AudioBuffer.Resize(0x2000)))
			return E_OUTOFMEMORY;
		m_AudioBufferValidSize = 0;
	}
	// Create events for sample ready or user stop
	RETURN_IF_FAILED(m_SampleReadyEvent.create(wil::EventOptions::None));

	// Initialize MF
	RETURN_IF_FAILED(MFStartup(MF_VERSION, MFSTARTUP_LITE));

	// Register MMCSS work queue
	DWORD dwTaskID = 0;
	RETURN_IF_FAILED(MFLockSharedWorkQueue(L"Capture", 0, &dwTaskID, &m_dwQueueID));

	// Set the capture event work queue to use the MMCSS queue
	m_xSampleReady.SetQueueID(m_dwQueueID);
	return NOERROR;
}

CLoopbackCapture::~CLoopbackCapture()
{
	if (m_dwQueueID != 0)
	{
		MFUnlockWorkQueue(m_dwQueueID);
	}
}

HRESULT	CLoopbackCapture::ActivateAudioInterface(DWORD processId, bool includeProcessTree)
{
	if (m_DeviceState != DeviceState::Uninitialized)
		return NOERROR;
	AUDIOCLIENT_ACTIVATION_PARAMS audioclientActivationParams = {};
	audioclientActivationParams.ActivationType = AUDIOCLIENT_ACTIVATION_TYPE_PROCESS_LOOPBACK;
	audioclientActivationParams.ProcessLoopbackParams.ProcessLoopbackMode = includeProcessTree ?
																			PROCESS_LOOPBACK_MODE_INCLUDE_TARGET_PROCESS_TREE :
																			PROCESS_LOOPBACK_MODE_EXCLUDE_TARGET_PROCESS_TREE;
	audioclientActivationParams.ProcessLoopbackParams.TargetProcessId = processId;

	PROPVARIANT activateParams = {};
	activateParams.vt = VT_BLOB;
	activateParams.blob.cbSize = sizeof(audioclientActivationParams);
	activateParams.blob.pBlobData = (BYTE*)&audioclientActivationParams;

	wil::com_ptr_nothrow<IActivateAudioInterfaceAsyncOperation>	 asyncOp;
	RETURN_IF_FAILED(ActivateAudioInterfaceAsync(VIRTUAL_AUDIO_DEVICE_PROCESS_LOOPBACK, __uuidof(IAudioClient), &activateParams, this, &asyncOp));

	// Wait for activation completion
	m_hActivateCompleted.Wait();
	return m_ActivateResult;
}

//
//  ActivateCompleted()
//
//  Callback implementation of ActivateAudioInterfaceAsync function.  This will be called on MTA thread
//  when results of the activation are available.
//
HRESULT CLoopbackCapture::ActivateCompleted(IActivateAudioInterfaceAsyncOperation *operation)
{
	m_ActivateResult = SetDeviceStateErrorIfFailed([&]()->HRESULT
		{
			// Check for a successful activation result
			HRESULT hrActivateResult = E_UNEXPECTED;
			wil::com_ptr_nothrow<IUnknown> punkAudioInterface;
			RETURN_IF_FAILED(operation->GetActivateResult(&hrActivateResult, &punkAudioInterface));
			RETURN_IF_FAILED(hrActivateResult);

			// Get the pointer for the Audio Client
			RETURN_IF_FAILED(punkAudioInterface.copy_to(&m_AudioClient));

			// The app can also call m_AudioClient->GetMixFormat instead to get the capture format.
			// 32 - bit float format.
			m_CaptureFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
			m_CaptureFormat.nChannels = 1;
			m_CaptureFormat.nSamplesPerSec = 44100;
			m_CaptureFormat.wBitsPerSample = 32;
			m_CaptureFormat.nBlockAlign = m_CaptureFormat.nChannels * m_CaptureFormat.wBitsPerSample / 8;
			m_CaptureFormat.nAvgBytesPerSec = m_CaptureFormat.nSamplesPerSec * m_CaptureFormat.nBlockAlign;

			// Initialize the AudioClient in Shared Mode with the user specified buffer
			RETURN_IF_FAILED(m_AudioClient->Initialize(	AUDCLNT_SHAREMODE_SHARED,
														AUDCLNT_STREAMFLAGS_LOOPBACK | AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
														200000,
														0,
														&m_CaptureFormat,
														nullptr));

			// Get the maximum size of the AudioClient Buffer
			RETURN_IF_FAILED(m_AudioClient->GetBufferSize(&m_BufferFrames));

			// Get the capture client
			RETURN_IF_FAILED(m_AudioClient->GetService(IID_PPV_ARGS(&m_AudioCaptureClient)));

			// Create Async callback for sample events
			RETURN_IF_FAILED(MFCreateAsyncResult(nullptr, &m_xSampleReady, nullptr, &m_SampleReadyAsyncResult));

			// Tell the system which event handle it should signal when an audio buffer is ready to be processed by the client
			RETURN_IF_FAILED(m_AudioClient->SetEventHandle(m_SampleReadyEvent.get()));

			// Everything is ready.
			m_DeviceState = DeviceState::Initialized;

			return S_OK;
		}());

	// Let ActivateAudioInterface know that m_ActivateResult has the result of the activation attempt.
	m_hActivateCompleted.Trigger();
	return S_OK;
}

HRESULT CLoopbackCapture::StartCaptureAsync(DWORD processId, bool includeProcessTree)
{
	RETURN_IF_FAILED(InitializeLoopbackCapture());
	RETURN_IF_FAILED(ActivateAudioInterface(processId, includeProcessTree));

	// We should be in the initialzied state if this is the first time through getting ready to capture.
	if (m_DeviceState == DeviceState::Initialized ||
		m_DeviceState == DeviceState::Stopped)
	{
		m_DeviceState = DeviceState::Starting;

#if		0
		wil::com_ptr_nothrow<IAudioStreamVolume>	audioVolume;
		wil::com_ptr_nothrow<ISimpleAudioVolume>	audioVolume2;
		wil::com_ptr_nothrow<IChannelAudioVolume>	audioVolume3;

		HRESULT hr0 = m_AudioClient->GetService(IID_PPV_ARGS(&audioVolume));
		HRESULT hr1 = m_AudioClient->GetService(IID_PPV_ARGS(&audioVolume2));
		HRESULT hr2 = m_AudioClient->GetService(IID_PPV_ARGS(&audioVolume3));
		if (audioVolume != null)
		{
			float		volume;
			audioVolume->GetAllVolumes(1, &volume);
		}
#endif
		return MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, &m_xStartCapture, nullptr);
	}

	return S_OK;
}

//
//  OnStartCapture()
//
//  Callback method to start capture
//
HRESULT CLoopbackCapture::OnStartCapture(IMFAsyncResult *pResult)
{
	(void)pResult;
	// Start the capture
	RETURN_IF_FAILED(m_AudioClient->Start());

	m_DeviceState = DeviceState::Capturing;
	MFPutWaitingWorkItem(m_SampleReadyEvent.get(), 0, m_SampleReadyAsyncResult.get(), &m_SampleReadyKey);
	return S_OK;
}

//
//  StopCaptureAsync()
//
//  Stop capture asynchronously via MF Work Item
//
HRESULT CLoopbackCapture::StopCaptureAsync()
{
	if (m_DeviceState != DeviceState::Capturing && m_DeviceState != DeviceState::Error)
		return E_NOT_VALID_STATE;

	m_DeviceState = DeviceState::Stopping;

	RETURN_IF_FAILED(MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, &m_xStopCapture, nullptr));

	// Wait for capture to stop
	m_hCaptureStopped.Wait();
	return S_OK;
}

bool		CLoopbackCapture::UpdateAudioBuffer(u32 sampleCount)
{
	const u32	waveformSampleCount = sampleCount * 2;
	{
		// Grab waveform buffer:
		PK_SCOPEDLOCK(m_CritSec);
		const ureg	audioBufferValidCount = m_AudioBufferValidSize / m_AudioBuffer.Stride();
	
		if (!PK_VERIFY(m_AudioBufferCopy.Resize(waveformSampleCount)))
			return false;
		if (m_DeviceState != DeviceState::Capturing)
			return false;
		if (waveformSampleCount <= audioBufferValidCount)
		{
			ureg		buffOffset = audioBufferValidCount - waveformSampleCount;
			Mem::Copy(m_AudioBufferCopy.RawDataPointer(), m_AudioBuffer.RawDataPointer() + buffOffset, waveformSampleCount * sizeof(float));
		}
		else
		{
			ureg		emptyCount = waveformSampleCount - audioBufferValidCount;
			Mem::Clear(m_AudioBufferCopy.RawDataPointer() + audioBufferValidCount, emptyCount);
			Mem::Copy(m_AudioBufferCopy.RawDataPointer(), m_AudioBuffer.RawDataPointer(), audioBufferValidCount * sizeof(float));
		}
	}
	// Compute spectrum:
	float		*audioBuffer = m_AudioBufferCopy.RawDataPointer();

	if (audioBuffer == null)
		return false;
	for (u32 i = 0; i < waveformSampleCount; ++i)
		audioBuffer[i] *= m_VolumeMultiplier;
	if (!PK_VERIFY(m_SpectrumBuffer.Resize(sampleCount)))
		return false;
	if (!PK_VERIFY(m_FFTInputData.Resize(waveformSampleCount) && m_FFTOutputData.Resize(waveformSampleCount)))
		return false;
	for (u32 i = 0; i < waveformSampleCount; ++i)
	{
		m_FFTInputData[i].real(audioBuffer[i]);
		m_FFTInputData[i].imag(0);
	}
	_BasicFFT(m_FFTInputData.RawDataPointer(), m_FFTOutputData.RawDataPointer(), IntegerTools::Log2(waveformSampleCount));
	for (u32 j = 0; j < sampleCount; ++j)
	{
		m_SpectrumBuffer[j] = PKSqrt(PKSquared(m_FFTOutputData[j].real()) + PKSquared(m_FFTOutputData[j].imag())) / static_cast<float>(sampleCount);
	}
	return true;
}

float	*CLoopbackCapture::GetAudioWaveform()
{
	return m_AudioBufferCopy.RawDataPointer();
}

float	*CLoopbackCapture::GetAudioSpectrum()
{
	return m_SpectrumBuffer.RawDataPointer();
}

void	CLoopbackCapture::SetVolumeMultiplier(float volume)
{
	m_VolumeMultiplier = volume;
}

void	CLoopbackCapture::_BasicFFT(std::complex<float> *inputs, std::complex<float> *outputs, u32 log2n)
{
	const float					kPI = TNumericConstants<float>::Pi();
	const std::complex<float>	kImag(0, 1);
	const u32					n = 1 << log2n;

	for (u32 i = 0; i < n; ++i)
	{
		outputs[IntegerTools::ReverseBits(i) >> (32 - log2n)] = inputs[i];
	}
	for (u32 s = 1; s <= log2n; ++s)
	{
		u32 m = 1 << s;
		u32 m2 = m >> 1;
		std::complex<float> w(1, 0);
		std::complex<float>	wm = exp(-kImag * (kPI / m2));
		for (u32 j = 0; j < m2; ++j)
		{
			for (u32 k = j; k < n; k += m)
			{
				std::complex<float>	t = w * outputs[k + m2];
				std::complex<float>	u = outputs[k];
				outputs[k] = u + t;
				outputs[k + m2] = u - t;
			}
			w *= wm;
		}
	}
}

//
//  OnStopCapture()
//
//  Callback method to stop capture
//
HRESULT CLoopbackCapture::OnStopCapture(IMFAsyncResult *pResult)
{
	(void)pResult;
	// Stop capture by cancelling Work Item
	// Cancel the queued work item (if any)
	if (0 != m_SampleReadyKey)
	{
		MFCancelWorkItem(m_SampleReadyKey);
		m_SampleReadyKey = 0;
	}

	m_AudioClient->Stop();
	return FinishCaptureAsync();
}

//
//  FinishCaptureAsync()
//
//  Finalizes WAV file on a separate thread via MF Work Item
//
HRESULT CLoopbackCapture::FinishCaptureAsync()
{
	// We should be flushing when this is called
	return MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, &m_xFinishCapture, nullptr);
}

//
//  OnFinishCapture()
//
//  Because of the asynchronous nature of the MF Work Queues and the DataWriter, there could still be
//  a sample processing.  So this will get called to finalize the WAV header.
//
HRESULT CLoopbackCapture::OnFinishCapture(IMFAsyncResult *pResult)
{
	(void)pResult;
	m_DeviceState = DeviceState::Stopped;
	m_hCaptureStopped.Trigger();
	return NOERROR;
}

//
//  OnSampleReady()
//
//  Callback method when ready to fill sample buffer
//
HRESULT CLoopbackCapture::OnSampleReady(IMFAsyncResult *pResult)
{
	(void)pResult;
	if (SUCCEEDED(OnAudioSampleRequested()))
	{
		// Re-queue work item for next sample
		if (m_DeviceState == DeviceState::Capturing)
		{
			// Re-queue work item for next sample
			return MFPutWaitingWorkItem(m_SampleReadyEvent.get(), 0, m_SampleReadyAsyncResult.get(), &m_SampleReadyKey);
		}
	}
	else
	{
		m_DeviceState = DeviceState::Error;
	}
	return S_OK;
}

//
//  OnAudioSampleRequested()
//
//  Called when audio device fires m_SampleReadyEvent
//
HRESULT CLoopbackCapture::OnAudioSampleRequested()
{
	UINT32	FramesAvailable = 0;
	BYTE	*dataWinInternalBuffer = nullptr;
	DWORD	dwCaptureFlags;
	UINT64	u64DevicePosition = 0;
	UINT64	u64QPCPosition = 0;
	DWORD	cbBytesToCapture = 0;

	PK_SCOPEDLOCK(m_CritSec);

	// If this flag is set, we have already queued up the async call to finialize the WAV header
	// So we don't want to grab or write any more data that would possibly give us an invalid size
	if (m_DeviceState == DeviceState::Stopping)
	{
		return S_OK;
	}

	// A word on why we have a loop here;
	// Suppose it has been 10 milliseconds or so since the last time
	// this routine was invoked, and that we're capturing 48000 samples per second.
	//
	// The audio engine can be reasonably expected to have accumulated about that much
	// audio data - that is, about 480 samples.
	//
	// However, the audio engine is free to accumulate this in various ways:
	// a. as a single packet of 480 samples, OR
	// b. as a packet of 80 samples plus a packet of 400 samples, OR
	// c. as 48 packets of 10 samples each.
	//
	// In particular, there is no guarantee that this routine will be
	// run once for each packet.
	//
	// So every time this routine runs, we need to read ALL the packets
	// that are now available;
	//
	// We do this by calling IAudioCaptureClient::GetNextPacketSize
	// over and over again until it indicates there are no more packets remaining.
	while (SUCCEEDED(m_AudioCaptureClient->GetNextPacketSize(&FramesAvailable)) && FramesAvailable > 0)
	{
		cbBytesToCapture = FramesAvailable * m_CaptureFormat.nBlockAlign;

		// Get sample buffer
		RETURN_IF_FAILED(m_AudioCaptureClient->GetBuffer(&dataWinInternalBuffer, &FramesAvailable, &dwCaptureFlags, &u64DevicePosition, &u64QPCPosition));

		// Write Buffer
		if (m_DeviceState != DeviceState::Stopping)
		{
			const ureg	audioBufferTotalSize = m_AudioBuffer.CoveredBytes();

			if (m_AudioBufferValidSize + cbBytesToCapture > audioBufferTotalSize)
			{
				ureg			bufferLeft = audioBufferTotalSize - m_AudioBufferValidSize;
				ureg			offsetAudioBuffer = cbBytesToCapture - bufferLeft;
				const u8	*srcToOffsetPtr = (u8*)Mem::AdvanceRawPointer(m_AudioBuffer.RawDataPointer(), offsetAudioBuffer);
				Mem::CopyT_Overlapped((u8*)m_AudioBuffer.RawDataPointer(), srcToOffsetPtr, m_AudioBufferValidSize - offsetAudioBuffer);
				void		*dstPtr = Mem::AdvanceRawPointer(m_AudioBuffer.RawDataPointer(), audioBufferTotalSize - cbBytesToCapture);
				Mem::Copy(dstPtr, dataWinInternalBuffer, cbBytesToCapture);
				m_AudioBufferValidSize = audioBufferTotalSize;
			}
			else
			{
				void	*dstPtr = Mem::AdvanceRawPointer(m_AudioBuffer.RawDataPointer(), m_AudioBufferValidSize);
				Mem::Copy(dstPtr, dataWinInternalBuffer, cbBytesToCapture);
				m_AudioBufferValidSize += cbBytesToCapture;
			}
		}
		// Release buffer back
		m_AudioCaptureClient->ReleaseBuffer(FramesAvailable);
	}
	return S_OK;
}

bool	CWindowLoopbackCapture::StartCapturing()
{
	m_LoopbackAudio.StopCaptureAsync();
	// Start audio loopback:
	DWORD	processId = GetCurrentProcessId();
	HRESULT	hr = m_LoopbackAudio.StartCaptureAsync(processId, true);
	return hr == S_OK;
}

bool	CWindowLoopbackCapture::StopCapturing()
{
	HRESULT	hr = m_LoopbackAudio.StopCaptureAsync();
	return hr == S_OK;
}

bool	CWindowLoopbackCapture::UpdateAudioBuffer(u32 sampleCount)
{
	return m_LoopbackAudio.UpdateAudioBuffer(sampleCount);
}

float	*CWindowLoopbackCapture::GetAudioWaveform()
{
	return m_LoopbackAudio.GetAudioWaveform();
}

float	*CWindowLoopbackCapture::GetAudioSpectrum()
{
	return m_LoopbackAudio.GetAudioSpectrum();
}

void	CWindowLoopbackCapture::SetVolumeMultiplier(float volume)
{
	m_LoopbackAudio.SetVolumeMultiplier(volume);
}
