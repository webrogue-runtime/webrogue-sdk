# Webrogue SDK

[![Webrogue SDK](https://github.com/webrogue-runtime/webrogue-sdk/actions/workflows/build_sdk.yml/badge.svg)](https://github.com/webrogue-runtime/webrogue-sdk/actions/workflows/build_sdk.yml)


Webrogue SDK is a variant of [WASI SDK](https://github.com/WebAssembly/wasi-sdk). It exist to simplify development of [Webrogue](https://github.com/webrogue-runtime/webrogue) applications.

> [!IMPORTANT]  
> All libraries there are built as static.

In addition to `libc` and `libc++` from WASI SDK, Webrogue SDK bundles a number of popular libraries needed to develop desktop and mobile apps and games. Most notable are:

### WebrogueGFX

A Webrogue-specific library. It provides a relatively simple API to access underlying graphics and windowing systems, and to receive UI events like clicks, keystrokes and touch events. WebrogueGFX is used by other high-level libraries like GLFW and GTK in realm of Webrogue.

### Vulkan

Mesa driver with modified GFXStream allows to do seemly impossible: use Vulkan graphics and compute API from inside of WebAssembly VM.
This driver includes WSI extension called `VK_WEBROGUE_surface` (gated behind `VK_USE_PLATFORM_WEBROGUE` macro and unavailable in upstream spec yet). ICD library is bundled as `libvulkan_gfxstream.a`, but you can use `libvulkan.a` library in addition to simplify loading of Vulkan entrypoints.

### OpenGL ES and EGL

Custom fork of Google's ANGLE translates OpenGL ES calls to Vulkan, allowing to port OpenGL ES applications to Webrogue. It is bundled as `libEGL.a` file, `-lEGL` flag links both OpenGL ES and EGL. You may also need `-Wl,--export=eglGetProcAddress` linker flag if EGL is linked weakly, as in cases of SDL and GLFW.

### GLFW

Upstream GLFW is available only for Windows, macOS, Wayland and X11. But Webrogue extends this list, allowing to port GLFW applications even to mobile devices. Both OpenGL ES and Vulkan supported.


### SDL2 and SDL3

While SDL is very portable on itself, Webrogue makes it even more portable by removing many platform-specific pitfalls. Both OpenGL ES and Vulkan can be used with for rendering. SDL_ttf libraries included.

### GTK4 and GLib

> [!WARNING]
> Many important features are still missing in Webrogue's port of GTK and GLib.

Work on porting GTK to Webrogue has begun. GTK is a big and complex thing, but there are first results on using Vulkan to run GTK applications under Webrogue. 

### Other libraries

Webrogue SDK bundles some other libraries, including:
- Cairo
- libepoxy
- Expat
- Fontconfig
- FreeType
- FriBidi
- GdkPixbuf
- Graphene
- HarfBuzz
- HarfBuzz-subset
- libffi
- libintl
- libjpeg-turbo
- libpng
- libxml2
- Mbed TLS
- Ogg
- p256-m
- Pango
- PCRE2
- Pixman
- librsvg
- LibTIFF
- Vorbis
- Zlib

## How to download

Webrogue SDK can be downloaded using [Webrogue VS Code extension](https://marketplace.visualstudio.com/items?itemName=webrogue.webrogue-vscode) or [directly from GitHub Releases](https://github.com/webrogue-runtime/webrogue-sdk/releases).

