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

#include <utility>
#include <swgtk/Utility.hpp>
#include <swgtk/App.hpp>

namespace swgtk {
	Scene::Scene(const gsl::not_null<App*>& parent, std::shared_ptr<Node>&& node) noexcept
	: _parent(parent), _renderer(parent->Renderer()), _root(std::move(node))
	{
	}

	bool Scene::Create() {
		return _root->Create(*this);
	}

	bool Scene::Update(const float dt) {
		return _root->Update(*this, dt);
	}

	void Scene::Destroy() {
		_root->Destroy(*this);
	}

} // namespace swgtk
