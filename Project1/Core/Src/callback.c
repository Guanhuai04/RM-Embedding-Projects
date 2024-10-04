//
// Created by guanhuai on 24-10-3.
//

#include "main.h"
#include "tim.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == htim6.Instance) {
      HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    }
}