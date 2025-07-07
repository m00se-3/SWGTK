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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_

#include <type_traits>
#include <utility>

#ifdef _DEBUG
#include <fmt/format.h>
#define DEBUG_PRINT(Debug_Message, Debug_Error) fmt::println(Debug_Message, Debug_Error);
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) fmt::println(Debug_Message, Debug_Error_1, Debug_error_2);

#else
#define DEBUG_PRINT(Debug_Message, Debug_Error) 
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) 
#endif

namespace swgtk {
    
    /**
     * @brief A proxy class intended to deter users from erroneously deleting
     * framework pointers. You can access the underlying object with the '->'
     * operator.
     * 
     * @tparam Ptr 
     */
    template<typename Ptr>
    class ObjectRef {
    public:
        ObjectRef() = default;
        explicit ObjectRef(Ptr* ptr) : _ptr(ptr) {}
        Ptr* operator->() const { return _ptr; }
        operator bool() const { return _ptr != nullptr; }

    private:
        Ptr* _ptr = nullptr;
    };

    // Enum class operators.

    template <typename T>
    concept ScopedEnum = std::is_scoped_enum_v<T>;

    template <ScopedEnum Enum>
    [[nodiscard]] constexpr Enum operator&(const Enum rhs, const Enum lhs) {
        return Enum{std::to_underlying(rhs) & std::to_underlying(lhs)};
    }

    template <ScopedEnum Enum>
    [[nodiscard]] constexpr Enum operator|(const Enum rhs, const Enum lhs) {
        return Enum{std::to_underlying(rhs) | std::to_underlying(lhs)};
    }

    enum class LuaPrivledges {
        None = 0,
        UserInput = 1,
        DrawCalls = 2,
        WindowControl = 4,
        Fonts = 8,
        All = 15,
    };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_