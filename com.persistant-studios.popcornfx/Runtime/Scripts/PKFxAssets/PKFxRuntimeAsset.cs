//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxRuntimeAsset : MonoBehaviour
	{
		public byte[] _data;

		public virtual byte[] m_Data
		{
			get { return _data; }
			set { _data = value; }
		}
		public string m_AssetVirtualPath = "";
	}
}