/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_TIMER_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_TIMER_HPP_

#include <chrono>

namespace swgtk {

    class Timer {
      public:
        void UpdateTime() {
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

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_TIMER_HPP_
