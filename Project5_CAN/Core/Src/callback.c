//
// Created by guanhuai on 24-10-12.
//

#include "can.h"
#include "main.h"
#include <stdio.h>

CAN_RxHeaderTypeDef rxHeader;
uint8_t rxData[1];
float values[4];

float linearMapping(uint16_t in, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    float k = ((float)(out_max - out_min)) / ((float)(in_max - in_min)); // 计算斜率
    return (float)(out_min) + k * ((float)(in - in_min));
}

void canRxMsgCallback_v1(const uint8_t rx_data[8]) {
    uint16_t ecd = (uint16_t)rx_data[0] << 8 | (uint16_t)rx_data[1];
    values[0] = linearMapping(ecd, 0, 8191, 0, 360);
    values[1] = (float)(int16_t)((uint16_t)rx_data[2] << 8 | (uint16_t)rx_data[3]);
    values[2] = (float)(int16_t)((uint16_t)rx_data[4] << 8 | (uint16_t)rx_data[5]);
    values[3] = (float)(int8_t)rx_data[6];
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->Instance == hcan1.Instance) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
        //        canRxMsgCallback_v1(rxData);
        //        printf("angle: %8.6f\tspeed:%8.6f\tintensity:%8.6f\ttemp:%8.6f\n", values[0], values[1], values[2], values[3]);

        printf("Received: %d StdId: %lu\n", rxData[0], rxHeader.StdId);
        if (rxHeader.StdId == 0x000) {
            HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, rxData[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

uint8_t flag = 0;

void sendLEDData(uint8_t data) {
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[1] = { data };
    uint32_t TxMailbox;

    // 设置CAN标识符
    TxHeader.StdId = 0x001; // 使用0x200或0x1FF
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 1;

    // 将消息添加到发送邮箱
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        // 错误处理
        Error_Handler();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == USER_KEY_Pin) {
        if (HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == GPIO_PIN_SET) {
            flag ^= 1;
            sendLEDData(flag);
            printf("Send: %d\n", flag);
        }
    }
}
