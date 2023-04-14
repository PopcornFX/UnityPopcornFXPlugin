//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using AOT;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace PopcornFX
{
	[RequireComponent(typeof(AudioSource))]
	public class PKFxAudioSourceSampler : MonoBehaviour
	{
		public string channel;

		private static Dictionary<string, PKFxAudioSourceSampler> m_AudioSources = new Dictionary<string, PKFxAudioSourceSampler>();
		private static bool m_DelegateSet = false;

		private float[] m_Samples;
		private GCHandle m_SamplesHandle;
		private AudioSource m_Source;
		private void Start()
		{
			if (!m_DelegateSet)
			{
				PKFxManager.RegisterAudioSpectrumData(new PKFxManager.AudioCallback(OnAudioSpectrumData));
				PKFxManager.RegisterAudioWaveformData(new PKFxManager.AudioCallback(OnAudioWaveformData));
				m_DelegateSet = true;
			}

			Init();
		}

		public void Init()
		{
			// Initialize the data buffer. Expected size is 1024.
			m_Samples = new float[1024];
			// Pin the array
			m_SamplesHandle = GCHandle.Alloc(m_Samples, GCHandleType.Pinned);
			string key = channel;
			if (string.IsNullOrEmpty(key))
				key = "Master";
			m_Source = GetComponent<AudioSource>();
			if (m_AudioSources.ContainsKey(key))
				m_AudioSources[key] = this;
			else 
				m_AudioSources.Add(key, this);
		}

		void Update()
		{
		}

		[MonoPInvokeCallback(typeof(PKFxManager.AudioCallback))]
		public static IntPtr OnAudioSpectrumData(IntPtr channelName, IntPtr nbSamples)
		{
			string name = Marshal.PtrToStringAnsi(channelName);
			PKFxAudioSourceSampler audioSrc = null;
			if (m_AudioSources.TryGetValue(name, out audioSrc))
			{
				audioSrc.m_Source.GetSpectrumData(audioSrc.m_Samples, 0, FFTWindow.Rectangular);
				// Last value filled by Unity seems wrong...
				audioSrc.m_Samples[1023] = audioSrc.m_Samples[1022];
				// return the address of the pinned object.
				return audioSrc.m_SamplesHandle.AddrOfPinnedObject();
			}
			return PKFxManagerImpl.OnAudioSpectrumData(channelName, nbSamples);
		}

		[MonoPInvokeCallback(typeof(PKFxManager.AudioCallback))]
		public static IntPtr OnAudioWaveformData(IntPtr channelName, IntPtr nbSamples)
		{
			string name = Marshal.PtrToStringAnsi(channelName);
			PKFxAudioSourceSampler audioSrc = null;
			if (m_AudioSources.TryGetValue(name, out audioSrc))
			{
				audioSrc.m_Source.GetOutputData(audioSrc.m_Samples, 0);
				// return the address of the pinned object.
				return audioSrc.m_SamplesHandle.AddrOfPinnedObject();
			}
			return PKFxManagerImpl.OnAudioWaveformData(channelName, nbSamples);
		}

		void OnDestroy()
		{
			m_AudioSources.Remove(channel);
			m_SamplesHandle.Free();
		}
	}
}