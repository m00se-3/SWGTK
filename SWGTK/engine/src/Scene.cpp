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

#include <swgtk/Utility.hpp>
#include <swgtk/App.hpp>
#include <memory>

namespace swgtk {
	Scene::Scene(const gsl::not_null<App*>& parent)
	: _parent(parent), _renderer(parent->Renderer())
	{
	}

	bool Scene::Create() const {
		return _root->Create();
	}

	bool Scene::Update(const float dt) const {
		return _root->Update(dt);
	}

	void Scene::Destroy() const {
		_root->Destroy();
	}

} // namespace swgtk
