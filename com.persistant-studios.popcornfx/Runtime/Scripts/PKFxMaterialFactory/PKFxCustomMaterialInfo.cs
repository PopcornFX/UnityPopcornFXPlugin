//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	[Serializable]
	public class PKFxCustomMaterialInfo : PKFxShaderInputBindings
	{
		public string	m_AssetVirtualPath;
		public string	m_BatchDescName;
		public uint		m_UID;
		public Material m_CustomMaterial;
	}
}