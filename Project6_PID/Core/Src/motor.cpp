//
// Created by guanhuai on 24-10-26.
//

#include "motor.h"

Motor::Motor(const Motor::MotorInfo_t& info, const Motor::ControlMethod_e& method, PID* pos_pid, PID* speed_pid) :
    info_(info),
    method_(method),
    pos_pid_(pos_pid),
    speed_pid_(speed_pid) {
    tgt_angle_ = 0;
    tgt_speed_ = 0;
    fdb_angle_ = 0;
    fdb_speed_ = 0;
    pos_pid_->reset();
    speed_pid_->reset();

    if (info_.type == Motor::Type_e::M3508) {
        speed_pid_->setOutMax(16384);
        ratio_ = 3591.f / 187.f;
    } else if (info_.type == Motor::Type_e::M2006) {
        speed_pid_->setOutMax(10000);
        ratio_ = 36.f / 1.f;
    }
}

void Motor::canTxMsg(CAN_HandleTypeDef* hcan) const {
    static uint32_t can_tx_mail_box;
    static CAN_TxHeaderTypeDef can_tx_header;
    can_tx_header.StdId = 0x200;
    can_tx_header.IDE = CAN_ID_STD;
    can_tx_header.RTR = CAN_RTR_DATA;
    can_tx_header.DLC = 8;

    uint8_t can_tx_data[8] = {};
    can_tx_data[2 * (info_.can_std_id - 0x201)] = (intensity_ >> 8);
    can_tx_data[2 * (info_.can_std_id - 0x201) + 1] = (intensity_);

    HAL_CAN_AddTxMessage(hcan, &can_tx_header, can_tx_data, &can_tx_mail_box);
}

void Motor::canRxMsgCallback(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, const uint8_t rx_data[8]) {
    if (rx_header.StdId != info_.can_std_id) {
        return;
    }
    raw_data_.ecd = (int16_t)(rx_data[0] << 8 | rx_data[1]);
    raw_data_.rotate_speed_rpm = (int16_t)(rx_data[2] << 8 | rx_data[3]);
    raw_data_.current = (int16_t)(rx_data[4] << 8 | rx_data[5]);
    raw_data_.temp = (int8_t)rx_data[6];

    fdb_speed_ = (float)raw_data_.rotate_speed_rpm / ratio_;
    fdb_ecd_angle_ = math::ecd2deg(raw_data_.ecd, 8192); // ecd to deg
    fdb_angle_ += math::loopLimit(fdb_ecd_angle_ - last_fdb_ecd_angle_, -180.f, 180.f) / ratio_;
    last_fdb_ecd_angle_ = fdb_ecd_angle_;
}

void Motor::setSpeed(const float& targetSpeed) {
    tgt_speed_ = targetSpeed;
}

void Motor::setAngle(const float& targetAngle) {
    tgt_angle_ = targetAngle;
}

void Motor::handle() {
    if (method_ == ControlMethod::SPEED) {
        intensity_float_ = speed_pid_->calc(tgt_speed_, fdb_speed_);
    } else if (method_ == ControlMethod::POSITION_SPEED) {
        tgt_speed_ = pos_pid_->calc(tgt_angle_, fdb_angle_);
        intensity_float_ = speed_pid_->calc(tgt_speed_, fdb_speed_);
    }
    intensity_ = (int16_t)intensity_float_;
}