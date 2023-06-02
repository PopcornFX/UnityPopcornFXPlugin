using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;

namespace PopcornFX
{
	public class PKFXHUDEffectProfiler : MonoBehaviour
	{
		public enum StatSortType : int
		{
			Emitter = 0,
			Particle,
			Time,
		}

		public struct SEffectStats
		{
			public uint		m_EmitterCount;
			public uint		m_TotalParticleCount;
			public float	m_TotalTimeAverage;
			public float	m_TotalTimeAverageRaw;
			public string	m_Unit;
			public string	m_EffectName;

			public SEffectStats(SEffectStatsToFill other, string name)
			{
				m_EmitterCount = (uint)other.m_EmitterCount;
				m_TotalParticleCount = (uint)other.m_TotalParticleCount;
				m_TotalTimeAverage = (float)other.m_TotalTimeAverage;
				m_TotalTimeAverageRaw = (float)other.m_TotalTimeAverageRaw;
				m_Unit = Marshal.PtrToStringAnsi(other.m_Unit);
				m_EffectName = name;

			}
		};

		public StatSortType				m_SortByType = StatSortType.Emitter;

		public GameObject				m_ListContainer;
		public GameObject				m_ToggleButton;
		public GameObject				m_EffectEntryPrefab;

		public Image					m_ImageGlobalCount;
		public Image					m_ImageGlobalTime;

		// Buttons Text Fields
		public Text						m_TextComponent_SortButtonText;
		public Text						m_TextComponent_PauseButtonText;
		public Text						m_TextComponent_HideButtonText;

		public Text						m_TextComponent_SortField;

		// Global Metrics Indicators
		public Text						m_TextComponent_GlobalUpdateTime;
		public Text						m_TextComponent_GlobalParticleCount;
		public Text						m_TextComponent_TotalEmitters;

		private Text					m_ToggleButtonText;
		private RectTransform			m_RectTransform;

		private const uint				m_MaxResult = 25;

		private bool					m_ProfilerRunning = true;
		private bool					m_ProfilerShow = true;
		private List<PKFxProfilerEntry>	m_ListEntries = new List<PKFxProfilerEntry>();

		private uint					m_TotalParticles = 0;
		private uint					m_TotalEmitters = 0;
		private float					m_TotalCalcutationTimeInSeconds = 0.0f;

		List<SEffectStats>				m_EffectData = new List<SEffectStats>();

		void Start()
		{
			m_RectTransform = this.GetComponent<RectTransform>();

			for (int i = 0; i < m_MaxResult; ++i)
			{
				PKFxProfilerEntry	result = Instantiate(m_EffectEntryPrefab).GetComponent<PKFxProfilerEntry>();
				result.transform.SetParent(m_ListContainer.transform);
				result.transform.localScale = Vector3.one;
				result.gameObject.SetActive(false);
				m_ListEntries.Add(result);
			}

			SetShortcuts();
		}

