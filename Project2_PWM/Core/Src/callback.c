//
// Created by guanhuai on 24-10-3.
//

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "tim.h"
#include "usart.h"

extern char rx_buffer[RX_DATA_LEN];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == huart6.Instance) {
        char message[50];
        sprintf(message, "Received: %s\n", rx_buffer);
        HAL_UART_Transmit(&huart6, (uint8_t*)message, strlen(message), 0xFFFF);
        HAL_UART_Receive_IT(&huart6, (uint8_t*)rx_buffer, RX_DATA_LEN);
    }
}