#ifndef SWGTK_PROPS_HPP
#define SWGTK_PROPS_HPP

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_process.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_thread.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_tray.h>

#include <cstdint>
#include <variant>
#include <string>

namespace swgtk::props {

  using PropValue = std::variant<std::monostate, void*, std::string, std::int64_t, float, bool>;

  template<typename... T>
  struct PropOverload final : public T... {
    using T::operator()...;
  };

  namespace metadata {
    constexpr auto Name = SDL_PROP_APP_METADATA_NAME_STRING;
    constexpr auto Version = SDL_PROP_APP_METADATA_VERSION_STRING;
    constexpr auto ID = SDL_PROP_APP_METADATA_IDENTIFIER_STRING;
    constexpr auto Creater = SDL_PROP_APP_METADATA_CREATOR_STRING;
    constexpr auto Copyright = SDL_PROP_APP_METADATA_COPYRIGHT_STRING;
    constexpr auto URL = SDL_PROP_APP_METADATA_URL_STRING;
    constexpr auto Type = SDL_PROP_APP_METADATA_TYPE_STRING;
  } // namespace metadata

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

  namespace renderCreate {
    constexpr auto Name = SDL_PROP_RENDERER_CREATE_NAME_STRING;
    constexpr auto Window = SDL_PROP_RENDERER_CREATE_WINDOW_POINTER;
    constexpr auto Surface = SDL_PROP_RENDERER_CREATE_SURFACE_POINTER;
    constexpr auto ColorSpace = SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER;
    constexpr auto Vsync = SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER;

    namespace shader {
      constexpr auto Device = SDL_PROP_RENDERER_CREATE_GPU_DEVICE_POINTER;
      constexpr auto Spirv = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_SPIRV_BOOLEAN;
      constexpr auto DXIL = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_DXIL_BOOLEAN;
      constexpr auto MSL = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_MSL_BOOLEAN;
    } // namespace shader

    namespace vk {
      constexpr auto Instance = SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER;
      constexpr auto Surface = SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER;
      constexpr auto PhyicalDevice = SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER;
      constexpr auto Device = SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER;
      constexpr auto GfxQueue = SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER;
      constexpr auto PresentQueue = SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER;
    } // namespace vk

  } // namespace renderCreate

  namespace gpuCreate {
    constexpr auto Debug = SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN;
    constexpr auto LowPower = SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN;
    constexpr auto Verbose = SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN;
    constexpr auto Name = SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING;
    constexpr auto ClipDist = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN;
    constexpr auto DepthClamping = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN;
    constexpr auto IndirectDraw = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN;
    constexpr auto Anisotropy = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN;

    namespace shader {
      constexpr auto Private = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_PRIVATE_BOOLEAN;
      constexpr auto Spirv = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN;
      constexpr auto DXBC = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXBC_BOOLEAN;
      constexpr auto DXSIL = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN;
      constexpr auto MSL = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_MSL_BOOLEAN;
      constexpr auto Metal = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_METALLIB_BOOLEAN;
    } // namespace shader

    namespace d3d12 {
      constexpr auto SemanticPrefix = SDL_PROP_GPU_DEVICE_CREATE_D3D12_SEMANTIC_NAME_STRING;
      constexpr auto FewResSlots = SDL_PROP_GPU_DEVICE_CREATE_D3D12_ALLOW_FEWER_RESOURCE_SLOTS_BOOLEAN;
      constexpr auto SDKVersion = SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_VERSION_NUMBER;
      constexpr auto SDKPath = SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_PATH_STRING;
    } // namespace d3d12

    namespace vk {
      constexpr auto RequireHardware = SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN;
      constexpr auto Options = SDL_PROP_GPU_DEVICE_CREATE_VULKAN_OPTIONS_POINTER;
    } // namespace vk

  } // namespace gpuCreate

  namespace thread {
    constexpr auto Entry = SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER;
    constexpr auto Name = SDL_PROP_THREAD_CREATE_NAME_STRING;
    constexpr auto UserData = SDL_PROP_THREAD_CREATE_USERDATA_POINTER;
    constexpr auto StackSize = SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER;
  } // namespace thread

  namespace ioFile {
    constexpr auto Win32Handle = SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER;
    constexpr auto File = SDL_PROP_IOSTREAM_STDIO_FILE_POINTER;
    constexpr auto FileDesc = SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER;
    constexpr auto AndroidAsset = SDL_PROP_IOSTREAM_ANDROID_AASSET_POINTER;
  } // namespace ioFile

  namespace ioMem {
    constexpr auto ConstPointer = SDL_PROP_IOSTREAM_MEMORY_POINTER;
    constexpr auto MemSize = SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER;
    constexpr auto Deleter = SDL_PROP_IOSTREAM_MEMORY_FREE_FUNC_POINTER;
    constexpr auto ChuckSize = SDL_PROP_IOSTREAM_DYNAMIC_CHUNKSIZE_NUMBER;
    constexpr auto Pointer = SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER;
  } // namespace ioMem

  namespace fileDia {
    constexpr auto Filters = SDL_PROP_FILE_DIALOG_FILTERS_POINTER;
    constexpr auto NumFilters = SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER;
    constexpr auto Window = SDL_PROP_FILE_DIALOG_WINDOW_POINTER;
    constexpr auto Location = SDL_PROP_FILE_DIALOG_LOCATION_STRING;
    constexpr auto Many = SDL_PROP_FILE_DIALOG_MANY_BOOLEAN;
    constexpr auto Title = SDL_PROP_FILE_DIALOG_TITLE_STRING;
    constexpr auto Accept = SDL_PROP_FILE_DIALOG_ACCEPT_STRING;
    constexpr auto Cancel = SDL_PROP_FILE_DIALOG_CANCEL_STRING;
  } // namespace fileDia

  namespace process {
    constexpr auto Args = SDL_PROP_PROCESS_CREATE_ARGS_POINTER;
    constexpr auto Env = SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER;
    constexpr auto WorkingDir = SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING;
    constexpr auto StdIn = SDL_PROP_PROCESS_CREATE_STDIN_NUMBER;
    constexpr auto StdOut = SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER;
    constexpr auto StdErr = SDL_PROP_PROCESS_CREATE_STDERR_NUMBER;
    constexpr auto StdInRedirect = SDL_PROP_PROCESS_CREATE_STDIN_POINTER;
    constexpr auto StdOutRedirect = SDL_PROP_PROCESS_CREATE_STDOUT_POINTER;
    constexpr auto StdErrRedirect = SDL_PROP_PROCESS_CREATE_STDERR_POINTER;
    constexpr auto ErrToOut = SDL_PROP_PROCESS_CREATE_STDERR_TO_STDOUT_BOOLEAN;
    constexpr auto Cmd = SDL_PROP_PROCESS_CREATE_CMDLINE_STRING;
    constexpr auto Background = SDL_PROP_PROCESS_BACKGROUND_BOOLEAN;
  } // namespace process

} // namespace swgtk::props

#endif // !SWGTK_PROPS_HPP
