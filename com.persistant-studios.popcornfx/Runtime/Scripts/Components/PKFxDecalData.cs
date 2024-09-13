using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace PopcornFX
{
	public class PKFxDecalData : MonoBehaviour
	{
		public Material		m_Material;
		public string		m_MaterialID;
		public Texture2D	m_AtlasRects;
		public uint			m_UID;

		public void Reset()
		{
			m_Material = null;
			m_MaterialID = "";
			m_AtlasRects = null;
			m_UID = 0;
		}
	}
}