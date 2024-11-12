using UnityEngine;
using UnityEngine.UI; // 引入 UI 模块，用于 Slider
using TMPro; // 引入 TextMeshPro 模块

public class SliderValueDisplay : MonoBehaviour
{
    public Slider slider;           // 引用 Slider 组件
    public TMP_Text valueText;      // 引用 TextMeshPro 的 Text 组件

    void Start()
    {
        // 初始化时显示当前 Slider 的值
        UpdateSliderValue(slider.value);

        // 监听 Slider 值的变化
        slider.onValueChanged.AddListener(UpdateSliderValue);
    }

    // 更新 Slider 的值到 Text 中
    private void UpdateSliderValue(float value)
    {
        // 更新 Text 显示值
        valueText.text = value.ToString("0"); // 格式化显示为整数，你可以改为"0.0"显示小数
    }
}