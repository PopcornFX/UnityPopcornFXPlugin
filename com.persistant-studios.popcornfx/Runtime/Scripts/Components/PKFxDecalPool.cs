using PopcornFX;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class PKFxDecalPool : MonoBehaviour
{
	[StructLayout(LayoutKind.Sequential)]
	public struct SDecalInfo
	{
		public Vector3		m_Position;
		public Vector3		m_Scale;
		public Quaternion	m_Orientation;
		public IntPtr		m_DiffuseMap;
		public IntPtr		m_EmissiveMap;
		public Vector4		m_DiffuseColor;
		public Vector4		m_EmissiveColor;
		public float		m_AtlasID;
		public int			m_UID;
	}

	public List<PKFxDecalData> m_Decals = new List<PKFxDecalData>();

	public int m_MaxDecalNumber = 0;

	private GameObject		m_Template;
	private GameObject		m_DecalRoot;
	private List<IntPtr>	m_Buffer = new List<IntPtr>();
	private List<int>		m_BufferCount = new List<int>();
	private uint			m_MaxActive;

	private PKFxMaterialFactory m_Factory;


	private void Start()
	{
		m_DecalRoot = new GameObject("PopcornFX Decal Root");
		m_DecalRoot.transform.SetParent(gameObject.transform);
		m_Factory = PKFxSettings.MaterialFactory;
		m_Template = m_Factory.GetDecalTemplate();

		m_Template.transform.SetParent(m_DecalRoot.transform);

		m_Template.SetActive(false);

		for (int i = 0; i < m_MaxDecalNumber; ++i)
		{
			GameObject obj = Instantiate<GameObject>(m_Template);
			obj.transform.SetParent(m_DecalRoot.transform);

			m_Decals.Add(obj.GetComponent<PKFxDecalData>());
		}
		m_MaxActive = 0;
	}

	public void AddDecalBuffer(IntPtr decalInfos, int count)
	{
		if (decalInfos == IntPtr.Zero) 
			return;
		m_Buffer.Add(decalInfos);
		m_BufferCount.Add(Mathf.Min(count, m_MaxDecalNumber));
	}

	public void DrawDecalRenderers()
	{
		int i = 0;
		uint activeAtFrame = 0;

		unsafe
		{
			for (int j = 0; j < m_Buffer.Count; j++)
			{
				IntPtr ptr = m_Buffer[j];
				SDecalInfo* currentPtr = (SDecalInfo*)ptr.ToPointer();

				uint k = 0;
				while (k < m_BufferCount[j] && i < m_MaxDecalNumber)
				{
					SDecalInfo decalInfo = currentPtr[k];
					PKFxDecalData decal = m_Decals[i];

					decal.gameObject.SetActive(true);
					m_Factory.SetDecalValue(decal, decalInfo);
					++i;
					++k;
				}
				activeAtFrame += k;
			}
		}
		while (i < m_MaxActive)
		{
			m_Decals[i].gameObject.SetActive(false);
			m_Decals[i].Reset();
			++i;
		}
		m_MaxActive = activeAtFrame;
		m_Buffer.Clear();
		m_BufferCount.Clear();
	}
}
