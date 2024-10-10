//
// Created by guanhuai on 24-10-9.
//

#include "remote_control.h"
#include <cstdio>
#include <cstring>

const int16_t RC_CH_OFFSET = 1024;
const int16_t RC_CH_MIN = 364;
const int16_t RC_CH_MAX = 1684;

RC rc(&huart3);

RC::RC(UART_HandleTypeDef* huart) : huart_(huart), rx_len_(0) {}

void RC::init() {
    reset();
    rx_len_ = 0;
    __HAL_UART_ENABLE_IT(huart_, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart_, rx_buf_, 1);
}

bool RC::checkUart(UART_HandleTypeDef* huart) {
    return huart_ == huart;
}

void RC::reset() {
    channel_.l_col = 0;
    channel_.l_row = 0;
    channel_.r_col = 0;
    channel_.r_row = 0;
    switch_.l = DOWN;
    switch_.r = DOWN;
    mouse_.x = 0;
    mouse_.y = 0;
    mouse_.z = 0;
    mouse_.l_btn = false;
    mouse_.r_btn = false;
    key_ = 0;
}

void RC::handle() {
    channel_.r_row =
        static_cast<float>(((rx_data_[0] | (rx_data_[1] << 8)) & 0x07FF) - RC_CH_OFFSET) / (RC_CH_MAX - RC_CH_MIN) * 2;
    channel_.r_col = static_cast<float>(((rx_data_[1] >> 3 | (rx_data_[2] << 5)) & 0x07FF) - RC_CH_OFFSET)
        / (RC_CH_MAX - RC_CH_MIN) * 2;
    channel_.l_row =
        static_cast<float>(((rx_data_[2] >> 6 | (rx_data_[3] << 2) | (rx_data_[4] << 10)) & 0x07FF) - RC_CH_OFFSET)
        / (RC_CH_MAX - RC_CH_MIN) * 2;
    channel_.l_col = static_cast<float>(((rx_data_[4] >> 1 | rx_data_[5] << 7) & 0x07FF) - RC_CH_OFFSET)
        / (RC_CH_MAX - RC_CH_MIN) * 2;
    auto switch_r_raw = (rx_data_[5] >> 4 & 0x0003);
    if (switch_r_raw == 1) {
        switch_.r = RCSwitchState_e::UP;
    } else if (switch_r_raw == 2) {
        switch_.r = RCSwitchState_e::DOWN;
    } else if (switch_r_raw == 3) {
        switch_.r = RCSwitchState_e::MID;
    }
    auto switch_l_raw = (rx_data_[5] >> 6 & 0x0003);
    if (switch_l_raw == 1) {
        switch_.l = RCSwitchState_e::UP;
    } else if (switch_l_raw == 2) {
        switch_.l = RCSwitchState_e::DOWN;
    } else if (switch_l_raw == 3) {
        switch_.l = RCSwitchState_e::MID;
    }
    mouse_.x = (int16_t)(rx_data_[6] | (rx_data_[7] << 8));
    mouse_.y = (int16_t)(rx_data_[8] | (rx_data_[9] << 8));
    mouse_.z = (int16_t)(rx_data_[10] | (rx_data_[11] << 8));
    mouse_.l_btn = (rx_data_[12] != 0);
    mouse_.r_btn = (rx_data_[13] != 0);
    key_ = (uint16_t)(rx_data_[14] | (rx_data_[15] << 8));

    printf("%s\n", rc.toString().c_str());
}

void RC::idleCallback() {
    HAL_UART_AbortReceive(huart_);
    //    auto recv_len = RC_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart_->hdmarx);
    if (rx_len_ >= RC_RX_DATA_SIZE) {
        memcpy(rx_data_, rx_buf_ + rx_len_ - RC_RX_DATA_SIZE, RC_RX_DATA_SIZE);
    }
    rx_len_ = 0;
    HAL_UART_Receive_DMA(huart_, rx_buf_, 1);
}

void RC::rxCpltCallback() {
    rx_len_ += 1;
    HAL_UART_Receive_DMA(huart_, rx_buf_ + rx_len_, 1);
}

std::string RC::toString() const {
    char buf[256];
    sprintf(
        buf,
        "l_col: %f, l_row: %f, r_col: %f, r_row: %f, switch_l: %d, switch_r: %d, mouse: %d, %d, %d, mouse_l_btn: %d, mouse_r_btn: %d, key: %d\n",
        channel_.l_col,
        channel_.l_row,
        channel_.r_col,
        channel_.r_row,
        switch_.l,
        switch_.r,
        mouse_.x,
        mouse_.y,
        mouse_.z,
        mouse_.l_btn,
        mouse_.r_btn,
        key_);
    return { buf };
}