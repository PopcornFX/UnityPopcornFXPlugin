//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using PopcornFX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using UnityEngine;
//----------------------------------------------------------------------------

public class PKFxAudioPool : MonoBehaviour
{
	[StructLayout(LayoutKind.Sequential)]
	public struct SSoundInfo
	{
		public Vector2Int	m_SelfID;
		public float		m_Age;
		public Vector3		m_Position;
		public Vector3		m_Velocity;
		public float		m_Radius;
		public float		m_DopplerLevel;
		public float		m_Volume;
		public bool			m_Audible;
		public IntPtr		m_SoundData;
	}

	public struct SSoundComponent
	{
		public SSoundInfo	m_Info;
		public AudioSource	m_AudioSource;
		public bool			m_IsUpdated;
	}


	public Dictionary<int, SSoundComponent> m_SoundComponents = new Dictionary<int, SSoundComponent>();
	public List<AudioSource> m_Sounds = new List<AudioSource>();
	public int m_MaxSoundNumber = 0;


	private IntPtr m_Buffer;
	private int m_BufferCount;

	private void Start()
	{
		if (m_MaxSoundNumber > 0)
		{
			GameObject template = CreateSoundTemplate();

			for (int i = 1; i < m_MaxSoundNumber; ++i)
			{
				GameObject obj = Instantiate<GameObject>(template);
				obj.transform.SetParent(gameObject.transform);
				m_Sounds.Add(obj.GetComponent<AudioSource>());
			}
		}
	}

	public void SetSoundBuffer(IntPtr soundInfos, int count)
	{
		m_Buffer = soundInfos;
		m_BufferCount = Mathf.Min(count, m_MaxSoundNumber);
	}

	public void DrawSoundRenderers()
	{
		int i = 0;

		unsafe
		{
			if ((SSoundInfo*)m_Buffer == null)
				return;
			
			while (i < m_BufferCount)
			{
				SSoundInfo* currentPtr = (SSoundInfo*)m_Buffer.ToPointer();
				SSoundInfo soundInfo = currentPtr[i];

				SSoundComponent soundComponent;
				int Id = soundInfo.m_SelfID.GetHashCode();
				if (!m_SoundComponents.ContainsKey(Id))
				{
					soundComponent = new SSoundComponent();
					foreach (AudioSource audioSource in m_Sounds)
					{
						if (!audioSource.gameObject.activeSelf)
						{
							soundComponent.m_AudioSource = audioSource;
							break;
						}
					}
				   
					soundComponent.m_Info = soundInfo;
					soundComponent.m_IsUpdated = true;

					m_SoundComponents.Add(Id, soundComponent);
				}
				else
				{
					soundComponent = m_SoundComponents[Id];
					soundComponent.m_Info = soundInfo;
					soundComponent.m_IsUpdated = true;
					m_SoundComponents[Id] = soundComponent;
				}
				AudioSource sound = soundComponent.m_AudioSource;

				if (soundInfo.m_SoundData != null)
				{
					string soundPath = Marshal.PtrToStringAnsi(soundInfo.m_SoundData);

					AudioClip clip = PKFxManager.GetSoundFromPath(soundPath);
					if (clip == null)
					{
						Debug.LogWarning($"[PopcornFX] Could not find the AudioClip \"{soundPath}\".");
					}
					else if (!sound.isPlaying)
					{
						if (sound.clip != clip)
						{
							sound.Stop();
							sound.clip = clip;
						}
						sound.gameObject.SetActive(true);
						sound.Play();
					}
				}
				SetSoundValue(sound, soundInfo);
				i++;
			}
		}
		int[] keys = m_SoundComponents.Keys.ToArray();
		for (int j = 0; j < m_SoundComponents.Keys.Count; j++)
		{
			int id = keys[j];

			SSoundComponent soundComponent = m_SoundComponents[id];

			if (m_SoundComponents[id].m_IsUpdated)
			{
				soundComponent.m_IsUpdated = false;
			}
			else if (soundComponent.m_AudioSource.gameObject.activeSelf)
			{
				soundComponent.m_AudioSource.gameObject.SetActive(false);
				soundComponent.m_AudioSource.Stop();
				soundComponent.m_AudioSource.clip = null;
			}
			else
			{
				m_SoundComponents.Remove(id);
				break;
			}
			m_SoundComponents[id] = soundComponent;
		}

		m_BufferCount = 0;
	}

	public virtual GameObject CreateSoundTemplate()
	{
		GameObject template = new GameObject("PopcornFX Sound");
		template.transform.SetParent(gameObject.transform);
		template.SetActive(false);

		AudioSource audioComponent = template.AddComponent<AudioSource>();
		audioComponent.spatialBlend = 1;
		m_Sounds.Add(audioComponent);

		return template;
	}

	public virtual void SetSoundValue(AudioSource audioSource, PKFxAudioPool.SSoundInfo info)
	{
		audioSource.transform.position = info.m_Position;
		audioSource.volume = info.m_Volume;
		audioSource.dopplerLevel = info.m_DopplerLevel;
		audioSource.maxDistance = info.m_Radius;
	}
}