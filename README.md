# SWGTK (SoftWare & Games ToolKit)

SWGTK is a toolkit for creating games and applications. It comes with built-in Lua bindings.
SWGTK is written in C++, is relatively unopinionated, and is a great starting place for creating larger engines.
Having SDL3 at the backend, SWGTK should be portable to anything that SDL3 can support.

**The project is still in early stages and is only a *thin* wrapper around SDL. This means some SDL types will still be used in your code.**

## Design

SWGTK is designed to be flexible, able to fully cooperate with the user's vision. Its focus is, primarily, on getting a window on the screen and hooking up the user with an easy-to-use rendering pipeling. The rendering system itself is very extensible. Our plans are to eventually have a rendering pipeline for every backend SDL3 supports, which should cover the vast majority of use cases.

A rendering backend is selected by the user and is injected into the application during initialization. The class hierarchy is intended not only as a means to support more renderers, but also allows users to customize existing ones to fit their exact needs.

**Note:** This library, by design choice, does *not* come with any audio features. The intention is to make audio handling a 3rd party user choice if they need it. How exactly is this going to work with the Lua runner? (See below.) I don't know yet.

This library is built with a strict set of compiler options and analyzer warnings. Warnings
are treated as errors. The code is also frequently tested with both AddressSanitizer and UndefinedBehaviorSanitizer.
This project takes safe software and best practices very seriously.

Example programs can be found in the 'examples' folder. The examples are kind of a testing playground at the moment, as a proper testing framework is still in the works.

## Code Standards & Technologies

This library was made possible by the following tools and libraries.(Thanks to the efforts of all software developers, open source or otherwise!)

- Language Standards
  - C++ 23
  - Lua 5.4
  - CMake 3.25 or above
- 3rd Party Libraries
  - [SDL3](https://github.com/libsdl-org/SDL)
  - SDL3_image
  - SDL3_ttf
  - [sol3](https://github.com/ThePhD/sol2) - For the Lua bindings.
  - [fmt](https://github.com/fmtlib/fmt)
  - [gsl-lite](https://github.com/gsl-lite/gsl-lite) - A support library for the C++ Core Guidelines
- Development Tools
  - Visual Studio
  - CLion
  - VS Code
  - [CPM](https://github.com/cpm-cmake/CPM.cmake) - A package manager that works directly in CMake
  - clang-tidy
  - cppcheck
  - cpplint

## Features

**This project is still in early development.** Here is a short list of working features and features planned for future
releases:

- [x] Single window applications.
- [ ] Multi-window applications.
- [ ] Web-based applications.(Via [emscripten](https://emscripten.org/))
- [ ] Mobile applications.
- [x] Default font.
- [x] Interface for user chosen fonts.
- [x] Input from mouse and keyboard.
- [ ] Input from touchscreen.
- [ ] Input from gamepads.
- [ ] Input from joysticks.
- [x] External Lua scripts for C++ programs.
- [x] Dedicated Lua runner. (Inspired by [Love2D](https://github.com/love2d/love))
- [x] Hardware accelerated 2D rendering system.
- [ ] 2D software rendering system.
- [ ] 3D capable GPU rendering systems.

**Note:** Porting SWGTK to consoles are further down the road and will require additional repositories according to each SDK's terms of use.

## Getting Started

### Requirements

Your compiler **must** support C++ 23 and you must be using CMake 3.25 or later.

- LLVM 19 or newer
- GCC 14 or newer
- Visual Studio 2022 or newer

### C++ with CMake

SWGTK uses CMake as the building system. Here is an example of adding it to your project. The sample uses CPM as the package manager. It should go without saying, then, that you can easily add SWGTK using CMake's FetchContent API:

```cmake
CPMAddPackage("gh:m00se-3/SWGTK#main")

target_link_libraries([your_project] PRIVATE swgtk)
```

After this you can create your application using something like this:

```c++
#include <swgtk/App.hpp>
#include <swgtk/Scene.hpp>
#include <swgtk/Simple2DRenderer.hpp>

class MyAppClass : public swgtk::Scene::Node{
    public:
    explicit MyAppClass(const gsl::not_null<swgtk::Scene*>& scene) : swgtk::Scene::Node(scene) {}
    bool Create() override;        // Called after object is created.
    bool Update(float) override; // Called every frame.
};

int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
    constexpr auto windowWidth = 800;
    constexpr auto windowHeight = 600;

    if(swgtk::App app; app.InitGraphics("App Title.", windowWidth, windowHeight, swgtk::Simple2DRenderer::Create())) {
        app.RunGame<MyAppClass>();
    }
}
```

### For Visual Studio

If you clone this repository and build it from source, there is a special CMake preset for Visual Studio users.
Open a command prompt in the root directory and enter this:

```bash
  cmake . --preset=msvc -G "Visual Studio 17 2022"
```

This will generate a fresh Visual Studio solution in the 'build/msvc' directory.

### Lua with swl2d

The build includes an executable that can run Lua scripts without needing to set up a C++ program. To use it, simply type the following command:

```bash
 swl2d path/to/your/root/lua/file.lua [options...]
```

The current options are:

- --title "[your game title]"
- --width [width of your game window]
- --height [height of your game window]

Currently, your root file should look something like this:

```lua
function swgtk.OnCreate() 
    --- startup code here
end

function swgtk.OnUpdate(deltaTime)
    --- called every frame
end
```

Please see the example programs for more information.

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
