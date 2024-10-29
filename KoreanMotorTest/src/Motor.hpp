#pragma once
#include "A4988.h"

// 电机比例
const float motorRatio1 = 10.0f / 6.0f;  // 电机1的比例
const float motorRatio2 = 10.0f / 6.0f * 19.0f / 2.0f;  // 电机2的比例

// 电机状态，0表示正常，1表示归位，2表示空闲
int motorState = 0;  

// 电机引脚设置，DIR, STEP, SW
const int motorPins1[5][3] = { { 17, 18, 8 }, { 38, 37, 36 }, { 4, 5, 6 }, { 48, 45, 35 }, { 46, 9, 10 } };
const int motorPins2[5][3] = { { 19, 20, 3 }, { 41, 40, 39 }, { 7, 15, 16 }, { 14, 21, 47 }, { 11, 12, 13 } };

// A4988步进电机驱动参数
const short STPS = 600;  // 每转步数
const short RPM  = 60;   // 转速，以每分钟转数表示
const int   SLP  = 42;   // 休眠引脚

class DRV8428 : public A4988 {
private:
    short sw_pin;  // 开关引脚的成员变量

public:
    /*
     * 基本连接：只有DIR和STEP引脚连接。
     * 微步控制应该是硬连线的。
     */
    // 构造函数，初始化电机参数及开关引脚
    DRV8428(short steps, short dir_pin, short step_pin, short sw_pin) : A4988(steps, dir_pin, step_pin), sw_pin(sw_pin) {
        pinMode(sw_pin, INPUT);  // 初始化开关引脚
    }

    // 读取开关状态，按下返回true
    bool readSwitch() {
        return digitalRead(sw_pin) == LOW;  // 当开关按下时，返回true
    }
};

// 初始化多个步进电机
DRV8428 motors[10] = { 
    DRV8428(STPS, motorPins1[0][0], motorPins1[0][1], motorPins1[0][2]), 
    DRV8428(STPS, motorPins2[0][0], motorPins2[0][1], motorPins2[0][2]),
    DRV8428(STPS, motorPins1[1][0], motorPins1[1][1], motorPins1[1][2]), 
    DRV8428(STPS, motorPins2[1][0], motorPins2[1][1], motorPins2[1][2]),
    DRV8428(STPS, motorPins1[2][0], motorPins1[2][1], motorPins1[2][2]), 
    DRV8428(STPS, motorPins2[2][0], motorPins2[2][1], motorPins2[2][2]),
    DRV8428(STPS, motorPins1[3][0], motorPins1[3][1], motorPins1[3][2]), 
    DRV8428(STPS, motorPins2[3][0], motorPins2[3][1], motorPins2[3][2]),
    DRV8428(STPS, motorPins1[4][0], motorPins1[4][1], motorPins1[4][2]), 
    DRV8428(STPS, motorPins2[4][0], motorPins2[4][1], motorPins2[4][2]) 
};

class Camera {
private:
    DRV8428 motor1;  // 摄像头的电机1
    DRV8428 motor2;  // 摄像头的电机2

    int currentPitch = 0;  // 当前俯仰角
    int currentYaw   = 0;  // 当前偏航角
    int targetPitch  = 0;  // 目标俯仰角
    int targetYaw    = 0;  // 目标偏航角

    bool isHoming = false;  // 是否正在归位

    public:
    // 构造函数，初始化摄像头电机
    Camera(DRV8428 motor1, DRV8428 motor2) : motor1(motor1), motor2(motor2) {}

    // 设置目标偏航角
    void setYaw(int yaw) {
        targetYaw = yaw;
    }

    // 设置目标俯仰角
    void setPitch(int pitch) {
        targetPitch = pitch;
    }

    // 移动摄像头到目标角度
    bool move() {
        // 如果开关被按下，重置当前位置
        if (motor1.readSwitch()) {
            currentPitch = 0;
            // Serial.println("press");
        }
            
        if (motor2.readSwitch()) {
            currentYaw = 0;
            // Serial.println("press");
        }

        // 移动到目标俯仰角
        if (currentPitch != targetPitch) {
            if (currentPitch < targetPitch) {
                motor1.move(-1);  // 向上移动
                motor2.move(-1);  // 同步移动
                currentPitch++;
                // Serial.println("up");
            } else {
                motor1.move(1);  // 向下移动
                motor2.move(1);  // 同步移动
                currentPitch--;
                // Serial.println("down");
            }
            return false;  // 还未到达目标
        }

        // 移动到目标偏航角
        if (currentYaw != targetYaw) {
            if (currentYaw < targetYaw) {
                motor2.move(1);  // 向右移动
                currentYaw++;
            } else {
                motor2.move(-1);  // 向左移动
                currentYaw--;
            }
            return false;  // 还未到达目标
        }

        return true;  // 到达目标
    }

    // 归位函数
    bool moveHome() {
        if (isHoming) {
            return true;  // 已经在归位状态
        }
        if (!motor1.readSwitch()) {
            motor1.move(1);  // 向上移动电机1
            motor2.move(1);  // 向上移动电机2
            return false;  // 继续移动
        } else if (!motor2.readSwitch()) {
            motor2.move(-1);  // 向下移动电机2
            return false;  // 继续移动
        } else {
            // 都到达底部开关，重置位置
            currentPitch = 0;
            currentYaw   = 0;
            targetPitch  = 0;
            targetYaw    = 0;
            isHoming     = true;  // 设置为归位状态
            return true;  // 完成归位
        }
    }

    // 设置归位状态
    void setHoming(bool homing) {
        isHoming = homing;
    }
};

// 初始化多个摄像头
Camera cameras[5] = { 
    Camera(motors[0], motors[1]), 
    Camera(motors[2], motors[3]), 
    Camera(motors[4], motors[5]), 
    Camera(motors[6], motors[7]),
    Camera(motors[8], motors[9]) 
};
