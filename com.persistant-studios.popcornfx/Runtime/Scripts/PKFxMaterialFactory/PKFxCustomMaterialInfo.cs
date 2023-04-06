//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
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
		public int		m_UID;
		public Material m_CustomMaterial;
	}
}