#include "../src/button_state.hpp"
#include "../src/scroll_math.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

namespace {

    bool near(const double actual, const double expected, const double epsilon = 1e-9) {
        return std::abs(actual - expected) <= epsilon;
    }

} // namespace

int main() {
    const auto unrelatedButton = Autoscroll::decideSwallowedButtonEvent(false, false, false, false);
    assert(!unrelatedButton.cancelEvent);
    assert(!unrelatedButton.clearButton);

    const auto pendingRelease = Autoscroll::decideSwallowedButtonEvent(true, false, false, true);
    assert(pendingRelease.cancelEvent);
    assert(pendingRelease.clearButton);

    const auto stalePressWhileDisabled = Autoscroll::decideSwallowedButtonEvent(true, false, false, false);
    assert(!stalePressWhileDisabled.cancelEvent);
    assert(stalePressWhileDisabled.clearButton);

    const auto pendingPressWhileEnabled = Autoscroll::decideSwallowedButtonEvent(true, true, false, false);
    assert(pendingPressWhileEnabled.cancelEvent);
    assert(!pendingPressWhileEnabled.clearButton);

    const auto pendingPressWhileActive = Autoscroll::decideSwallowedButtonEvent(true, false, true, false);
    assert(pendingPressWhileActive.cancelEvent);
    assert(!pendingPressWhileActive.clearButton);

    const Autoscroll::ScrollCurve defaults;
    assert(near(defaults.sensitivity, 4.0));
    assert(near(defaults.acceleration, 1.075));
    assert(near(defaults.maxSpeed, 1500.0));

    const Autoscroll::ScrollCurve linear{
        .deadZone     = 10.0,
        .sensitivity  = 2.0,
        .acceleration = 1.0,
        .maxSpeed     = 100.0,
    };

    assert(near(Autoscroll::velocityForDisplacement(0.0, linear), 0.0));
    assert(near(Autoscroll::velocityForDisplacement(10.0, linear), 0.0));
    assert(near(Autoscroll::velocityForDisplacement(-9.0, linear), 0.0));
    assert(near(Autoscroll::velocityForDisplacement(20.0, linear), 20.0));
    assert(near(Autoscroll::velocityForDisplacement(-20.0, linear), -20.0));
    assert(near(Autoscroll::velocityForDisplacement(1000.0, linear), 100.0));
    assert(near(Autoscroll::deltaForFrame(20.0, 0.5, linear), 10.0));
    assert(near(Autoscroll::deltaForFrame(20.0, 0.0, linear), 0.0));

    const Autoscroll::ScrollCurve accelerated{
        .deadZone     = 0.0,
        .sensitivity  = 1.0,
        .acceleration = 2.0,
        .maxSpeed     = 1000.0,
    };

    assert(near(Autoscroll::velocityForDisplacement(5.0, accelerated), 25.0));
    assert(near(Autoscroll::velocityForDisplacement(-5.0, accelerated), -25.0));

    std::cout << "button state and scroll math tests passed\n";
    return 0;
}
