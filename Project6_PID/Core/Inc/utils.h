//
// Created by guanhuai on 24-10-26.
//

#ifndef PROJECT6_PID_UTILS_H
#define PROJECT6_PID_UTILS_H

namespace math {
float loopLimit(float val, const float& min, const float& max);

float ecd2deg(const float& ecd, const float& ecd_range);

float clamp(float val, const float& min, const float& max);
} // namespace math

#endif //PROJECT6_PID_UTILS_H
