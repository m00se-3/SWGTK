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
#include "swgtk/Scene.hpp"

#include <swgtk/App.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {
  scene::scene(const object_ref<app>& parent) :
      _parent(parent), _renderer(parent->renderer()) {
  }

  auto scene::create() const -> bool {
    return _root->create();
  }

  auto scene::update(const float dt) const -> bool {
    return _root->update(dt);
  }

  void scene::destroy() const {
    _root->destroy();
  }

} // namespace swgtk
