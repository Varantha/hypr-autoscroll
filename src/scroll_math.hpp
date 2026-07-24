#pragma once

#include <algorithm>
#include <cmath>

namespace Autoscroll {

    struct ScrollCurve {
        double deadZone     = 12.0;
        double sensitivity  = 8.0;
        double acceleration = 1.15;
        double maxSpeed     = 3000.0;
    };

    [[nodiscard]] inline double velocityForDisplacement(const double displacement, const ScrollCurve& curve) {
        const double distance = std::abs(displacement);

        if (distance <= curve.deadZone || curve.sensitivity <= 0.0 || curve.maxSpeed <= 0.0) {
            return 0.0;
        }

        const double outsideDeadZone = distance - curve.deadZone;
        const double speed =
            std::min(curve.maxSpeed, curve.sensitivity * std::pow(outsideDeadZone, std::max(0.0, curve.acceleration)));

        return std::copysign(speed, displacement);
    }

    [[nodiscard]] inline double deltaForFrame(const double displacement, const double elapsedSeconds,
                                              const ScrollCurve& curve) {
        if (elapsedSeconds <= 0.0)
            return 0.0;

        return velocityForDisplacement(displacement, curve) * elapsedSeconds;
    }

} // namespace Autoscroll
