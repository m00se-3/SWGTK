#ifndef SWGTK_TIMER_HPP
#define SWGTK_TIMER_HPP

#include <chrono>

namespace swgtk {

    class Timer {
      public:
        constexpr void UpdateTime() {
            _currentFrameTime = std::chrono::steady_clock::now();

            _timeDifference = std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime);
            _lastFrameTime = _currentFrameTime;
        }

        [[nodiscard]] constexpr auto GetSeconds() const { return std::chrono::duration<float, std::ratio<1,1>>(_timeDifference).count(); }
        [[nodiscard]] constexpr auto GetMicroseconds() const { return _timeDifference.count(); }

      private:
        using timePoint = std::chrono::time_point<std::chrono::steady_clock>;

        timePoint _lastFrameTime = std::chrono::steady_clock::now(),
                  _currentFrameTime;
        std::chrono::microseconds _timeDifference{};
    };

}

#endif //SWGTK_TIMER_HPP
