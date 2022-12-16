//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/PostFX_Blur"
{
	Properties
	{
		_MainTex("Base (RGB)", 2D) = "white" {}
		_DistortionTex("Distortion (RGB)", 2D) = "black" {}
		_BlurFactor("Blur factor [ 0 ; 1 ]", Range(0, 1)) = 0.2
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

			struct v2f
			{
				half4 pos : POSITION;
				half2 uv : TEXCOORD0;
			};

			sampler2D	_MainTex;
			half4 		_MainTex_ST;
			sampler2D	_DistortionTex;
			half4 		_DistortionTex_ST;
			float		_BlurFactor;

			v2f vert(appdata_img v)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(v.vertex);
				o.uv.xy = v.texcoord.xy;
				return o;
			}

			half4 frag(v2f i) : COLOR
			{
				half4 color = tex2D(_MainTex, UnityStereoScreenSpaceUVAdjust(i.uv, _MainTex_ST)) * half(0.325).xxxx;
		
				float2	offsets = float(tex2D(_DistortionTex, UnityStereoScreenSpaceUVAdjust(i.uv, _DistortionTex_ST)).z * _BlurFactor / 20.0).xx;
		
				half4 b1 = offsets.xyxy * half4(1,1, -1,-1);
				half4 b2 = offsets.xyxy * half4(1,-1, -1,1);
		
				half4 uv01 = i.uv.xyxy + b1;
				half4 uv02 = i.uv.xyxy + b2;
				half4 uv21 = i.uv.xyxy + b1 * 1.5;
				half4 uv22 = i.uv.xyxy + b2 * 1.5;
		
				color += 0.1125 * tex2D(_MainTex, uv01.xy);
				color += 0.1125 * tex2D(_MainTex, uv02.xy);
				color += 0.1125 * tex2D(_MainTex, uv01.zw);
				color += 0.1125 * tex2D(_MainTex, uv02.zw);
		
				color += 0.05625 * tex2D(_MainTex, uv21.xy);
				color += 0.05625 * tex2D(_MainTex, uv22.xy);
				color += 0.05625 * tex2D(_MainTex, uv21.zw);
				color += 0.05625 * tex2D(_MainTex, uv22.zw);
				return color;
			}
			ENDCG
		}
	}

Fallback off

}