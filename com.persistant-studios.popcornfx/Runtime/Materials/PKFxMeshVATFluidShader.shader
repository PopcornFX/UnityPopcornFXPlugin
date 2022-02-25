Shader "Unlit/PKFxMeshVATFluidShader"
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
		[NoScaleOffset]_ColorMap("Color Map", 2D) = "white" {}
		_NumFrames("Num Frames", Int) = 99
		_Bounds("Bounds", Vector) = (-1,1,0,0)
		[Toggle] _PackedData("Packed Normals", Int) = 0
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
			sampler2D _NormalMap;
			sampler2D _ColorMap;

			v2f vert(appdata_full v)
			{
				UNITY_SETUP_INSTANCE_ID(v);

				v2f o;
				o.uv = TRANSFORM_TEX(v.texcoord, _MainTex);
				o.uv1 = v.texcoord1;

				float cursor = (ceil(saturate(UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _Cursor)) * (_NumFrames - 1))) / _NumFrames;
				float2 samplingUVs = float2(v.texcoord.x, v.texcoord.y - cursor);
				
				if (_PadToPowerOf2)
					samplingUVs *= float2(_PaddedRatio.x, _PaddedRatio.y);

				float4 p = tex2Dlod(_PositionMap, float4(samplingUVs, 0, 0));
				o.vertex = UnityObjectToClipPos(ConvertSpace(lerp(_Bounds.x, _Bounds.y, p.xyz)));

				float4 c = tex2Dlod(_ColorMap, float4(samplingUVs, 0, 0));
				o.color = c;

				if (_PackedData)
				{
					o.normal = ConvertSpace(UnpackAlpha(p.w));
				}
				else
				{
					o.normal = ConvertSpace(tex2Dlod(_NormalMap, p));
				}

				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col = tex2D(_MainTex, i.uv) * UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);

				col *= i.color;

				return col;
			}
			ENDCG
		}
	}
}
