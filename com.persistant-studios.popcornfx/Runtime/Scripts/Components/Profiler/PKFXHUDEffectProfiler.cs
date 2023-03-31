using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using System.IO;
using UnityEngine.UI;

namespace PopcornFX
{
	public class PKFXHUDEffectProfiler : MonoBehaviour
	{
		public enum StatSortType : int
		{
			Instance = 0,
			Particle,
			Time,
		}

		public struct SEffectStats
		{
			public int m_InstanceCount;
			public int m_TotalParticleCount;
			public double m_TotalTimeAverage;
			public double m_TotalTimeAverageRaw;
			public string m_Unit;
			public string m_EffectName;

			public SEffectStats(SEffectStatsToFill other, string name)
			{
				m_InstanceCount = other.m_InstanceCount;
				m_TotalParticleCount = other.m_TotalParticleCount;
				m_TotalTimeAverage = other.m_TotalTimeAverage;
				m_TotalTimeAverageRaw = other.m_TotalTimeAverageRaw;
				m_Unit = Marshal.PtrToStringAnsi(other.m_Unit);
				m_EffectName = name;

			}
		};

		public StatSortType m_SortByType = StatSortType.Instance;

		public GameObject	m_ListContainerGO;
		public GameObject	m_ToggleButtonGO;
		public GameObject	m_ToggleButtonTextGO;
		public GameObject	m_EffectEntryPrefab;
		private Text		m_ToggleButtonText;

		private const int m_MaxResult = 25;

		private const string m_Text_StopProfiler = "Disable Profiler";
		private const string m_Text_StartProfiler = "Enable Profiler";

		private bool m_ProfilerState = false;
		private List<GameObject> m_ListObjects = new List<GameObject>();

		List<SEffectStats>		m_EffectData = new List<SEffectStats>();

		void Start()
		{
			m_ToggleButtonText = m_ToggleButtonTextGO.GetComponent<Text>();

			for (int i = 0; i < m_MaxResult; ++i)
			{
				GameObject result = Instantiate(m_EffectEntryPrefab);
				result.transform.SetParent(m_ListContainerGO.transform);
				m_ListObjects.Add(result);
			}
		}

		string	_GetSpacing(int desiredSpacing, int offset)
		{
			desiredSpacing -= (offset);

			return new string(' ', desiredSpacing);
		}

		void Update()
		{
			if (m_ProfilerState)
			{
				SStatsToFill data = new SStatsToFill();

				bool result = PKFxManager.InteractiveProfilerPullData("Particles", ref data);
				//PKFxManager.InteractiveProfilerPullData("Meshes", ref data);

				int count = 0;
				if (result)
				{
					List<SEffectStats> effectData = new List<SEffectStats>();
					if (data.m_EffectsStatsCount != 0 && data.m_EffectsStats != null && data.m_EffectNames != null)
					{
						string[] effectNames = Marshal.PtrToStringAnsi(data.m_EffectNames).Split(' ');
						unsafe
						{
							SEffectStatsToFill* effectStats = (SEffectStatsToFill*)data.m_EffectsStats.ToPointer();

							for (int i = 0; i < data.m_EffectsStatsCount; ++i)
							{
								effectData.Add(new SEffectStats(effectStats[i], Path.GetFileName(effectNames[i])));
							}
						}
						//To do sort EffectData;
						if (m_SortByType == StatSortType.Instance)
							effectData.Sort(delegate (SEffectStats x, SEffectStats y) { return y.m_InstanceCount - x.m_InstanceCount; });
						else if (m_SortByType == StatSortType.Particle)
							effectData.Sort(delegate (SEffectStats x, SEffectStats y) { return y.m_TotalParticleCount - x.m_TotalParticleCount; });
						else if (m_SortByType == StatSortType.Time)
							effectData.Sort(delegate (SEffectStats x, SEffectStats y)
							{
								if (y.m_TotalTimeAverageRaw > x.m_TotalTimeAverageRaw)
									return 1;
								return -1;
							});
					}
					m_EffectData = effectData;
				}
				while (count < m_EffectData.Count && count < m_MaxResult)
				{
					string effectName = m_EffectData[count].m_EffectName;
					Text[] texts = m_ListObjects[count].GetComponentsInChildren<Text>();
					int nameLen = effectName.Length;
					texts[0].text = effectName;
					texts[1].text = m_EffectData[count].m_InstanceCount.ToString() + _GetSpacing(nameLen, m_EffectData[count].m_InstanceCount.ToString().Length);
					texts[2].text = m_EffectData[count].m_TotalParticleCount.ToString() +_GetSpacing(nameLen, m_EffectData[count].m_TotalParticleCount.ToString().Length);
					texts[3].text = m_EffectData[count].m_TotalTimeAverage.ToString("F2") + m_EffectData[count].m_Unit +_GetSpacing(nameLen, m_EffectData[count].m_TotalTimeAverage.ToString("F2").Length + 2);
					++count;

				}
				while (count < m_MaxResult)
				{
					Text[] texts = m_ListObjects[count].GetComponentsInChildren<Text>();
					texts[0].text = "";
					texts[1].text = "";
					texts[2].text = "";
					texts[3].text = "";
					++count;
				}
			}
		}

		public void ToggleProfiler()
		{
			m_ProfilerState = !m_ProfilerState;

			PKFxManager.InteractiveProfilerEnable(m_ProfilerState);

			if (m_ProfilerState)
			{
				m_ToggleButtonText.text = m_Text_StopProfiler;
			}
			else
			{
				int count = 0;
				while (count < m_MaxResult)
				{
					Text[] texts = m_ListObjects[count].GetComponentsInChildren<Text>();
					texts[0].text = "";
					texts[1].text = "";
					texts[2].text = "";
					texts[3].text = "";
					++count;
				}
				m_ToggleButtonText.text = m_Text_StartProfiler;
			}
		}

		public void SetSortByType(int type)
		{
			m_SortByType = (StatSortType)type;
		}
	}

}