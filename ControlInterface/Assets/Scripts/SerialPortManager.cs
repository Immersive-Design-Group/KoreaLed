using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Text;
using UnityEngine.UI;
using TMPro;

public class SerialPortManager : MonoBehaviour
{
    // 串口对象
    private SerialPort serialPort;

    // 串口参数
    public string portName = "COM3";
    public int baudRate = 9600;

    public Slider[] sliders = new Slider[10];

    public TMP_Text stateText;

    public TMP_InputField portNameInput;
    public TMP_InputField baudRateInput;
    public TMP_InputField xStepInput;
    public TMP_InputField xSpanInput;
    public TMP_InputField xTimesInput;
    public TMP_InputField yStepInput;

    private Color originalTextColor = Color.black; // 原始文本颜色
    private Color changedTextColor = new Color(0.8f, 0, 0); // 深红色

    public Toggle motor1Toggle;
    public Toggle motor2Toggle;
    public Toggle motor3Toggle;
    public Toggle motor4Toggle;
    public Toggle motor5Toggle;

    public int x_step = 1;
    public float x_span = 0.5f; // 间隔时间，单位为秒
    public int x_times = 3; // 发送次数

    public int y_step = 1;
    public float y_span = 0.5f; // 间隔时间，单位为秒
    public int y_times = 3; // 发送次数

    // 模拟模式标志
    public bool simulateMode = true; // 如果没有串口设备，将其设置为 true

    void Start()
    {
        if (!simulateMode)
        {
            // 初始化串口
            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 500;
            serialPort.WriteTimeout = 500;

            try
            {
                serialPort.Open();
                Debug.Log("串口已打开：" + portName);
                stateText.text = "Serial port is open: " + portName;
            }
            catch (System.Exception e)
            {
                Debug.LogError("Serial port open failed: " + e.Message);
                stateText.text = "Serial port open failed: " + e.Message;
            }
        }
        else
        {
            Debug.Log("模拟模式开启，跳过串口初始化");
        }

    }

    // 通用的 InputField 内容改变处理方法
    public void OnInputFieldChanged(TMP_InputField inputField)
    {
        inputField.textComponent.color = changedTextColor;
    }
    public void UpdatePortName()
    {
        portName = portNameInput.text;
        Debug.Log("串口名称已更新为：" + portName);
    

        baudRate = int.Parse(baudRateInput.text);
        Debug.Log("波特率已更新为：" + baudRate);
        // stateText.text = "Port name: " + portName + ", Baud rate: " + baudRate;

        x_step = int.Parse(xStepInput.text);
        Debug.Log("x_step 的值: " + x_step);

        x_span = float.Parse(xSpanInput.text);
        Debug.Log("x_span 的值: " + x_span);

        x_times = int.Parse(xTimesInput.text);
        Debug.Log("x_times 的值: " + x_times);

        y_step = int.Parse(yStepInput.text);
        Debug.Log("y_step 的值: " + y_step);

        // stateText.text = "Port name: " + portName + ", Baud rate: " + baudRate + ", x_step: " + x_step + "x_span: " + x_span + ", x_times: " + x_times;
        stateText.text = "Updated";
        ResetInputFieldColors();

        if (!simulateMode)
        {
            // 初始化串口
            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 500;
            serialPort.WriteTimeout = 500;

            try
            {
                serialPort.Open();
                Debug.Log("串口已打开：" + portName);
                stateText.text = "Serial port is open: " + portName;
            }
            catch (System.Exception e)
            {
                Debug.LogError("Serial port open failed: " + e.Message);
                stateText.text = "Serial port open failed: " + e.Message;
            }
        }
        else
        {
            Debug.Log("模拟模式开启，跳过串口初始化");
        }
    }
    // 重置所有 InputField 的文本颜色
    private void ResetInputFieldColors()
    {
        portNameInput.textComponent.color = originalTextColor;
        baudRateInput.textComponent.color = originalTextColor;
        xStepInput.textComponent.color = originalTextColor;
        xSpanInput.textComponent.color = originalTextColor;
        xTimesInput.textComponent.color = originalTextColor;
        yStepInput.textComponent.color = originalTextColor;
        // 如果有更多的 InputField，可以在这里添加
    }

