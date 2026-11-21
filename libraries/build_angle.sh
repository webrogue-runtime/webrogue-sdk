set -ex
cd $(dirname $0)

export VPYTHON_VIRTUALENV_ROOT=/tmp/vpython-root
export XDG_CACHE_HOME=/tmp
export HOME=/tmp

if [ ! -d "angle" ] || [ ! -d "chromium-build" ]; then
    sh get_angle.sh
fi
export PATH="$(pwd)/depot_tools:$PATH"

LIBWR4C_INCLUDE=angle/third_party/libwr4c/include
mkdir -p "$LIBWR4C_INCLUDE/libwr4c" "$LIBWR4C_INCLUDE/dawn" "$LIBWR4C_INCLUDE/webgpu"

copy_if_changed() {
    if [ ! -f "$2" ] || ! cmp -s "$1" "$2"; then
        cp "$1" "$2"
    fi
}

copy_if_changed libwr4c/wr4c.h "$LIBWR4C_INCLUDE/libwr4c/wr4c.h"
copy_if_changed libwr4c/wr4c_webgpu.h "$LIBWR4C_INCLUDE/libwr4c/wr4c_webgpu.h"
copy_if_changed libwr4c/include/dawn/dawn_proc.h "$LIBWR4C_INCLUDE/dawn/dawn_proc.h"
copy_if_changed libwr4c/include/dawn/dawn_proc_table.h "$LIBWR4C_INCLUDE/dawn/dawn_proc_table.h"
copy_if_changed wasi-webgpu-headers/webgpu/webgpu.h "$LIBWR4C_INCLUDE/webgpu/webgpu.h"

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
    target_os=\"wasip3\" \
    is_debug=$IS_DEBUG \
    angle_build_all=false \
    angle_has_frame_capture=false \
    angle_enable_gl=false \
    angle_enable_vulkan=false \
    angle_enable_d3d9=false \
    angle_enable_d3d11=false \
    angle_enable_null=false \
    angle_enable_metal=false \
    angle_enable_essl=false \
    angle_enable_wgpu=true \
    angle_enable_glsl=true \
    is_official_build=$IS_OFFICIAL_BUILD \
    chrome_pgo_phase=0 \
    clang_base_path=\"/wasi-sdk\" \
    clang_version=\"23\" \
    clang_use_chrome_plugins=false \
    use_custom_libcxx=false \
    angle_enable_abseil=false \
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
    $OUT_DIR/obj/third_party/zlib/google/libcompression_utils_portable.a \
    $OUT_DIR/obj/third_party/zlib/libchrome_zlib.a \
    $OUT_DIR/obj/libangle_gpu_info_util.a \
    $OUT_DIR/obj/libangle_image_util.a \
    $EXTRA_OBJECTS

/wasi-sdk/bin/llvm-ar qs $OUT_DIR/libEGL.a \
    $OUT_DIR/obj/libANGLE_no_vulkan/*.o \
    $OUT_DIR/obj/src/libANGLE/renderer/wgpu/angle_wgpu_backend/*.o \
    $OUT_DIR/obj/angle_common_shader_state/*.o \
    $OUT_DIR/obj/angle_version_info/*.o \
    $OUT_DIR/obj/angle_frame_capture_mock/*.o \
    $OUT_DIR/obj/angle_common/*.o \
    $OUT_DIR/obj/xxhash/*.o \
    $OUT_DIR/obj/third_party/astc-encoder/astcenc/*.o \
    $EXTRA_OBJECTS \
