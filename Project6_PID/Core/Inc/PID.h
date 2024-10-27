//
// Created by guanhuai on 24-10-26.
//

#ifndef PROJECT6_PID_PID_H
#define PROJECT6_PID_PID_H

#include "utils.h"

class PID {
public:
    PID(float kp, float ki, float kd, float i_max = 0, float out_max = 0);

    void reset();
    float calc(float ref, float fdb);
    void setOutMax(float out_max) {
        out_max_ = out_max;
    }
    void setIMax(float i_max) {
        i_max_ = i_max;
    }

private:
    float kp_, ki_, kd_;
    float i_max_, out_max_;
    float output_ {};

    float ref_ {}, fdb_ {};
    float err_, err_sum_, last_err_;
    float pout_ {}, iout_ {}, dout_ {};
};

#endif //PROJECT6_PID_PID_H
