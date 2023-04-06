//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using AOT;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxRaycasts
	{
		public delegate void RaycastPackCallback(IntPtr raycastQuery);

		[MonoPInvokeCallback(typeof(RaycastPackCallback))]
		public static void OnRaycastPack(IntPtr raycastQuery)
		{
			if (PKFxSettings.CustomRaycast != null)
			{
				PKFxSettings.CustomRaycast(raycastQuery);
				return;
			}
			unsafe
			{
				SRaycastPack* raycastPack = (SRaycastPack*)raycastQuery.ToPointer();
				Vector4* origins = (Vector4*)raycastPack->m_RayOrigins.ToPointer();
				Vector4* directions = (Vector4*)raycastPack->m_RayDirections.ToPointer();
				Vector4* resNormals = (Vector4*)raycastPack->m_OutNormals.ToPointer();
				Vector4* resPositions = (Vector4*)raycastPack->m_OutPositions.ToPointer();
				float* resDistance = (float*)raycastPack->m_OutDistances.ToPointer();

				uint raycastsCount = (uint)raycastPack->m_RayCount;

				int layerMask = raycastPack->m_FilterLayer == 0 ? Physics.DefaultRaycastLayers : (1 << raycastPack->m_FilterLayer);

				for (uint i = 0; i < raycastsCount; ++i)
				{
					RaycastHit rayHit;
					bool hit = Physics.Raycast(origins[i], directions[i], out rayHit, directions[i].w, layerMask);

					if (PKFxSettings.DebugEffectsRaycasts)
						Debug.DrawRay(origins[i], directions[i] * directions[i].w, Color.red);

					if (resNormals != null)
						resNormals[i] = rayHit.normal;
					if (resPositions != null)
						resPositions[i] = rayHit.point;
					if (hit)
						resDistance[i] = rayHit.distance;
					else
						resDistance[i] = float.PositiveInfinity;
				}
			}
		}

	}
}