//
// Created by guanhuai on 24-10-4.
//

#include "main.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

extern char rx_buffer[RX_DATA_LEN];

int led_id = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    //    if (htim->Instance == htim6.Instance) {
    //        printf("Timer 6 elapsed\n");
    //    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    //    if (htim->Instance == htim5.Instance) {
    //        printf("Timer 5 channel %d elapsed\n", htim5.Channel);
    //    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == huart1.Instance) {
        if (rx_buffer[0] == 'b') {
            led_id = 0;
            printf("Input detected, LED: blue\n");
        } else if (rx_buffer[0] == 'g') {
            led_id = 1;
            printf("Input detected, LED: green\n");
        } else if (rx_buffer[0] == 'r') {
            led_id = 2;
            printf("Input detected, LED: red\n");
        } else {
            printf("Invalid command: %s\n", rx_buffer);
        }

        HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, RX_DATA_LEN);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == USER_KEY_Pin) {
        led_id = (led_id + 1) % 3;
        printf("Key pressed, LED : %s\n", led_id == 0 ? "blue" : led_id == 1 ? "green" : "red");
    }
}