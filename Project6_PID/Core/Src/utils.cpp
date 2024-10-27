//
// Created by guanhuai on 24-10-26.
//

#include "utils.h"

float math::loopLimit(float val, const float& min, const float& max) {
    if (min >= max)
        return val;
    if (val > max) {
        while (val > max)
            val -= (max - min);
    } else if (val < min) {
        while (val < min)
            val += (max - min);
    }
    return val;
}

float math::ecd2deg(const float& ecd, const float& ecd_range) {
    return ecd * 360.f / ecd_range;
}

float math::clamp(float val, const float& min, const float& max) {
    if (val < min) {
        return min;
    } else if (val > max) {
        return max;
    } else {
        return val;
    }
}