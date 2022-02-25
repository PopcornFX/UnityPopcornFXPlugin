#include "InstancingBufferDecl.cginc"
void getInstanceColor_float(out float4 color)
{
	color = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _BaseColor);
}