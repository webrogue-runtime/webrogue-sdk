meson setup --reconfigure build/mesa mesa -Dgallium-drivers= -Dvulkan-drivers=gfxstream -Dplatforms=webrogue -Dglx=disabled -Dllvm=disabled -Degl-native-platform=surfaceless -Dgles1=disabled -Dgles2=disabled -Dgbm=disabled -Dexpat=disabled -Degl=disabled -Dglvnd=disabled -Dzlib=disabled -Dshader-cache=disabled --cross-file=../opt/wasip1/meson_cross.txt
ninja -C build/mesa \
    src/vulkan/runtime/libvulkan_runtime.a \
    src/vulkan/runtime/libvulkan_instance.a \
    src/gfxstream/guest/vulkan/libvulkan_gfxstream.a

# src/gtest/libgtest.a: STATIC_LINKER
# src/util/libxmlconfig.a: STATIC_LINKER
# src/util/libparson.a: STATIC_LINKER
# src/compiler/spirv/spirv2nir: cpp_LINKER
# src/compiler/isaspec/libisaspec.a: STATIC_LINKER
# src/vulkan/vulkan_api.def: CUSTOM_COMMAND
# src/vulkan/util/libvulkan_util.a: STATIC_LINKER
# src/vulkan/runtime/libvulkan_lite_runtime.a: STATIC_LINKER
# src/vulkan/runtime/libvulkan_lite_instance.a: STATIC_LINKER
# src/vulkan/runtime/libvulkan_runtime.a: STATIC_LINKER
# src/vulkan/runtime/libvulkan_instance.a: STATIC_LINKER
# src/vulkan/wsi/libvulkan_wsi.a: STATIC_LINKER

# ./src/gfxstream/guest/platform/libplatform_virtgpu.a
# ./src/gfxstream/guest/platform/kumquat/libplatform_virtgpu_kumquat.a
# ./src/gfxstream/guest/GoldfishAddressSpace/libgoldfish_address_space.a
# ./src/gfxstream/guest/connection-manager/libconnection_manager.a
# ./src/gfxstream/guest/vulkan/libvulkan_gfxstream.a
# ./src/gfxstream/guest/vulkan/stubs/libgfxstream_vk_stubs.a
# ./src/gfxstream/guest/vulkan-mapper/libgfxstream_vulkan_mapper.a
# ./src/gfxstream/aemu/libaemu.a