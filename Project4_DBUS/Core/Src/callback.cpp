//
// Created by guanhuai on 24-10-9.
//

#include "callback.h"
#include "remote_control.h"
#include <stdio.h>

extern RC rc;

void User_UART_IdleHandler(UART_HandleTypeDef* huart) {
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        if (rc.checkUart(huart)) {
            rc.idleCallback();
            rc.handle();
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (rc.checkUart(huart)) {
        rc.rxCpltCallback();
    }
}

void rcInit() {
    rc.init();
}