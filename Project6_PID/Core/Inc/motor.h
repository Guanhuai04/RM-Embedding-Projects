//
// Created by guanhuai on 24-10-26.
//

#ifndef PROJECT6_PID_MOTOR_H
#define PROJECT6_PID_MOTOR_H

#include "PID.h"
#include "can.h"
#include <cstdint>

class Motor {
public:
    typedef enum Type { M3508, M2006 } Type_e;
    typedef enum ControlMethod { SPEED, POSITION_SPEED } ControlMethod_e;

    typedef struct MotorInfo {
        Type_e type;
        uint32_t can_std_id;
    } MotorInfo_t;

    // 电调反馈数据
    typedef struct MotorRawData {
        int16_t ecd; // encoder value(0~8191) 编码器值(0~8191)
        int16_t rotate_speed_rpm; // rotational speed(rpm) 转速(单位rpm)
        int16_t current; // torque current 转矩电流
        int8_t temp; // temperature 温度
    } MotorRawData_t;

private:
    MotorInfo_t info_;
    ControlMethod_e method_;
    MotorRawData_t raw_data_;

    PID* pos_pid_;
    PID* speed_pid_;

    float ratio_; // 减速比
    float intensity_float_ {};
    int16_t intensity_ {}; // 控制电流

    float fdb_angle_;
    float fdb_ecd_angle_, last_fdb_ecd_angle_;
    float fdb_speed_;
    float tgt_angle_;
    float tgt_speed_;

public:
    Motor(const MotorInfo_t& info, const ControlMethod_e& method, PID* pos_pid, PID* speed_pid);

    void handle();
    void canTxMsg(CAN_HandleTypeDef* hcan) const;
    void canRxMsgCallback(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, const uint8_t rx_data[8]);
    void setSpeed(const float& targetSpeed); // rpm
    void setAngle(const float& targetAngle); // deg
};

#endif //PROJECT6_PID_MOTOR_H
