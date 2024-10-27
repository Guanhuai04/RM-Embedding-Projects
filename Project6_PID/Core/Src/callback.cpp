//
// Created by guanhuai on 24-10-26.
//

#include "callback.h"
#include "can.h"
#include "motor.h"
#include "usart.h"
#include <cstdio>

PID pos_pid(1, 0.01, 0.8, 0);
PID speed_pid(10, 0.5, 1, 4000);
//Motor motor({ Motor::Type_e::M2006, 0x201 }, Motor::ControlMethod_e::SPEED, &pos_pid, &speed_pid);
Motor motor({ Motor::Type_e::M2006, 0x201 }, Motor::ControlMethod_e::POSITION_SPEED, &pos_pid, &speed_pid);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->Instance == hcan1.Instance) {
        CAN_RxHeaderTypeDef rx_header;
        static uint8_t rx_data[8];
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
        printf("Received: %d %d %d %d",
               rx_data[0] << 8 | rx_data[1],
               rx_data[2] << 8 | rx_data[3],
               rx_data[4] << 8 | rx_data[5],
               rx_data[6]);
        motor.canRxMsgCallback(hcan, rx_header, rx_data);
    }
}

void motorInit() {
    //    motor.setSpeed(60);
    motor.setAngle(20);
}

void motorHandler() {
    motor.handle();
    motor.canTxMsg(&hcan1);
}