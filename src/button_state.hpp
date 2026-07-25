#pragma once

namespace Autoscroll {

    struct SwallowedButtonDecision {
        bool cancelEvent = false;
        bool clearButton = false;
    };

    inline constexpr SwallowedButtonDecision decideSwallowedButtonEvent(const bool matchesSwallowedButton,
                                                                        const bool middleModeEnabled,
                                                                        const bool autoscrollActive,
                                                                        const bool released) {
        if (!matchesSwallowedButton)
            return {};

        if (released)
            return {.cancelEvent = true, .clearButton = true};

        if (!middleModeEnabled && !autoscrollActive)
            return {.cancelEvent = false, .clearButton = true};

        return {.cancelEvent = true, .clearButton = false};
    }

} // namespace Autoscroll
