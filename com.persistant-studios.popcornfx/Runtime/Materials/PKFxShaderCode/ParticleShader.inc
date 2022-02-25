//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#if		USE_HDRP

	#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/ShaderPass.cs.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/Material/Material.hlsl"
#elif	USE_URP
	#include "UnityCG.cginc"
#else
	#include "UnityCG.cginc"
#endif

// Vertex inputs:
#include "VertexInput.inc"

// Vertex outputs:
#include "VertexOutput.inc"

//------------------------------------------------------------------------------------
// Vertex shader
//------------------------------------------------------------------------------------
#include "VertexShader.inc"

//------------------------------------------------------------------------------------
// Fragment shader
//------------------------------------------------------------------------------------
#include "FragmentShader.inc"

