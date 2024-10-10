//
// Created by guanhuai on 24-10-9.
//

#ifndef PROJECT4_DBUS_CALLBACK_H
#define PROJECT4_DBUS_CALLBACK_H

#include "main.h"
#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif

void User_UART_IdleHandler(UART_HandleTypeDef* huart);
void rcInit();

#ifdef __cplusplus
}
#endif

#endif //PROJECT4_DBUS_CALLBACK_H