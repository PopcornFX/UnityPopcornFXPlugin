#pragma once

#include <pk_kernel/include/kr_refptr.h>

class	ILoopbackCapture : public CRefCountedObject
{
public:
	ILoopbackCapture() { }
	virtual ~ILoopbackCapture() { }

	virtual bool	StartCapturing() = 0;
	virtual bool	StopCapturing() = 0;

	virtual bool	UpdateAudioBuffer(u32 sampleCount) = 0;
	virtual float	*GetAudioWaveform() = 0;
	virtual float	*GetAudioSpectrum() = 0;
	virtual void	SetVolumeMultiplier(float volume) = 0;

	static ILoopbackCapture		*CreateLoopbackCapture();
};
PK_DECLARE_REFPTRINTERFACE(LoopbackCapture);
