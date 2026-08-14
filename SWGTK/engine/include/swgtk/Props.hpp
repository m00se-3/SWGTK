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
  enum class meta {
    name,
    version,
    id,
    creator,
    copyright,
    url,
    type,
  };

  enum class window {

  };
}
namespace swgtk {
  template<swgtk::props::meta Id, typename Type>
  struct MetaProp;

  template<swgtk::props::window Id>
  struct WindowProp;

  template<>
  struct MetaProp<props::meta::name, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_NAME_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::version, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_VERSION_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::id, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_IDENTIFIER_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::creator, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_CREATOR_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::copyright, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_COPYRIGHT_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::type, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_TYPE_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

  template<>
  struct MetaProp<props::meta::url, const char*> {
    constexpr MetaProp(const char* str) : _value(str) {}
    constexpr static auto id = SDL_PROP_APP_METADATA_URL_STRING;

    constexpr auto operator()() const -> const char* { return _value; }
    const char* _value{};
  };

}

namespace swgtk::props {

  using PropValue = std::variant<std::monostate, void*, std::string, std::int64_t, float, bool>;

  template<typename... T>
  struct prop_overload final : public T... {
    using T::operator()...;
  };

  namespace metadata {
    constexpr auto name = SDL_PROP_APP_METADATA_NAME_STRING;
    constexpr auto version = SDL_PROP_APP_METADATA_VERSION_STRING;
    constexpr auto id = SDL_PROP_APP_METADATA_IDENTIFIER_STRING;
    constexpr auto creater = SDL_PROP_APP_METADATA_CREATOR_STRING;
    constexpr auto copyright = SDL_PROP_APP_METADATA_COPYRIGHT_STRING;
    constexpr auto url = SDL_PROP_APP_METADATA_URL_STRING;
    constexpr auto type = SDL_PROP_APP_METADATA_TYPE_STRING;
  } // namespace metadata

  namespace winCreate {
    constexpr auto always_on_top = SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN;
    constexpr auto borderless = SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN;
    constexpr auto const_popup = SDL_PROP_WINDOW_CREATE_CONSTRAIN_POPUP_BOOLEAN;
    constexpr auto focusable = SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN;
    constexpr auto ext_context = SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN;
    constexpr auto flags = SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER;
    constexpr auto fullscreen = SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN;
    constexpr auto height = SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER;
    constexpr auto hidden = SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN;
    constexpr auto high_dpi = SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN;
    constexpr auto maximized = SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN;
    constexpr auto menu = SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN;
    constexpr auto metal = SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN;
    constexpr auto minimized = SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN;
    constexpr auto modal = SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN;
    constexpr auto mouse_grabbed = SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN;
    constexpr auto open_gl = SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN;
    constexpr auto parent = SDL_PROP_WINDOW_CREATE_PARENT_POINTER;
    constexpr auto resizable = SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN;
    constexpr auto title = SDL_PROP_WINDOW_CREATE_TITLE_STRING;
    constexpr auto transparent = SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN;
    constexpr auto tooltip = SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN;
    constexpr auto utility = SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN;
    constexpr auto vulkan = SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN;
    constexpr auto width = SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER;
    constexpr auto x = SDL_PROP_WINDOW_CREATE_X_NUMBER;
    constexpr auto y = SDL_PROP_WINDOW_CREATE_Y_NUMBER;
    constexpr auto cocoa = SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER;
    constexpr auto cocoa_view = SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER;
    constexpr auto window_scene = SDL_PROP_WINDOW_CREATE_WINDOWSCENE_POINTER;
    constexpr auto wayland_surface_role_custom = SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN;
    constexpr auto wayland_egl = SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN;
    // constexpr auto WaylandID = SDL_PROP_WINDOW_CREATE_WAYLAND_WINDOW_ID_STRING;
    constexpr auto wayland_wl_surface = SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER;
    constexpr auto win32_hwnd = SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER;
    constexpr auto win32_pixel_format = SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER;
    constexpr auto x11 = SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER;
    constexpr auto emscripten_canvas = SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_CANVAS_ID_STRING;
    constexpr auto emscripten_keyboard = SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_KEYBOARD_ELEMENT_STRING;
    // constexpr auto VisionOSSettings = SDL_PROP_WINDOW_CREATE_VISIONOS_SETTINGS_STRING;
  } // namespace winCreate

  namespace renderCreate {
    constexpr auto name = SDL_PROP_RENDERER_CREATE_NAME_STRING;
    constexpr auto window = SDL_PROP_RENDERER_CREATE_WINDOW_POINTER;
    constexpr auto surface = SDL_PROP_RENDERER_CREATE_SURFACE_POINTER;
    constexpr auto color_space = SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER;
    constexpr auto vsync = SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER;

    namespace shader {
      constexpr auto device = SDL_PROP_RENDERER_CREATE_GPU_DEVICE_POINTER;
      constexpr auto spirv = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_SPIRV_BOOLEAN;
      constexpr auto dxil = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_DXIL_BOOLEAN;
      constexpr auto msl = SDL_PROP_RENDERER_CREATE_GPU_SHADERS_MSL_BOOLEAN;
    } // namespace shader

