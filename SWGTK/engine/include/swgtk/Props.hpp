#ifndef SWGTK_PROPS_HPP
#define SWGTK_PROPS_HPP

#include <SDL3/SDL_video.h>

namespace swgtk::props {
  namespace winCreate {
    constexpr auto AlwaysOnTop = SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN;
    constexpr auto Borderless = SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN;
    constexpr auto ConstPopup = SDL_PROP_WINDOW_CREATE_CONSTRAIN_POPUP_BOOLEAN;
    constexpr auto Focusable = SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN;
    constexpr auto ExtContext = SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN;
    constexpr auto Flags = SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER;
    constexpr auto Fullscreen = SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN;
    constexpr auto Height = SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER;
    constexpr auto Hidden = SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN;
    constexpr auto HighDPI = SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN;
    constexpr auto Maximized = SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN;
    constexpr auto Menu = SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN;
    constexpr auto Metal = SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN;
    constexpr auto Minimized = SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN;
    constexpr auto Modal = SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN;
    constexpr auto MouseGrabbed = SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN;
    constexpr auto OpenGL = SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN;
    constexpr auto Parent = SDL_PROP_WINDOW_CREATE_PARENT_POINTER;
    constexpr auto Resizable = SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN;
    constexpr auto Title = SDL_PROP_WINDOW_CREATE_TITLE_STRING;
    constexpr auto Transparent = SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN;
    constexpr auto Tooltip = SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN;
    constexpr auto Utility = SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN;
    constexpr auto Vulkan = SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN;
    constexpr auto Width = SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER;
    constexpr auto X = SDL_PROP_WINDOW_CREATE_X_NUMBER;
    constexpr auto Y = SDL_PROP_WINDOW_CREATE_Y_NUMBER;
    constexpr auto Cocoa = SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER;
    constexpr auto CocoaView = SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER;
    constexpr auto WindowScene = SDL_PROP_WINDOW_CREATE_WINDOWSCENE_POINTER;
    constexpr auto WaylandSurfaceRoleCustom = SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN;
    constexpr auto WaylandEGL = SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN;
    // constexpr auto WaylandID = SDL_PROP_WINDOW_CREATE_WAYLAND_WINDOW_ID_STRING;
    constexpr auto WaylandWLSurface = SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER;
    constexpr auto Win32HWND = SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER;
    constexpr auto Win32PixelFormat = SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER;
    constexpr auto X11 = SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER;
    constexpr auto EmscriptenCanvas = SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_CANVAS_ID_STRING;
    constexpr auto EmscriptenKeyboard = SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_KEYBOARD_ELEMENT_STRING;
    // constexpr auto VisionOSSettings = SDL_PROP_WINDOW_CREATE_VISIONOS_SETTINGS_STRING;
  } // namespace winCreate

  namespace renderer {

  }
} // namespace swgtk::props

#endif // !SWGTK_PROPS_HPP
