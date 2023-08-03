//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "pk_particles/include/ps_config.h"

#if	(PK_PARTICLES_HAS_STATS != 0)

#include <pk_kernel/include/kr_profiler.h>
#include <pk_kernel/include/kr_profiler_details.h>
#include <pk_particles/include/ps_stats.h>

#include "ManagedToNative.h"

__PK_API_BEGIN

//----------------------------------------------------------------------------

class	CMultiReport
{
public:
	struct SReportEntry
	{
		Profiler::CProfilerReport	m_Report;
	};

	CMultiReport();
	~CMultiReport();

	void	SetReportHistoryCount(u32 count);
	void	Clear();
	void	MergeReport(Profiler::CProfiler *profiler);

private:
	void	_ClearReportByIndex(u32 idx);

	TArray<SReportEntry>	m_Reports;
	u32						m_NextReport;
};

//----------------------------------------------------------------------------

class CLiveProfiler
{
public:
	struct	SPopcornFXEffectTimings
	{
		const CParticleEffect	*m_Effect = null; // Use the effect pointer as a compare value only
		CString					m_EffectPath;
		SEvolveStatsReport		m_TotalStatsReport;
		u32						m_TotalParticleCount_CPU = 0;
		u32						m_InstanceCount = 0;

		float					m_TotalTimeAverage = 0.0f;

		float			TotalTime() const { return m_TotalStatsReport.m_PipelineStages[PopcornFX::SEvolveStatsReport::PipelineStage_Total].m_Time; }
		u32				TotalParticleCount_CPU() const { return m_TotalParticleCount_CPU; }
		u32				TotalParticleCount() const { return m_TotalParticleCount_CPU; }
		u32				TotalInstanceCount() const { return m_InstanceCount; }
	};

	struct SStatsReport
	{
		float							m_CollectionUpdateTime = 0.0f;
		float							m_CollectionUpdateTimeAverage = 0.0f;

		float							m_TotalTimeAverage = 0.0f;

		u32								m_ParticleCount = 0;
		u32								m_ParticleCountAverage = 0;
		u32								m_EffectsUsedCount = 0;
		u32								m_EffectsUsedCountAverage = 0;

		TArray<SPopcornFXEffectTimings>	m_EffectTimings;
		TArray<SPopcornFXEffectTimings>	m_EffectTimingsAverage;

		bool							m_Dirty = false;
	};

public:
	CLiveProfiler();
	~CLiveProfiler();

	void			StartFrame();
	void			EndFrame();
	void			AddReport(const char *reportName, CParticleMediumCollection *medCol, float updateTime);
	bool			FillFrameStats(const char *reportName, SStatsToFill &data);

	void			EnableFrameStats(bool enable) { m_FrameStatsEnabled = enable; }
	void			EnableEffectsStats(bool enable) { m_EffectsStatsEnabled = enable; }

private:
	//stats

	u32									m_CurrentFrameNumber = 0;
	u32									m_NumberOfFrameToAverage = 16;

	 bool								m_FrameStatsEnabled = true;
	 bool								m_EffectsStatsEnabled = true;

	 THashMap<SStatsReport*, CString>	m_StatsReports;

	 SEffectStatsToFill					*m_AliveStatsMemory = null;
	 CString							m_AliveNamesMemory;

};

//----------------------------------------------------------------------------

__PK_API_END

#endif