		void Update()
		{
			if (PKFxSettings.ProfilerHideKey != KeyCode.None && Input.GetKeyDown(PKFxSettings.ProfilerHideKey))
			{
				ToggleVisibility();
			}
			if (!m_ProfilerShow)
				return;

			if (PKFxSettings.ProfilerPauseKey != KeyCode.None && Input.GetKeyDown(PKFxSettings.ProfilerPauseKey))
			{
				ToggleProfiler();
			}
			if (PKFxSettings.ProfilerSortKey != KeyCode.None && Input.GetKeyDown(PKFxSettings.ProfilerSortKey))
			{
				ToggleSort();
			}

			SetShortcuts();

			if (m_ProfilerRunning)
			{
				SStatsToFill	particleData = new SStatsToFill();
				SStatsToFill	meshData = new SStatsToFill();

				// Resetting counter
				m_TotalParticles = 0;
				m_TotalCalcutationTimeInSeconds = 0;

				bool	result = PKFxManager.InteractiveProfilerPullData("Particles", ref particleData);
				List<SEffectStats> effectData = new List<SEffectStats>();
				if (!result)
					return;
				if (particleData.m_EffectsStatsCount > 0 && particleData.m_EffectsStats != null && particleData.m_EffectNames != null)
				{
					string[]	effectNames = Marshal.PtrToStringAnsi(particleData.m_EffectNames).Split(' ');
					unsafe
					{
						SEffectStatsToFill*	effectStats = (SEffectStatsToFill*)particleData.m_EffectsStats.ToPointer();

						for (int i = 0; i < particleData.m_EffectsStatsCount; ++i)
						{
							SEffectStats stats = new SEffectStats(effectStats[i], Path.GetFileName(effectNames[i]));

							m_TotalParticles += stats.m_TotalParticleCount;
							m_TotalCalcutationTimeInSeconds += (float)stats.m_TotalTimeAverageRaw; // TODO : remove once m_CollectionUpdateTimeInterval is fixed
							
							effectData.Add(stats);
						}
					}
				}

				result = PKFxManager.InteractiveProfilerPullData("Meshes", ref meshData);
				if (!result)
					return;
				if (meshData.m_EffectsStatsCount > 0 && meshData.m_EffectsStats != null && meshData.m_EffectNames != null)
				{
					string[] effectNames = Marshal.PtrToStringAnsi(meshData.m_EffectNames).Split(' ');
					unsafe
					{
						SEffectStatsToFill* effectStats = (SEffectStatsToFill*)meshData.m_EffectsStats.ToPointer();
				
						for (int i = 0; i < meshData.m_EffectsStatsCount; ++i)
						{
							SEffectStats stats = new SEffectStats(effectStats[i], Path.GetFileName(effectNames[i]));

							m_TotalParticles += stats.m_TotalParticleCount;
							m_TotalCalcutationTimeInSeconds += (float)stats.m_TotalTimeAverageRaw; // TODO : remove once m_CollectionUpdateTimeInterval is fixed

							effectData.Add(stats);
						}
					}
				}

				if (m_SortByType == StatSortType.Emitter)
					effectData.Sort(delegate (SEffectStats x, SEffectStats y) { return (int)(y.m_EmitterCount - x.m_EmitterCount); });
				else if (m_SortByType == StatSortType.Particle)
					effectData.Sort(delegate (SEffectStats x, SEffectStats y) { return (int)(y.m_TotalParticleCount - x.m_TotalParticleCount); });
				else if (m_SortByType == StatSortType.Time)
				{
					effectData.Sort(delegate (SEffectStats x, SEffectStats y)
					{
						if (y.m_TotalTimeAverageRaw > x.m_TotalTimeAverageRaw)
							return 1;
						return -1;
					});
				}
				m_EffectData = effectData;

				// TODO : This is broken but should work once m_CollectionUpdateTimeInterval is fixed
				#if false
					m_TotalCalcutationTime = particleData.m_CollectionUpdateTimeAverage + meshData.m_CollectionUpdateTimeAverage;
				#endif

				m_TotalEmitters = (uint)(particleData.m_EffectsUsedCountAverage + meshData.m_EffectsUsedCountAverage);

				// Individual FXs
				int	count = 0;
				while (count < m_EffectData.Count && count < m_MaxResult)
				{
					m_ListEntries[count].m_NameText.text = m_EffectData[count].m_EffectName;
					m_ListEntries[count].m_CountText.text = m_EffectData[count].m_TotalParticleCount.ToString();
					m_ListEntries[count].m_EmitterText.text = m_EffectData[count].m_EmitterCount.ToString();
					m_ListEntries[count].m_TimeText.text = m_EffectData[count].m_TotalTimeAverage.ToString("F2").Replace(',', '.') + " " + m_EffectData[count].m_Unit;
					m_ListEntries[count].m_TimeBarImage.color = Color.Lerp(PKFxSettings.ProfilerColorBudgetNormal, PKFxSettings.ProfilerColorBudgetExceeded, (float)(m_EffectData[count].m_TotalTimeAverageRaw / PKFxSettings.ProfilerPerEffectBudgetInSeconds));
					m_ListEntries[count].m_CountImage.color = Color.Lerp(PKFxSettings.ProfilerColorBudgetNormal, PKFxSettings.ProfilerColorBudgetExceeded, (m_EffectData[count].m_TotalParticleCount / (float)PKFxSettings.ProfilerPerEffectParticleBudget));
					m_ListEntries[count].m_BarRect.sizeDelta = new Vector2((float)(100 * (m_EffectData[count].m_TotalTimeAverageRaw / m_TotalCalcutationTimeInSeconds)), m_ListEntries[count].m_BarRect.sizeDelta.y); // 100 is the width of the bar in UI units

					m_ListEntries[count].gameObject.SetActive(true);
					++count;
				}
				while (count < m_MaxResult)
				{
					m_ListEntries[count].gameObject.SetActive(false); 
					++count;
				}
				LayoutRebuilder.MarkLayoutForRebuild(m_RectTransform);
			}

			// Setting global Stats
			m_TextComponent_GlobalParticleCount.text = m_TotalParticles.ToString();
			m_TextComponent_TotalEmitters.text = m_TotalEmitters.ToString();
			m_ImageGlobalCount.color = Color.Lerp(PKFxSettings.ProfilerColorBudgetNormal, PKFxSettings.ProfilerColorBudgetExceeded, (m_TotalParticles / (float)PKFxSettings.ProfilerSceneParticleBudget));
			m_ImageGlobalTime.color = Color.Lerp(PKFxSettings.ProfilerColorBudgetNormal, PKFxSettings.ProfilerColorBudgetExceeded, ((float)m_TotalCalcutationTimeInSeconds / (float)PKFxSettings.ProfilerSceneBudgetInSeconds));
			m_TextComponent_GlobalUpdateTime.text = ToStringClosestScale(m_TotalCalcutationTimeInSeconds);
		}