    namespace vk {
      constexpr auto instance = SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER;
      constexpr auto surface = SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER;
      constexpr auto phyical_device = SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER;
      constexpr auto device = SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER;
      constexpr auto gfx_queue = SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER;
      constexpr auto present_queue = SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER;
    } // namespace vk

  } // namespace renderCreate

  namespace gpuCreate {
    constexpr auto debug = SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN;
    constexpr auto low_power = SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN;
    constexpr auto verbose = SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN;
    constexpr auto name = SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING;
    constexpr auto clip_dist = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN;
    constexpr auto depth_clamping = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN;
    constexpr auto indirect_draw = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN;
    constexpr auto anisotropy = SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN;

    namespace shader {
      constexpr auto priv = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_PRIVATE_BOOLEAN;
      constexpr auto spirv = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN;
      constexpr auto dxbc = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXBC_BOOLEAN;
      constexpr auto dxsil = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN;
      constexpr auto msl = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_MSL_BOOLEAN;
      constexpr auto metal = SDL_PROP_GPU_DEVICE_CREATE_SHADERS_METALLIB_BOOLEAN;
    } // namespace shader

    namespace d3d12 {
      constexpr auto semantic_prefix = SDL_PROP_GPU_DEVICE_CREATE_D3D12_SEMANTIC_NAME_STRING;
      constexpr auto few_res_slots = SDL_PROP_GPU_DEVICE_CREATE_D3D12_ALLOW_FEWER_RESOURCE_SLOTS_BOOLEAN;
      constexpr auto sdk_version = SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_VERSION_NUMBER;
      constexpr auto sdk_path = SDL_PROP_GPU_DEVICE_CREATE_D3D12_AGILITY_SDK_PATH_STRING;
    } // namespace d3d12

    namespace vk {
      constexpr auto require_hardware = SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN;
      constexpr auto options = SDL_PROP_GPU_DEVICE_CREATE_VULKAN_OPTIONS_POINTER;
    } // namespace vk

  } // namespace gpuCreate

  namespace thread {
    constexpr auto entry = SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER;
    constexpr auto name = SDL_PROP_THREAD_CREATE_NAME_STRING;
    constexpr auto user_data = SDL_PROP_THREAD_CREATE_USERDATA_POINTER;
    constexpr auto stack_size = SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER;
  } // namespace thread

  namespace ioFile {
    constexpr auto win32_handle = SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER;
    constexpr auto file = SDL_PROP_IOSTREAM_STDIO_FILE_POINTER;
    constexpr auto file_desc = SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER;
    constexpr auto android_asset = SDL_PROP_IOSTREAM_ANDROID_AASSET_POINTER;
  } // namespace ioFile

  namespace ioMem {
    constexpr auto const_pointer = SDL_PROP_IOSTREAM_MEMORY_POINTER;
    constexpr auto mem_size = SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER;
    constexpr auto deleter = SDL_PROP_IOSTREAM_MEMORY_FREE_FUNC_POINTER;
    constexpr auto chuck_size = SDL_PROP_IOSTREAM_DYNAMIC_CHUNKSIZE_NUMBER;
    constexpr auto pointer = SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER;
  } // namespace ioMem

  namespace fileDia {
    constexpr auto filters = SDL_PROP_FILE_DIALOG_FILTERS_POINTER;
    constexpr auto num_filters = SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER;
    constexpr auto window = SDL_PROP_FILE_DIALOG_WINDOW_POINTER;
    constexpr auto location = SDL_PROP_FILE_DIALOG_LOCATION_STRING;
    constexpr auto many = SDL_PROP_FILE_DIALOG_MANY_BOOLEAN;
    constexpr auto title = SDL_PROP_FILE_DIALOG_TITLE_STRING;
    constexpr auto accept = SDL_PROP_FILE_DIALOG_ACCEPT_STRING;
    constexpr auto cancel = SDL_PROP_FILE_DIALOG_CANCEL_STRING;
  } // namespace fileDia

  namespace process {
    constexpr auto args = SDL_PROP_PROCESS_CREATE_ARGS_POINTER;
    constexpr auto env = SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER;
    constexpr auto working_dir = SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING;
    constexpr auto std_in = SDL_PROP_PROCESS_CREATE_STDIN_NUMBER;
    constexpr auto std_out = SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER;
    constexpr auto std_err = SDL_PROP_PROCESS_CREATE_STDERR_NUMBER;
    constexpr auto std_in_redirect = SDL_PROP_PROCESS_CREATE_STDIN_POINTER;
    constexpr auto std_out_redirect = SDL_PROP_PROCESS_CREATE_STDOUT_POINTER;
    constexpr auto std_err_redirect = SDL_PROP_PROCESS_CREATE_STDERR_POINTER;
    constexpr auto err_to_out = SDL_PROP_PROCESS_CREATE_STDERR_TO_STDOUT_BOOLEAN;
    constexpr auto cmd = SDL_PROP_PROCESS_CREATE_CMDLINE_STRING;
    constexpr auto background = SDL_PROP_PROCESS_BACKGROUND_BOOLEAN;
  } // namespace process

} // namespace swgtk::props

#endif // !SWGTK_PROPS_HPP
