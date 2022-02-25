#pragma once

//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PKFX_RENDERER_SUB_VIEW_H__
#define __PKFX_RENDERER_SUB_VIEW_H__

// For the time being, CRendererSubView must be defined in the PopcornFX namespace:

//#define	ENABLE_RENDERBUFFER_TIMINGS

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CRendererSubView
{
public:
	enum	ERenderPass
	{
		RenderPass_Main,
		RenderPass_Shadow,
		RenderPass_Distortion,

		__MaxRenderPasses
	};

private:
	CFloat4						m_ZBufferParams;
	CFloat4x4					m_ViewMatrix;
	CFloat4x4					m_ViewMatrixGlobalScaled;
	CFloat4x4					m_ProjMatrix;
	CFloat4x4					m_BillBoardingMatrix;
	CFloat4x4					m_ViewProjMatrix;
	CFloat4x4					m_ViewProjMatrixGlobalScaled;
	CParticleMediumCollection	*m_ParticleMediumCollection;
	void						*m_DepthRT;
	u32							m_DepthRTBpp;
	float						m_LODBias;
	bool						m_UseLODBias;

protected:
	float						m_GlobalScale;
	ERenderPass					m_RenderPass;

public:
	CRendererSubView()
	:	m_ZBufferParams(CFloat4::ZERO)
	,	m_ViewMatrix(CFloat4x4::IDENTITY)
	,	m_BillBoardingMatrix(CFloat4x4::IDENTITY)
	,	m_ViewProjMatrix(CFloat4x4::IDENTITY)
	,	m_ParticleMediumCollection(null)
	,	m_DepthRT(null)
	,	m_DepthRTBpp(0)
	,	m_LODBias(0)
	,	m_UseLODBias(false)
	,	m_GlobalScale(1.0f)
	,	m_RenderPass(RenderPass_Main)
	{
	}

	~CRendererSubView()
	{
	}

	PK_FORCEINLINE float						GlobalScale() const { return m_GlobalScale; }
	PK_FORCEINLINE void							SetGlobalScale(float globalScale) { m_GlobalScale = globalScale; }

	PK_FORCEINLINE const CFloat4x4				&ViewMatrix() const { return m_ViewMatrix; }
	PK_FORCEINLINE const CFloat4x4				&ViewMatrixGlobalScaled() const { return m_ViewMatrixGlobalScaled; }
	PK_FORCEINLINE const CFloat4x4				&BillBoardingMatrix() const { return m_BillBoardingMatrix; }
	PK_FORCEINLINE const CFloat4x4				&ViewProjMatrix() const { return m_ViewProjMatrix; }
	PK_FORCEINLINE const CFloat4x4				&ViewProjMatrixGlobalScaled() const { return m_ViewProjMatrixGlobalScaled; }
	PK_FORCEINLINE void							SetMatrix(const CFloat4x4 &view, const CFloat4x4 &bMat, const CFloat4x4 &viewProj)
	{
		m_ViewMatrix = view;
		m_ProjMatrix = view.Inverse() * viewProj; // extract view to projection matrix
		m_BillBoardingMatrix = bMat;
		m_ViewProjMatrix = viewProj;
		m_ViewMatrixGlobalScaled = view;
		m_ViewMatrixGlobalScaled.StrippedXAxis() *= m_GlobalScale;
		m_ViewMatrixGlobalScaled.StrippedYAxis() *= m_GlobalScale;
		m_ViewMatrixGlobalScaled.StrippedZAxis() *= m_GlobalScale;
		m_ViewProjMatrixGlobalScaled = m_ViewMatrixGlobalScaled * m_ProjMatrix;

		//m_ViewProjMatrixGlobalScaled = (m_ViewMatrix * scaleMat) * m_ProjMatrix;
	}

	PK_FORCEINLINE const CFloat4				&ZBufferParams() const { return m_ZBufferParams; }
	PK_FORCEINLINE const void					*DepthRT() const { return m_DepthRT; }
	PK_FORCEINLINE u32							DepthRTBpp() const { return m_DepthRTBpp; }
	PK_FORCEINLINE void							SetDepthInfos(const CFloat4 &zbufferParams, void *depthRT, u32 depthRTBpp = 16)
	{
		m_ZBufferParams = zbufferParams;
		m_DepthRT = depthRT;
		m_DepthRTBpp = depthRTBpp;
	}
	PK_FORCEINLINE bool							UseLODBias() const { return m_UseLODBias; }
	PK_FORCEINLINE float						LODBias() const { PK_ASSERT(m_UseLODBias); return m_LODBias; }
	PK_FORCEINLINE void							SetLODBias(float bias, bool useLODBias = true)
	{
		m_LODBias = bias;
		m_UseLODBias = useLODBias;
	}

	PK_FORCEINLINE CParticleMediumCollection	*ParticleMediumCollection() const { return m_ParticleMediumCollection; }
//	PK_FORCEINLINE FxScenePtr					PKFXScene() const { return m_PKFXScene; }
//	PK_FORCEINLINE void							SetPKFXScene(FxScenePtr scene) { m_PKFXScene = scene; }
	PK_FORCEINLINE void							SetParticleMediumCollection(CParticleMediumCollection *collection) { m_ParticleMediumCollection = collection; }

	PK_FORCEINLINE ERenderPass					RenderPass() const { return m_RenderPass; }
	PK_FORCEINLINE void							SetRenderPass(ERenderPass renderPass) { m_RenderPass = renderPass; }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PKFX_RENDERER_SUB_VIEW_H__
