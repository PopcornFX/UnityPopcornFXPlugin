//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using UnityEngine;
using System.Collections.Generic;
//----------------------------------------------------------------------------

namespace PopcornFX
{
	public class PKFxLightPool : MonoBehaviour
	{
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

		private IntPtr		m_Buffer;
		private int			m_BufferCount;
		private int			m_MaxActive;

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

		public void SetLightBuffer(IntPtr lightInfos, int count)
		{
			m_Buffer = lightInfos;
			m_BufferCount = Mathf.Min(count, m_MaxLightNumber);
		}

		public void DrawLightRenderers()
		{
			int i = 0;

			unsafe
			{
				SLightInfo* currentPtr = (SLightInfo*)m_Buffer.ToPointer();
				while (i < m_BufferCount)
				{
					SLightInfo	lightInfo = currentPtr[i];
					Light		light = m_Light[i];

					light.gameObject.SetActive(true);
					m_Factory.SetLightValue(light, lightInfo);
					++i;
				}
			}
			while (i < m_MaxActive)
			{
				m_Light[i].gameObject.SetActive(false);
				++i;
			}
			m_MaxActive = m_BufferCount;

			m_BufferCount = 0;
		}

	}
}
