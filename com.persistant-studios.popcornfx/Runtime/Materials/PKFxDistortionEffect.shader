// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "PopcornFX/PKFxDistortion"
{
	Properties
	{
		_MainTex("Base (RGB)", 2D) = "white" {}
		_DistortionTex("Distortion (RGB)", 2D) = "black" {}
	}

	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always
		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"
	
			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
			};
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}

			sampler2D	_MainTex;
			half4 		_MainTex_ST;
			sampler2D	_DistortionTex;
			half4 		_DistortionTex_ST;

			float4 SampleColorAt(float2 uvOffset)
			{
				return tex2D(_MainTex, uvOffset);
			}

			float4 frag (v2f i) : SV_Target
			{
				// get the distortion MAP
				float2  sreenUV = UnityStereoScreenSpaceUVAdjust(i.uv, _DistortionTex_ST);
				float4	distoApply = tex2D(_DistortionTex, sreenUV) * 3.0;

				// Chromatic aberation
				float4	color0 = SampleColorAt(sreenUV + distoApply.xy * 0.0100) * float4(0.5, 0.0, 0., 0.0);
				float4	color1 = SampleColorAt(sreenUV + distoApply.xy * 0.0125) * float4(0.5, 0.5, 0., 0.5);
				float4	color2 = SampleColorAt(sreenUV + distoApply.xy * 0.0150) * float4(0.0, 0.5, 0., 0.5);
				float4	color3 = SampleColorAt(sreenUV + distoApply.xy * 0.0175) * float4(0.0, 0.0, 1., 0.0);

				return color0 + color1 + color2 + color3;
			}
ENDCG
		}
	}

Fallback off

}
