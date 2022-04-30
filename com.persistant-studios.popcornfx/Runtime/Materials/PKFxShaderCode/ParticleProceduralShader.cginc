//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#if USE_HDRP

	#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/ShaderPass.cs.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl"
	#include "Packages/com.unity.render-pipelines.high-definition/Runtime/Material/Material.hlsl"
#elif	USE_URP
	//#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl"
	//
	//#include "Packages/com.unity.render-pipelines.lightweight/ShaderLibrary/MetaInput.hlsl"
	//#include "Packages/com.unity.render-pipelines.lightweight/ShaderLibrary/Core.hlsl"
	//#include "Packages/com.unity.render-pipelines.lightweight/ShaderLibrary/Lighting.hlsl"
	//#include "Packages/com.unity.render-pipelines.lightweight/ShaderLibrary/ShaderGraphFunctions.hlsl"
	//#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Color.hlsl"
	//#include "Packages/com.unity.render-pipelines.lightweight/Shaders/UnlitInput.hlsl"
	//#include "Packages/com.unity.render-pipelines.lightweight/Shaders/UnlitMetaPass.hlsl"
	
	#include "UnityCG.cginc"
#else
	#include "UnityCG.cginc"
#endif

// No Vertex inputs
#include "VertexProceduralInput.cginc"

// Vertex outputs:
#include "VertexOutput.cginc"

//------------------------------------------------------------------------------------
// Vertex shader
//------------------------------------------------------------------------------------
#include "VertexProceduralShader.cginc"

//------------------------------------------------------------------------------------
// Fragment shader
//------------------------------------------------------------------------------------
#include "FragmentShader.cginc"

