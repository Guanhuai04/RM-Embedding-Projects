//
// Created by guanhuai on 24-10-9.
//

#ifndef PROJECT4_DBUS_REMOTE_CONTROL_H
#define PROJECT4_DBUS_REMOTE_CONTROL_H

#include "usart.h"
#include <string>

#define RC_RX_DATA_SIZE 18u
#define RC_RX_BUF_SIZE 30u

class RC {
private:
    UART_HandleTypeDef* huart_;
    uint8_t rx_buf_[RC_RX_BUF_SIZE] {};
    uint8_t rx_data_[RC_RX_DATA_SIZE] {};
    volatile uint8_t rx_len_;

public:
    enum RCSwitchState_e { UP = 0, MID, DOWN };
    struct RCChannel {
        float r_row;
        float r_col;
        float l_row;
        float l_col;
    } channel_ {};
    struct RCSwitch {
        RCSwitchState_e l;
        RCSwitchState_e r;
    } switch_ {};
    struct Mouse {
        int16_t x;
        int16_t y;
        int16_t z;
        bool l_btn;
        bool r_btn;
    } mouse_ {};
    uint16_t key_;

public:
    RC(UART_HandleTypeDef* huart);

    void init();
    void reset();
    void handle();
    bool checkUart(UART_HandleTypeDef* huart);
    void idleCallback();
    void rxCpltCallback();
    std::string toString() const;
};

#endif //PROJECT4_DBUS_REMOTE_CONTROL_H
