set -ex
cd $(dirname $0)

if [ ! -d "angle" ] || [ ! -d "chromium-build" ]; then
    sh get_angle.sh
fi
export PATH="$(pwd)/depot_tools:$PATH"

mkdir -p angle/third_party/webroguegfx
cp webroguegfx/webroguegfx.h angle/third_party/webroguegfx/webroguegfx.h

VULKAN_HEADERS_DIR="$(pwd)/Vulkan-Headers"

cd angle

GN_ARGS="--args=\
    target_cpu=\"wasm32\" \
    target_os=\"wasip1\" \
    is_debug=true \
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
    is_official_build=false \
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
gn gen out/wasip1 "$GN_ARGS"
gn args --list out/wasip1 "$GN_ARGS"
# # gn refs out/wasip1 "//third_party/zlib:zlib"

ninja -C out/wasip1 libGLESv2_static libEGL_static libANGLE_no_vulkan

cd ..
rm -f build/libEGL.a
OUT_DIR=angle/out/wasip1
/wasi-sdk/bin/llvm-ar qLs build/libEGL.a $OUT_DIR/obj/libEGL_static.a $OUT_DIR/obj/libGLESv2_thin_static.a $OUT_DIR/obj/libangle_common.a $OUT_DIR/obj/libangle_image_util.a $OUT_DIR/obj/libpreprocessor.a $OUT_DIR/obj/libtranslator.a $OUT_DIR/obj/third_party/spirv-tools/src/libspvtools.a $OUT_DIR/obj/third_party/spirv-tools/src/libspvtools_val.a $OUT_DIR/obj/third_party/zlib/google/libcompression_utils_portable.a $OUT_DIR/obj/third_party/zlib/libchrome_zlib.a
/wasi-sdk/bin/llvm-ar qs build/libEGL.a $OUT_DIR/obj/libANGLE_no_vulkan/*.o $OUT_DIR/obj/src/common/spirv/angle_spirv_builder/spirv_instruction_builder_autogen.o $OUT_DIR/obj/angle_common_shader_state/*.o $OUT_DIR/obj/angle_version_info/*.o $OUT_DIR/obj/angle_frame_capture_mock/*.o $OUT_DIR/obj/angle_gl_enum_utils/*.o $OUT_DIR/obj/angle_common/*.o $OUT_DIR/obj/xxhash/*.o $OUT_DIR/obj/src/common/spirv/angle_spirv_base/*.o