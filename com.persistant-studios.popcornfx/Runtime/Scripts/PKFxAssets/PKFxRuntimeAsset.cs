//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
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