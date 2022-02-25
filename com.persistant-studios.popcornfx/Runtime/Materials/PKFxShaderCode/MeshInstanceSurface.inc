
sampler2D _MainTex;

struct Input
{
	float2 uv_MainTex;
};

#if		!PK_IS_TRANSPARENT
half _Glossiness;
half _Metallic;
#endif

// Declare instanced properties inside a cbuffer.
// Each instanced property is an array of by default 500(D3D)/128(GL) elements. Since D3D and GL imposes a certain limitation
// of 64KB and 16KB respectively on the size of a cubffer, the default array size thus allows two matrix arrays in one cbuffer.
// Use maxcount option on #pragma instancing_options directive to specify array size other than default (divided by 4 when used
// for GL).
UNITY_INSTANCING_BUFFER_START(InstanceProperties)
UNITY_DEFINE_INSTANCED_PROP(fixed4, _BaseColor)	// Make _BaseColor an instanced property (i.e. an array)
UNITY_INSTANCING_BUFFER_END(InstanceProperties)

#if		!PK_IS_TRANSPARENT
void surf(Input IN, inout SurfaceOutputStandard o)
{
	// Albedo comes from a texture tinted by color
	float2 uv = IN.uv_MainTex;

	fixed4 c = tex2D(_MainTex, uv) * UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);

	// Metallic and smoothness come from slider variables
	o.Metallic = _Metallic;
	o.Smoothness = _Glossiness;

	o.Albedo = c.rgb;
	o.Alpha = c.a;
}

#else

	void surf (Input IN, inout SurfaceOutput o)
	{
		float2 uv = IN.uv_MainTex;

		fixed4 c = tex2D(_MainTex, uv) * UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);

		o.Emission = c;
	}

	fixed4 LightingNoLighting(SurfaceOutput s, fixed3 lightDir, fixed atten)
	{
		return fixed4(0,0,0,0);
	}

#endif
