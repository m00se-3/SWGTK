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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_ERRCODES_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_ERRCODES_HPP_

namespace swgtk {
	// Not sure if I'm keeping these yet.
	enum class LuaError
	{
		Ok,
		FileDir404,
		ParsingFailed,
		BadResult,
	};

    

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_ERRCODES_HPP_
