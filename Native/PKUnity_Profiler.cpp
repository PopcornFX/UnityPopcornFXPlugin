//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#include "precompiled.h"
#include "PKUnity_Profiler.h"

#if	(PK_PARTICLES_HAS_STATS != 0)

#include <pk_kernel/include/kr_caps_cpu.h>
#include <pk_kernel/include/kr_streams_memory.h>
#include <pk_kernel/include/kr_profiler.h>
#include <pk_kernel/include/kr_profiler_details.h>
#include <pk_base_object/include/hbo_object.h>
#include <pk_kernel/include/kr_hash.h>
#include <pk_kernel/include/kr_sort.h>
#include <pk_particles/include/ps_effect.h>
#include <pk_particles/include/ps_mediums.h>
#include <pk_particles/include/ps_storage.h>
#include <pk_particles/include/ps_stats.h>

#include <RuntimeManager.h>
#include <ManagedToNative.h>


__PK_API_BEGIN


//----------------------------------------------------------------------------

CMultiReport::CMultiReport()
	: m_NextReport(0)
{
	SetReportHistoryCount(64);
}

//----------------------------------------------------------------------------

CMultiReport::~CMultiReport()
{
}

//----------------------------------------------------------------------------

void	CMultiReport::SetReportHistoryCount(u32 count)
{
	if (m_Reports.Count() < count)
	{
		m_Reports.Resize(count);
	}
	else if (m_Reports.Count() > count)
	{
		for (u32 i = count; i < m_Reports.Count(); ++i)
			_ClearReportByIndex(i);
		m_Reports.Resize(count);
	}
}

//----------------------------------------------------------------------------

void	CMultiReport::Clear()
{
	for (u32 i = 0; i < m_Reports.Count(); ++i)
		_ClearReportByIndex(i);
}

//----------------------------------------------------------------------------

void	CMultiReport::MergeReport(Profiler::CProfiler *profiler)
{
	const u32	reporti = m_NextReport;
	m_NextReport = (m_NextReport + 1) % m_Reports.Count();

	_ClearReportByIndex(reporti);

	profiler->BuildReport(&m_Reports[reporti].m_Report);
}

//----------------------------------------------------------------------------

