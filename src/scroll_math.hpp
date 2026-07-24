#pragma once

#include <algorithm>
#include <cmath>

namespace Autoscroll {

    inline constexpr double DEFAULT_DEAD_ZONE    = 12.0;
    inline constexpr double DEFAULT_SENSITIVITY  = 4.0;
    inline constexpr double DEFAULT_ACCELERATION = 1.075;
    inline constexpr double DEFAULT_MAX_SPEED    = 1500.0;

    struct ScrollCurve {
        double deadZone     = DEFAULT_DEAD_ZONE;
        double sensitivity  = DEFAULT_SENSITIVITY;
        double acceleration = DEFAULT_ACCELERATION;
        double maxSpeed     = DEFAULT_MAX_SPEED;
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
