using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PKFxProfilerEntry : MonoBehaviour
{
    // This class serves to keep the following editor-set references for PKFXHUDEffectProfiler.cs to use

    public RectTransform    m_BarRect;
    public Image            m_TimeBarImage;
    public Image            m_CountImage;
    public Text             m_EmitterText;
    public Text             m_CountText;
    public Text             m_TimeText;
    public Text             m_NameText;
}