		// Unit should ideally be provided with the globaltime, but in the meantime this solution works (note that the behavior is not consistent with the timer of effects, which goes up at 500/0.5u)
		public string ToStringClosestScale(float calculationTime)
		{
			// milli : 10e-3
			if (calculationTime > 1e-4)
				return ((calculationTime / 1e-3).ToString("F2").Replace(',', '.') + " ms");
			// micro : 10e-6
			if (calculationTime > 1e-7)
				return ((calculationTime / 1e-6).ToString("F2").Replace(',', '.') + " µs");
			// nano : 10e-9
			if (calculationTime > 1e-10)
				return ((calculationTime / 1e-9).ToString("F2").Replace(',', '.') + " ns");
			return "";
		}

		public void ToggleVisibility()
		{
			CanvasGroup canvasGroup = gameObject.GetComponent<CanvasGroup>();
			m_ProfilerShow = !m_ProfilerShow;
			canvasGroup.alpha = m_ProfilerShow ? 1 : 0;
			canvasGroup.interactable = m_ProfilerShow;
			canvasGroup.blocksRaycasts = m_ProfilerShow;
			if (!m_ProfilerRunning)
				ToggleProfiler();
		}

		public void ToggleProfiler()
		{
			m_ProfilerRunning = !m_ProfilerRunning;

			PKFxManager.InteractiveProfilerEnable(m_ProfilerRunning);
		}
		public void ToggleSort()
		{
			SetSortByType(((int)m_SortByType + 1) % (int)(StatSortType.Time + 1));
		}

		public void SetSortByType(int type)
		{
			m_SortByType = (StatSortType)type;
			m_TextComponent_SortField.text = m_SortByType.ToString();
		}

		public void SetShortcuts()
		{
			m_TextComponent_PauseButtonText.text = (!m_ProfilerRunning ? "Unpause" : "Pause") + " (" + PKFxSettings.ProfilerPauseKey.ToString().Replace(',', '.') +")";
			m_TextComponent_HideButtonText.text = "Hide (" + PKFxSettings.ProfilerHideKey.ToString() + ")";
			m_TextComponent_SortButtonText.text = "Sort by : (" + PKFxSettings.ProfilerSortKey + ")";
		}
	}
}