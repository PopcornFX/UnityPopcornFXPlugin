//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxAsset : ScriptableObject
	{
		[HideInInspector]
		public byte[] _data;

		[HideInInspector]
		public virtual byte[] m_Data
		{
			get { return _data; }
			set { _data = value; }
		}
		[SerializeField]
		internal string m_AssetFullPath = "";
		[SerializeField]
		private string m_AssetVirtualPath = "";

		public string AssetVirtualPath
		{
			get
			{
				if (string.IsNullOrEmpty(m_AssetVirtualPath))
				{
					string fullPath = m_AssetFullPath;
					m_AssetVirtualPath = fullPath.Replace("Assets" + PKFxSettings.UnityPackFxPath + "/", "");
				}
				return m_AssetVirtualPath;
			}
			set
			{
				m_AssetVirtualPath = value;
				m_AssetFullPath = "Assets" + PKFxSettings.UnityPackFxPath + "/" + value;
			}
		}

		public string AssetFullPath
		{
			get
			{
				return m_AssetFullPath;
			}
			set
			{
				// Should not use the PKFxSettings here! It might not be loaded when the assets are being imported
				m_AssetFullPath = value;
			}
		}

		public virtual void Clean()
		{
			_data = null;
		}
	}
}