//--------------------------------------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//--------------------------------------------------------------------------------------------------------
Shader "Unlit/PKFxMeshVATRigidShader"
{
	Properties
	{
		 _Color("Color", Color) = (1,1,1,1)
		_MainTex("Albedo (RGB)", 2D) = "white" {}
		_Glossiness("Smoothness", Range(0,1)) = 0.5
		_Metallic("Metallic", Range(0,1)) = 0.0
		_SrcBlendMode("Src Blend Mode", Int) = 0
		_DstBlendMode("Dst Blend Mode", Int) = 0
		_ZTestMode("ZTest Mode", Int) = 0

		[NoScaleOffset]_PositionMap("Position Map", 2D) = "white" {}
		[NoScaleOffset]_RotationMap("Rotation Map", 2D) = "white" {}
		_NumFrames("Num Frames", Int) = 99
		_Bounds("Bounds", Vector) = (-1,1,0,0)
		_Pivot("Pivot", Vector) = (-1,1,0,0)
	}
	SubShader
	{
		Tags
		{
			"Queue" = "Geometry"
			"RenderType" = "Opaque"
		}
		LOD 200

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			// Use shader model 3.0 target, to get nicer looking lighting
			#pragma target 3.0

			// Enable instancing for this shader
			#pragma multi_compile_instancing

			#include "UnityCG.cginc"

			UNITY_INSTANCING_BUFFER_START(InstanceProperties)
			UNITY_DEFINE_INSTANCED_PROP(fixed4, _BaseColor)	// Make _BaseColor an instanced property (i.e. an array)
			UNITY_DEFINE_INSTANCED_PROP(float, _Cursor)
			UNITY_INSTANCING_BUFFER_END(InstanceProperties)

			#include "PKFxShaderCode/MeshVAT.inc"

			sampler2D _PositionMap;
			sampler2D _RotationMap;
			float4 _Pivot;

			v2f vert(appdata_full v)
			{
				UNITY_SETUP_INSTANCE_ID(v);

				v2f o;
				o.uv = TRANSFORM_TEX(v.texcoord, _MainTex);
				o.uv1 = v.texcoord1;
				o.color = fixed4(1, 1, 1, 1);

				float cursor = (ceil(saturate(UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _Cursor)) * (_NumFrames - 1))) / _NumFrames;
				float4 samplingUVs = float4(v.texcoord1.x, v.texcoord1.y - cursor, 0, 0);

				if (_PadToPowerOf2)
					samplingUVs *= float4(_PaddedRatio.x, _PaddedRatio.y, 1, 1);

				float4 p = tex2Dlod(_PositionMap, samplingUVs);
				float4 r = tex2Dlod(_RotationMap, samplingUVs);

				float3 offsets = ConvertSpace(lerp(_Bounds.x, _Bounds.y, p.xyz));
				float3 pivot = ConvertSpace(lerp(_Pivot.x, _Pivot.y, v.color));

				float4 q = r * 2.f - 1.f;

				q.xyz = ConvertSpace(q.xyz);

				o.vertex = UnityObjectToClipPos(RotateVector(v.vertex - pivot, q) + offsets);
				o.normal = RotateVector(v.normal, q);
				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col = tex2D(_MainTex, i.uv) * UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);
				return col;
			}
			ENDCG
		}
	}
}
