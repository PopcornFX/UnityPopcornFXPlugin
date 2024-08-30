//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
//----------------------------------------------------------------------------

namespace PopcornFX
{
	public class PKFxLightPool : MonoBehaviour
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct SLightInfo
		{
			public Vector3	m_Position;
			public Vector4	m_Color;
			public float	m_Range;
			public float	m_Intensity;
		}

		public List<Light>	m_Light = new List<Light>();
		public int			m_MaxLightNumber = 0;

		private GameObject	m_Template;
		private GameObject	m_LightRoot;

		private List<IntPtr>	m_Buffer = new List<IntPtr>();
		private List<int>		m_BufferCount = new List<int>();
		private uint			m_MaxActive;

		private PKFxMaterialFactory m_Factory;


		private void Start()
		{
			m_LightRoot = new GameObject("PopcornFX Lights Root");
			m_LightRoot.transform.SetParent(gameObject.transform);
			m_Factory = PKFxSettings.MaterialFactory;
			m_Template = m_Factory.GetLightTemplate();

			m_Template.transform.SetParent(m_LightRoot.transform);

			m_Template.SetActive(false);

			for (int i = 0; i < m_MaxLightNumber; ++i)
			{
				GameObject obj = Instantiate<GameObject>(m_Template);
				obj.transform.SetParent(m_LightRoot.transform);

				m_Light.Add(obj.GetComponent<Light>());
			}
			m_MaxActive = 0;
		}

		public void AddLightBuffer(IntPtr lightInfos, int count)
		{
			m_Buffer.Add(lightInfos);
			m_BufferCount.Add(Mathf.Min(count, m_MaxLightNumber));
		}

		public void DrawLightRenderers()
		{
			int		i = 0;
			uint	activeAtFrame = 0;
			int		lightsCount = Mathf.Min(m_Buffer.Count, m_MaxLightNumber);

			unsafe
			{
				for (int j = 0; j < m_Buffer.Count; j++)
				{
					IntPtr ptr = m_Buffer[j];
					SLightInfo* currentPtr = (SLightInfo*)ptr.ToPointer();

					uint k = 0;
					while (k < m_BufferCount[j] && i < m_MaxLightNumber)
					{
						SLightInfo	lightInfo = currentPtr[k];
						Light		light = m_Light[i];

						light.gameObject.SetActive(true);
						m_Factory.SetLightValue(light, lightInfo);
						++i;
						++k;
					}
					activeAtFrame += k;
				}
			}
			while (i < m_MaxActive)
			{
				m_Light[i].gameObject.SetActive(false);
				++i;
			}
			m_MaxActive = activeAtFrame;
			m_Buffer.Clear();
			m_BufferCount.Clear();
		}
	}
}