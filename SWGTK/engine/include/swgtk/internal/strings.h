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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_INTERNAL_STRINGS_H_
#define SWGTK_ENGINE_INCLUDE_SWGTK_INTERNAL_STRINGS_H_

/*
    TODO: Implement the following functions *as needed*:
    
    explicit_bzero
    index
    rindex
    ffs
    ffsl
    ffsll
    strcasecmp
    strncasecmp
    strcasecmp_l
    strncasecmp_l

*/

#include <cstring>
inline int bcmp(const void *s1, const void *s2, size_t n) noexcept {
    return std::memcmp(s1, s2, n);
}

inline void bcopy(const void *src, void *dest, size_t n) noexcept {
    std::memcpy(dest, src, n);
}

inline void bzero(void* s, size_t n) noexcept {
    std::memset(s, 0, n);
}

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_INTERNAL_STRINGS_H_