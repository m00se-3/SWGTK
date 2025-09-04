# SWGTK (SoftWare & Games ToolKit)

SWGTK is a toolkit for creating games and applications. It comes with built-in Lua bindings.
SWGTK is written in C++ 23, is relatively unopinionated, and is a great starting place for creating larger engines.
Having SDL3 at the backend, SWGTK should be portable to anything that SDL3 can support.

**The project is still in early stages and is only a *thin* wrapper around SDL. This means some SDL types will still be used in your code.**

## Design

SWGTK is designed to be flexible, able to fully cooperate with the user's vision. Its focus is, primarily, on getting a window on the screen and hooking up the user with an easy-to-use rendering pipeling. The rendering system itself is very extensible. Our plans are to eventually have a rendering pipeline for every backend SDL3 supports, which should cover the vast majority of use cases.

A rendering backend is selected by the user and is injected into the application during initialization. The class hierarchy is intended not only as a means to support more renderers, but also allows users to customize existing ones to fit their exact needs.

**Note:** SWGTK, at this time, does *not* come with any audio features. I plan to include an example program for integrating a 3rd party library into your code.

This library is built with a strict set of compiler options and analyzer warnings. Warnings
are treated as errors. This project takes safe software and best practices very seriously.

Example programs can be found in the 'examples' folder. The examples are kind of a testing playground at the moment, as a proper testing framework is still in the works.

## Features

**This project is still in early development.** Here is a short list of working features and features planned for future
releases:

- [x] Single window applications.
- [ ] Multi-window applications.
- [x] Web-based applications.(Via [emscripten](https://emscripten.org/))
- [ ] Mobile applications.
- [x] Default font.
- [x] Interface for user chosen fonts.
- [x] Input from mouse and keyboard.
- [ ] Input from touchscreen.
- [ ] Input from gamepads.
- [ ] Input from joysticks.
- [x] External Lua scripts for C++ programs.
- [x] Dedicated 2D Lua runner. ([SWL](https://github.com/m00se-3/SWL))
- [x] Hardware accelerated 2D rendering system.
- [ ] 2D software rendering system.
- [ ] 3D capable GPU rendering pipelines.

## Getting Started

### Requirements

- C++ 23 compliant compiler
  - LLVM 19 or newer
  - GCC 14 or newer
  - Visual Studio 2022 or newer
- 3rd Party Libraries
  - [SDL3](https://github.com/libsdl-org/SDL)
  - SDL3_image
  - SDL3_ttf
  - [sol3](https://github.com/ThePhD/sol2) - For the Lua bindings.
  - Lua
- Development Tools
  - CMake
  - [CPM](https://github.com/cpm-cmake/CPM.cmake) - A package manager that works directly in CMake.
  - clang-tidy [Optional]
  - cppcheck [Optional]
  - ccache [Optional]

### Getting started with CMake

SWGTK requires CMake 3.28 or newer. Here is an example of adding it to your project. The sample uses CPM as the package manager. It should go without saying, then, that you can easily add SWGTK using CMake's FetchContent API:

```cmake
# v0.1.0 is somewhat outdated
CPMAddPackage("gh:m00se-3/SWGTK#main")

# This is the easiest way to make sure all the shared library dependencies are where they need to be.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# You will need to link a rendering back-end with the main library.
target_link_libraries([your_project] PRIVATE swgtk swgtk::SDLHW2D)
```

In addition, the following options can be added depending on your needs:
- SWGTK_BUILD_EXAMPLES: Include the example programs. (Default: ON)
- SWGTK_NO_CCACHE: Disable ccache support. (Default: OFF)
- SWGTK_INSTALL_FREETYPE: Build the Freetype font library from source. (Default: OFF)
- SWGTK_LUA_BINDINGS: Enable Lua scripting support via sol3. (Default: ON)
- SWGTK_BUILD_TESTS: Build the unit test suite. (Default: ON)
- SWGTK_EXCEPTIONS: Build with exceptions enabled. (Default: OFF)

After this you can create your application using something like this:

```c++
#include <swgtk/App.hpp>
#include <swgtk/Scene.hpp>
#include <swgtk/SDLHW2D.hpp>

class MyAppClass : public swgtk::Scene::Node{
    public:
    explicit MyAppClass(const swgtk::ObjectRef<swgtk::Scene>& scene) : swgtk::Scene::Node(scene) {}
    bool Create() override;        // Called after object is created.
    bool Update(float) override;   // Called every frame.
};

int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
    constexpr auto windowWidth = 800;
    constexpr auto windowHeight = 600;

    if(swgtk::App app; app.InitGraphics("App Title.", windowWidth, windowHeight, swgtk::SDLHW2D::Create())) {
        app.RunGame<MyAppClass>();
    }
}
```
Please see the example programs for more information.

### For Visual Studio

If you clone this repository and build it from source, there is a special CMake preset for Visual Studio users.
Open a command prompt in the root directory and enter this:

```bash
  cmake . --preset=msvc -G "Visual Studio 17 2022"
```

*If, at any time, you discover a bug or a memory leak from within the framework, please post an issue with details.*

## MIT License

Copyright (c) 2023 Samuel Bridgham

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
