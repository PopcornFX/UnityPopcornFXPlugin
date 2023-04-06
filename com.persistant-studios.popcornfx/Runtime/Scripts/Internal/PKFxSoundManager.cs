//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.Serialization;
using System.Collections;
using System.Runtime.InteropServices;
using System;
using System.IO;
using System.Collections.Generic;
using AOT;

namespace PopcornFX
{
	public class PKFxSoundManager
	{
		[Serializable]
		public class SoundDescriptor
		{
			public SoundDescriptor(SNativeSoundDescriptor desc)
			{
				m_ChannelGroup = desc.m_ChannelGroup;
				m_Path = Marshal.PtrToStringAnsi(desc.m_Path);
				m_EventStart = Marshal.PtrToStringAnsi(desc.m_EventStart);
				m_EventStop = Marshal.PtrToStringAnsi(desc.m_EventStop);
				m_WorldPosition = new Vector3(desc.m_WorldPositionX, desc.m_WorldPositionY, desc.m_WorldPositionZ);
				m_Volume = desc.m_Volume;
				m_StartTimeOffsetInSeconds = desc.m_StartTimeOffsetInSeconds;
				m_PlayTimeInSeconds = desc.m_PlayTimeInSeconds;
				m_UserData = desc.m_UserData;
			}

			[FormerlySerializedAs("ChannelGroup")] public int m_ChannelGroup;
			[FormerlySerializedAs("Path")] public string m_Path;
			[FormerlySerializedAs("EventStart")] public string m_EventStart;
			[FormerlySerializedAs("EventStop")] public string m_EventStop;
			[FormerlySerializedAs("WorldPosition")] public Vector3 m_WorldPosition;
			[FormerlySerializedAs("Volume")] public float m_Volume;
			[FormerlySerializedAs("StartTimeOffsetInSeconds")] public float m_StartTimeOffsetInSeconds;
			[FormerlySerializedAs("PlayTimeInSeconds")] public float m_PlayTimeInSeconds;
			[FormerlySerializedAs("UserData")] public int m_UserData;
		}

		private delegate void VoidFPtr(IntPtr ptr);
		public delegate void StartSoundDelegate(SoundDescriptor soundDesc);

		private static StartSoundDelegate m_OnStartSoundDelegate = null;
		private static GameObject m_SpawnedSoundRoot;
		private static List<AudioSource> m_SpawnedSound = new List<AudioSource>();
		private static Dictionary<string, AudioClip> m_Sounds = new Dictionary<string, AudioClip>();

		//----------------------------------------------------------------------------

		public static void ClearSounds()
		{
			m_SpawnedSound.Clear();
		}

		//----------------------------------------------------------------------------

		public static void DeleteSoundsIFN()
		{
			if (m_OnStartSoundDelegate == null && m_SpawnedSound != null)
			{
				for (int i = m_SpawnedSound.Count - 1; i >= 0; i--)
				{
					AudioSource it = m_SpawnedSound[i];
					if (!it.isPlaying)
					{
						GameObject.Destroy(it.gameObject);
						m_SpawnedSound.RemoveAt(i);
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		public delegate void StartSoundCallback(IntPtr actionFactorySound);

		[MonoPInvokeCallback(typeof(StartSoundCallback))]
		public static void OnStartSound(IntPtr actionFactorySound)
		{
			SoundDescriptor sound = null;

			unsafe
			{
				SNativeSoundDescriptor* desc = (SNativeSoundDescriptor*)actionFactorySound.ToPointer();
				sound = new SoundDescriptor(*desc);
			}

			if (m_OnStartSoundDelegate != null)
			{
				m_OnStartSoundDelegate(sound);
				return;
			}

			string soundPath = sound.m_Path;

			AudioClip clip = null;
			if (m_Sounds.ContainsKey(soundPath) == true)
			{
				clip = m_Sounds[soundPath];
			}
			if (clip != null)
			{
				if (m_SpawnedSoundRoot == null)
				{
					m_SpawnedSoundRoot = new GameObject("PopcornFX Sounds");
					m_SpawnedSoundRoot.transform.position = Vector3.zero;
					m_SpawnedSoundRoot.isStatic = true;
				}

				GameObject soundGo = new GameObject("FxSound");
				if (soundGo != null)
				{
					soundGo.transform.parent = m_SpawnedSoundRoot.transform;
					soundGo.transform.position = sound.m_WorldPosition;
					AudioSource soundSource = soundGo.AddComponent<AudioSource>();
					if (soundSource != null)
					{
						soundSource.clip = clip;
						soundSource.Play();
						soundSource.volume = sound.m_Volume;
						soundSource.time = sound.m_StartTimeOffsetInSeconds;
						soundSource.spatialBlend = 1.0f;
						if (sound.m_PlayTimeInSeconds != 0.0f)
							GameObject.Destroy(soundSource, sound.m_PlayTimeInSeconds);
						else
							m_SpawnedSound.Add(soundSource);
					}
				}
			}
			else
				Debug.LogError("[PopcornFX] Could not load sound layer " + soundPath);
		}

		//----------------------------------------------------------------------------

		public static void RegisterCustomHandler(StartSoundDelegate customDelegate)
		{
			m_OnStartSoundDelegate = customDelegate;
		}

		internal static void AddSound(PKFxEffectAsset.DependencyDesc depDesc)
		{
			m_Sounds[depDesc.m_Path] = depDesc.m_Object as AudioClip;
		}
	}
}