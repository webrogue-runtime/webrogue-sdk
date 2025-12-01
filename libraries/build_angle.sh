set -ex
cd $(dirname $0)

if [ ! -d "angle" ] || [ ! -d "chromium-build" ]; then
    sh get_angle.sh
fi
export PATH="$(pwd)/depot_tools:$PATH"

WEBROGUEGFX_SRC=webroguegfx/webroguegfx.h
WEBROGUEGFX_DEST=angle/third_party/webroguegfx/include/webroguegfx/webroguegfx.h

if [ ! -f "$WEBROGUEGFX_DEST" ] || ! cmp -s "$WEBROGUEGFX_SRC" "$WEBROGUEGFX_DEST"; then
    cp "$WEBROGUEGFX_SRC" "$WEBROGUEGFX_DEST"
fi

VULKAN_HEADERS_DIR="$(pwd)/Vulkan-Headers"

OUT_DIR=$BUILD_DIR/angle

cd angle

case "$CONFIG" in
    Release)
        IS_DEBUG=false
        IS_OFFICIAL_BUILD=true
        EXTRA_OBJECTS=""
        ;;
    Debug)
        IS_DEBUG=true
        IS_OFFICIAL_BUILD=false
        EXTRA_OBJECTS="$OUT_DIR/obj/angle_gl_enum_utils/*.o"
        ;;
    *)
        echo "CONFIG must be Debug or Release"
        exit 1
        ;;
esac

GN_ARGS="--args=\
    target_cpu=\"wasm32\" \
    target_os=\"wasip1\" \
    is_debug=$IS_DEBUG \
    angle_build_all=false \
    angle_enable_null=false \
    angle_has_frame_capture=false \
    angle_enable_gl=false \
    angle_enable_vulkan=true \
    angle_enable_d3d9=false \
    angle_enable_d3d11=false \
    angle_enable_gl=false \
    angle_enable_null=false \
    angle_enable_metal=false \
    angle_enable_essl=false \
    angle_enable_wgpu=false \
    angle_enable_glsl=true \
    is_official_build=$IS_OFFICIAL_BUILD \
    chrome_pgo_phase=0 \
    clang_base_path=\"/wasi-sdk\" \
    clang_version=\"21\" \
    clang_use_chrome_plugins=false \
    use_custom_libcxx=false \
    angle_enable_abseil=false \
    angle_has_frame_capture=false \
    angle_enable_vulkan_validation_layers=false \
    angle_vulkan_headers_dir=\"$VULKAN_HEADERS_DIR\"
"

# # gn clean out/wasip1
gn gen $OUT_DIR "$GN_ARGS"
gn args --list $OUT_DIR "$GN_ARGS"
# # gn refs $OUT_DIR "//third_party/zlib:zlib"

ninja -C $OUT_DIR libGLESv2_static libEGL_static libANGLE_no_vulkan

cd ..

rm -f $OUT_DIR/libEGL.a

/wasi-sdk/bin/llvm-ar qLs $OUT_DIR/libEGL.a \
    $OUT_DIR/obj/libEGL_static.a \
    $OUT_DIR/obj/libGLESv2_thin_static.a \
    $OUT_DIR/obj/libangle_common.a \
    $OUT_DIR/obj/libangle_image_util.a \
    $OUT_DIR/obj/libpreprocessor.a \
    $OUT_DIR/obj/libtranslator.a \
    $OUT_DIR/obj/third_party/spirv-tools/src/libspvtools.a \
    $OUT_DIR/obj/third_party/spirv-tools/src/libspvtools_val.a \
    $OUT_DIR/obj/third_party/zlib/google/libcompression_utils_portable.a \
    $OUT_DIR/obj/third_party/zlib/libchrome_zlib.a \
    $OUT_DIR/obj/libangle_gpu_info_util.a \
    $OUT_DIR/obj/libangle_image_util.a \

/wasi-sdk/bin/llvm-ar qs $OUT_DIR/libEGL.a \
    $OUT_DIR/obj/libANGLE_no_vulkan/*.o \
    $OUT_DIR/obj/src/common/spirv/angle_spirv_builder/*.o \
    $OUT_DIR/obj/src/common/spirv/angle_spirv_parser/*.o \
    $OUT_DIR/obj/src/common/spirv/angle_spirv_base/*.o \
    $OUT_DIR/obj/src/common/vulkan/**/*.o \
    $OUT_DIR/obj/angle_common_shader_state/*.o \
    $OUT_DIR/obj/angle_version_info/*.o \
    $OUT_DIR/obj/angle_frame_capture_mock/*.o \
    $OUT_DIR/obj/angle_common/*.o \
    $OUT_DIR/obj/xxhash/*.o \
    $OUT_DIR/obj/src/libANGLE/renderer/vulkan/**/*.o \
    $OUT_DIR/obj/third_party/vulkan_memory_allocator/vulkan_memory_allocator/*.o \
    $OUT_DIR/obj/third_party/astc-encoder/astcenc/*.o \
    $OUT_DIR/obj/src/third_party/volk/volk/*.o \
    $EXTRA_OBJECTS \