void	CMultiReport::_ClearReportByIndex(u32 idx)
{
	SReportEntry		&report = m_Reports[idx];

	report.m_Report.Clear();
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//					CLiveProfiler
//----------------------------------------------------------------------------

CLiveProfiler::CLiveProfiler()
{
}

//----------------------------------------------------------------------------

CLiveProfiler::~CLiveProfiler()
{
	TArray<TPair<CString, SStatsReport*>>	data;
	m_StatsReports.PopulateArray(data);

	while (data.Count() != 0)
		PK_FREE(data.Pop().Value());
	m_StatsReports.Clear();

	if (m_AliveStatsMemory != null)
	{
		delete[] m_AliveStatsMemory;
		m_AliveStatsMemory = null;
	}
}

//----------------------------------------------------------------------------

void CLiveProfiler::StartFrame()
{
	++m_CurrentFrameNumber;
}

//----------------------------------------------------------------------------

void CLiveProfiler::EndFrame()
{
	if (m_CurrentFrameNumber >= m_NumberOfFrameToAverage)
		m_CurrentFrameNumber = 0;
}

//----------------------------------------------------------------------------

bool	operator==(const CLiveProfiler::SPopcornFXEffectTimings &other, const PopcornFX::CParticleEffect *effect)
{
	return effect == other.m_Effect;
}

//----------------------------------------------------------------------------

void	CLiveProfiler::AddReport(const char *reportName, CParticleMediumCollection *medCol, float updateTime)
{
	if (!m_StatsReports.Contains(reportName))
	{
		SStatsReport *report = PK_NEW(SStatsReport);
		PK_ASSERT(report != null);
		m_StatsReports.Insert(reportName, report);
	}
	SStatsReport	*activeReport = *m_StatsReports[reportName];

	if (activeReport == null)
		return;

	if (m_FrameStatsEnabled)
	{
		activeReport->m_CollectionUpdateTime += updateTime;
		activeReport->m_ParticleCount += medCol->Stats().m_TotalParticleCount_CPU.Load();
		activeReport->m_EffectsUsedCount += medCol->EffectList().UsedCount();

		if (m_CurrentFrameNumber >= m_NumberOfFrameToAverage)
		{
			activeReport->m_CollectionUpdateTimeAverage = activeReport->m_CollectionUpdateTime / m_CurrentFrameNumber;
			activeReport->m_ParticleCount = activeReport->m_ParticleCountAverage / m_CurrentFrameNumber;
			activeReport->m_EffectsUsedCountAverage = activeReport->m_EffectsUsedCount / m_CurrentFrameNumber;
		}
	}

	if (m_EffectsStatsEnabled)
	{
		for (u32 iMedium = 0; iMedium < medCol->Mediums().Count(); iMedium++)
		{
			const CParticleMedium	*medium = medCol->Mediums()[iMedium].Get();
			const CMediumStats		*mediumStats = medium->Stats();
			const CParticleEffect	*effect = medium->Descriptor()->ParentEffect();

			bool				newRegister = false;
			PopcornFX::CGuid	effectTimingsId = activeReport->m_EffectTimings.IndexOf(effect);
			if (!effectTimingsId.Valid())
			{
				newRegister = true;
				effectTimingsId = activeReport->m_EffectTimings.PushBack();
			}
			if (!PK_VERIFY(effectTimingsId.Valid()))
				break;
			SPopcornFXEffectTimings &effectTimings = activeReport->m_EffectTimings[effectTimingsId];

			if (newRegister)
			{
				effectTimings.m_Effect = effect;
				effectTimings.m_EffectPath = effect->File()->Path();
				effectTimings.m_InstanceCount = CRuntimeManager::Instance().GetInstanceCount(effect);// only compute that for the first medium of a given effect.
			}
			PopcornFX::SEvolveStatsReport	mediumStatsReport;
			mediumStats->ComputeGlobalStats(mediumStatsReport);

			effectTimings.m_TotalStatsReport += mediumStatsReport;
			if (PK_VERIFY(medium->ParticleStorage() != null) &&
				PK_VERIFY(medium->ParticleStorage()->Manager() != null))
			{
				effectTimings.m_TotalParticleCount_CPU += medium->ParticleStorage()->ActiveParticleCount();
				effectTimings.m_TotalTimeAverage += mediumStatsReport.m_PipelineStages[PopcornFX::SEvolveStatsReport::PipelineStage_Total].m_Time;
				activeReport->m_TotalTimeAverage += mediumStatsReport.m_PipelineStages[PopcornFX::SEvolveStatsReport::PipelineStage_Total].m_Time;
			}
		}
		if (m_CurrentFrameNumber >= m_NumberOfFrameToAverage)
		{
			float timeNormalizer = activeReport->m_CollectionUpdateTimeAverage / (activeReport->m_TotalTimeAverage / (float)m_CurrentFrameNumber);
			activeReport->m_EffectTimingsAverage = activeReport->m_EffectTimings;
			for (u32 i = 0; i < activeReport->m_EffectTimingsAverage.Count(); ++i)
			{
				activeReport->m_EffectTimingsAverage[i].m_TotalParticleCount_CPU = activeReport->m_EffectTimingsAverage[i].m_TotalParticleCount_CPU / m_CurrentFrameNumber;
				
				activeReport->m_EffectTimingsAverage[i].m_TotalTimeAverage = timeNormalizer * (activeReport->m_EffectTimingsAverage[i].m_TotalTimeAverage / (float)m_CurrentFrameNumber);
			}
			activeReport->m_EffectTimings.Clear();
			activeReport->m_Dirty = true;
		}
	}
	if (m_FrameStatsEnabled)
	{
		if (m_CurrentFrameNumber >= m_NumberOfFrameToAverage)
		{
			activeReport->m_CollectionUpdateTime = 0;
			activeReport->m_EffectsUsedCount = 0;
			activeReport->m_CollectionUpdateTimeAverage = 0;
		}
	}
}

bool	CLiveProfiler::FillFrameStats(const char *reportName, SStatsToFill &data)
{
	if (m_StatsReports.Contains(reportName))
	{
		SStatsReport *activeReport = *m_StatsReports[reportName];
		if (activeReport == null)
			return false;
		if (!activeReport->m_Dirty)
			return false;

		activeReport->m_Dirty = false;

		if (m_AliveStatsMemory != null)
		{
			delete[] m_AliveStatsMemory;
			m_AliveStatsMemory = null;
		}

		if (m_FrameStatsEnabled)
		{
			data.m_CollectionUpdateTimeAverage = activeReport->m_CollectionUpdateTimeAverage;
			data.m_EffectsUsedCountAverage = activeReport->m_EffectsUsedCountAverage;
			data.m_ParticleCountAverage = activeReport->m_ParticleCountAverage;
		}
		if (m_EffectsStatsEnabled)
		{
			data.m_EffectsStatsCount = activeReport->m_EffectTimingsAverage.Count();
		
			SEffectStatsToFill	*effectStats = new SEffectStatsToFill[data.m_EffectsStatsCount];
			CString				effectNames;
			for (u32 i = 0; i < data.m_EffectsStatsCount; ++i)
			{
				effectStats[i].m_InstanceCount = activeReport->m_EffectTimingsAverage[i].TotalInstanceCount();
				effectStats[i].m_TotalParticleCount = activeReport->m_EffectTimingsAverage[i].TotalParticleCount();

				const PopcornFX::Units::SValueAndNamedUnit	readableTime = PopcornFX::Units::AutoscaleTime(activeReport->m_EffectTimingsAverage[i].m_TotalTimeAverage, 0.5f);
				effectStats[i].m_TotalTimeAverageRaw = activeReport->m_EffectTimingsAverage[i].m_TotalTimeAverage;
				effectStats[i].m_TotalTimeAverage = readableTime.m_Value;
				effectStats[i].m_Unit = readableTime.m_UnitName;
				effectNames += activeReport->m_EffectTimingsAverage[i].m_EffectPath + " ";
			}
			data.m_EffectsStats = effectStats;
			m_AliveNamesMemory = effectNames;
			data.m_EffectNames = m_AliveNamesMemory.Data();
			m_AliveStatsMemory = effectStats;
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

__PK_API_END

#endif