    // 发送JSON数据的方法
    public void SendJsonData(string jsonData)
    {
        if (!simulateMode)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                // 将JSON字符串发送到串口
                serialPort.WriteLine(jsonData);
                Debug.Log("已发送JSON数据: " + jsonData);
                stateText.text = "JSON data sent: " + jsonData;
            }
            else
            {
                Debug.LogError("串口未打开或不可用");
                stateText.text = "Serial port is not open or available";
            }
        }
        else
        {
            // 模拟模式下，打印JSON数据到控制台
            Debug.Log("模拟模式 - 发送的 JSON 数据: " + jsonData);
        }
    }

    void OnApplicationQuit()
    {
        if (!simulateMode)
        {
            // 关闭串口连接
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
                Debug.Log("串口已关闭");
            }
        }
    }

    public void Reset()
    {
        if(!simulateMode)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                // 创建一个整型列表来存储Slider的数值
                int[] sliderValues = new int[10];

                // 获取每个Slider的数值
                for (int i = 0; i < sliders.Length; i++)
                {
                    sliderValues[i] = (int)sliders[i].value;
                }

                // 创建一个包含state和target列表的对象
                var data = new
                {
                    state = 0,
                    target = sliderValues
                };

                // 将对象转换成JSON字符串
                string jsonData = JsonUtility.ToJson(data);
                Debug.Log("生成的 JSON 数据: " + jsonData);

                // 通过串口发送JSON数据
                SendJsonData(jsonData);
            }
            else
            {
                Debug.LogError("串口未打开或不可用");
                stateText.text = "Serial port is not open or available";
            }
        }
        else
        {
            Debug.Log("模拟模式 - 发送的重置指令");
        }

    }



    public void Xfunction()
    {
        StartCoroutine(SendDataRepeatedly());
        Debug.Log("x_step 的值: " + x_step);
    }
    private IEnumerator SendDataRepeatedly()
    {
        int currentStep = x_step;
        for (int i = 0; i < x_times; i++)
        {
            int motor1Value = motor1Toggle.isOn ? currentStep : 0;
            int motor2Value = motor2Toggle.isOn ? currentStep : 0;
            int motor3Value = motor3Toggle.isOn ? currentStep : 0;
            int motor4Value = motor4Toggle.isOn ? currentStep : 0;
            int motor5Value = motor5Toggle.isOn ? currentStep : 0;

            string jsonData = "{\"device\":1,\"state\":0,\"target\":[" + motor1Value + ", " + motor2Value + ", " + motor3Value + ", " + motor4Value + ", " + motor5Value + "]}";
            // Debug.Log("编辑好le的 JSON 数据: " + jsonData);
            // if (simulateMode)
            // string jsonData = "{\"device\":1,\"state\":0,\"target\":[" + 20 + ", " + 20 + ", " + 20 + ", " + 20 + ", " + 20 + "]}";
            if (simulateMode)
            {
                Debug.Log(jsonData);
            }
            else
            {
                // Debug.Log("编辑好的 JSON 数据: " + jsonData);
                SendJsonData(jsonData);
            }
            currentStep += x_step;
            yield return new WaitForSeconds(x_span);
        }
    }

    public void Yfunction()
    {
        StartCoroutine(SendDataRepeatedl_y());
    }
    private IEnumerator SendDataRepeatedl_y()
    {
        // int currentStep = y_step;
        for (int i = 0; i < y_times; i++)
        {
            string jsonData = "{\"device\":0,\"state\":0,\"target\":[" + y_step + ", " + y_step + ", " + y_step + ", " + y_step + ", " + y_step + "]}";
            // string jsonData = "{\"device\":0,\"state\":0,\"target\":[" + currentStep + ", " + currentStep + ", " + currentStep + ", " + currentStep + ", " + currentStep + "]}";
            // string jsonData = "{\"device\":1,\"state\":0,\"target\":[" + 20 + ", " + 20 + ", " + 20 + ", " + 20 + ", " + 20 + "]}";
            // int motor1Value = motor1Toggle.isOn ? currentStep : 0;
            // int motor2Value = motor2Toggle.isOn ? currentStep : 0;
            // int motor3Value = motor3Toggle.isOn ? currentStep : 0;
            // int motor4Value = motor4Toggle.isOn ? currentStep : 0;
            // int motor5Value = motor5Toggle.isOn ? currentStep : 0;
            // string jsonData = "{\"device\":1,\"state\":0,\"target\":[" + motor1Value + ", " + motor2Value + ", " + motor3Value + ", " + motor4Value + ", " + motor5Value + "]}";

            if (simulateMode)
            {
                Debug.Log(jsonData);
            }
            else
            {
                // Debug.Log("编辑好的 JSON 数据: " + jsonData);
                SendJsonData(jsonData);
            }
            // currentStep += y_step;
            yield return new WaitForSeconds(y_span);
        }
    }

    public void Scan()
    {
        //执行完Yfunction再执行Xfunction
        StartCoroutine(ScanCoroutine());
        
    }

    private IEnumerator ScanCoroutine()
    {
        // Yfunction();
        yield return StartCoroutine(SendDataRepeatedl_y());

        // Xfunction();
        yield return StartCoroutine(SendDataRepeatedly());
    }

}