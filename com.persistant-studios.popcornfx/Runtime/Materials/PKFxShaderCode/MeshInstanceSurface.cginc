//--------------------------------------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//--------------------------------------------------------------------------------------------------------
sampler2D _MainTex;

#if !PK_HAS_EMISSIVE_NONE
sampler2D _EmissiveMap;
#	if PK_HAS_EMISSIVE_WITH_RAMP
sampler2D _EmissiveRampMap;
#	endif
#endif

#if PK_HAS_ALPHA_REMAP
sampler2D _AlphaMap;
#endif

#if PK_HAS_DIFFUSE_RAMP
sampler2D _DiffuseRampMap;
#endif

#if		!PK_IS_TRANSPARENT
	half _CutOutAlpha;
#endif

#if PK_IS_LIT
	half		_Roughness;
	half		_Metalness;
	sampler2D	_NormalMap;
#endif


#if PK_IS_LIT
#	define SURF_OUTPUT		SurfaceOutputStandard
#else
#	define SURF_OUTPUT		SurfaceOutput
#endif

#if	SHADER_API_D3D11 || SHADER_API_D3D9 || SHADER_API_XBOXONE || SHADER_API_METAL || SHADER_API_PSSL || SHADER_API_SWITCH
	#define	CLIP_Y_REVERSED		1
#else
	#define	CLIP_Y_REVERSED		0
#endif

#if	SHADER_API_D3D11 || SHADER_API_D3D9 || SHADER_API_XBOXONE || SHADER_API_METAL || SHADER_API_PSSL || SHADER_API_SWITCH
	#define	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE		0
#else
	#define	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE		1
#endif

#if		PK_HAS_SOFT
	float _InvSoftnessDistance;

	UNITY_DECLARE_SCREENSPACE_TEXTURE(_CameraDepthTexture)
	#define	LINEARIZE_DEPTH(x)		LinearEyeDepth(x)

	float	PKSampleDepthTexture(float2 uv)
	{
		float2		depthTexcoord;

		#if	UNITY_SINGLE_PASS_STEREO
			depthTexcoord = TransformStereoScreenSpaceTex(uv, 1.0);
		#else
			depthTexcoord = uv;
		#endif
		#if	CLIP_Y_REVERSED
			depthTexcoord.y = 1 - depthTexcoord.y;
		#endif
		float depth = UNITY_SAMPLE_SCREENSPACE_TEXTURE(_CameraDepthTexture, depthTexcoord).r;
		return depth;
	}

#endif

#include "VAT.cginc"

struct Input
{
	float2 uv_MainTex;
	float4 projPos;
	float4 color;
};

// Declare instanced properties inside a cbuffer.
// Each instanced property is an array of by default 500(D3D)/128(GL) elements. Since D3D and GL imposes a certain limitation
// of 64KB and 16KB respectively on the size of a cubffer, the default array size thus allows two matrix arrays in one cbuffer.
// Use maxcount option on #pragma instancing_options directive to specify array size other than default (divided by 4 when used
// for GL).
UNITY_INSTANCING_BUFFER_START(InstanceProperties)
	UNITY_DEFINE_INSTANCED_PROP(fixed4, _BaseColor)
#if !PK_HAS_EMISSIVE_NONE
	UNITY_DEFINE_INSTANCED_PROP(fixed4, _EmissiveColor)
#endif
#if PK_HAS_ALPHA_REMAP
	UNITY_DEFINE_INSTANCED_PROP(float, _AlphaCursor)
#endif
#if !PK_HAS_VAT_NONE
	UNITY_DEFINE_INSTANCED_PROP(float, _VATCursor)
#endif
UNITY_INSTANCING_BUFFER_END(InstanceProperties)

void vertMain(inout appdata_full v, out Input data)
{
	UNITY_INITIALIZE_OUTPUT(Input, data);

#if !PK_HAS_VAT_NONE
	float	vatCursor = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _VATCursor);
	float3	outPosition;
	float3	outNormal;
	float3	outTangent;
	float4	outColor;

	VAT_float(	v.texcoord.xy,
				v.texcoord1.xy,
				v.vertex.xyz,
				v.normal,
				v.tangent.xyz,
				v.color,
				vatCursor,
				outPosition,
				outNormal,
				outTangent,
				outColor);

	outColor *= UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);
#else
	float3	outPosition = v.vertex.xyz;
	float3	outNormal = v.normal;
	float3	outTangent = v.tangent.xyz;
	float4	outColor = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);
#endif

	v.vertex.xyz = outPosition;
	v.normal = outNormal;
	v.tangent.xyz = outTangent;
	data.color = outColor;
	data.projPos = UnityObjectToClipPos(outPosition);
}

void surf(Input IN, inout SURF_OUTPUT o)
{
	float2 uv = IN.uv_MainTex;
	fixed4 diffuse = tex2D(_MainTex, uv) * IN.color;

#if		PK_HAS_ALPHA_REMAP
	diffuse.a = tex2D(_AlphaMap, float2(diffuse.a, UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _AlphaCursor))).r;
#endif

#if		!PK_IS_TRANSPARENT
	clip(diffuse.a - _CutOutAlpha);
#endif

#if		PK_HAS_DIFFUSE_RAMP
	diffuse = tex2D(_DiffuseRampMap, float2(diffuse.r, 0));
#endif

#if PK_IS_LIT
	o.Albedo = diffuse.rgb;
#else
	o.Emission = diffuse.rgb;
#endif

	o.Alpha = diffuse.a;

#if		!PK_HAS_EMISSIVE_NONE
	fixed4 emissive = tex2D(_EmissiveMap, uv) * UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _EmissiveColor);
#if		PK_HAS_EMISSIVE_WITH_RAMP
	emissive = tex2D(_EmissiveRampMap, float2(emissive.r, 0));
#endif
	o.Emission += emissive.rgb;
#endif

#if		PK_HAS_SOFT
	float3	transformedPosition = IN.projPos.xyz / IN.projPos.w;

	#if	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE
		transformedPosition.z = transformedPosition.z * 0.5f + 0.5f;
	#endif

	float2	screenUV = transformedPosition.xy * 0.5f + 0.5f;

	float	sceneDepth = LINEARIZE_DEPTH(PKSampleDepthTexture(screenUV));
	float	fragDepth = LINEARIZE_DEPTH(transformedPosition.z);
	float	depthfade = saturate(_InvSoftnessDistance * (sceneDepth - fragDepth));
	o.Alpha *= depthfade;
#endif
#if		PK_IS_TRANSPARENT
	// We only support additive for transparent meshes so we need to mulitply the final color by the alpha
	o.Emission *= o.Alpha;
#endif

#if PK_IS_LIT
	o.Metallic = _Metalness;
	o.Smoothness = 1.0f - _Roughness;
	o.Normal = UnpackNormal(tex2D(_NormalMap, uv));
#endif

}


#if !PK_IS_LIT

fixed4 LightingNoLighting(SurfaceOutput s, fixed3 lightDir, fixed atten)
{
	fixed4 c;
	c.rgb = s.Albedo; 
	c.a = s.Alpha;
	return c;
}

#endif