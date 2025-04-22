cmake_minimum_required(VERSION 3.31)

set(GFX_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/gfxstream)
set(AEMU_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/aemu)

add_library(
    webroguegfx
    STATIC
    webroguegfx.c
    webroguegfx_events.c
    webroguegfx_gl.cpp
    webroguegfx_stream.cpp

    ${GFX_SRC_DIR}/common/etc/etc.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/GL2Encoder.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/gl2_enc.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/IOStream2.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/GL2EncoderUtils.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/GLESv2Validation.cpp
    ${GFX_SRC_DIR}/guest/GLESv2_enc/gl2_client_context.cpp
    ${GFX_SRC_DIR}/guest/OpenglSystemCommon/ThreadInfo.cpp
    ${GFX_SRC_DIR}/guest/OpenglSystemCommon/HostConnection.cpp
    ${GFX_SRC_DIR}/guest/OpenglSystemCommon/ProcessPipe.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/GLClientState.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/glUtils.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/GLESTextureUtils.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/ChecksumCalculator.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/GLSharedGroup.cpp
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/IndexRangeCache.cpp
    
    ${GFX_SRC_DIR}/guest/android-emu/aemu/base/Tracing.cpp
)

target_include_directories(
    webroguegfx
    PRIVATE
    ${GFX_SRC_DIR}/guest/egl
    ${GFX_SRC_DIR}/common/opengl/include
    ${GFX_SRC_DIR}/guest/GLESv1_enc
    ${GFX_SRC_DIR}/guest/GLESv2_enc
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon
    ${GFX_SRC_DIR}/guest/OpenglCodecCommon/include
    ${GFX_SRC_DIR}/guest/OpenglSystemCommon
    ${GFX_SRC_DIR}/guest/android-emu
    ${GFX_SRC_DIR}/include
    ${GFX_SRC_DIR}/guest/renderControl_enc
    ${GFX_SRC_DIR}/common/etc/include
    ${AEMU_SRC_DIR}/host-common/include
    ${AEMU_SRC_DIR}/host-common/include/host-common
    ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(webroguegfx PROPERTIES PUBLIC_HEADER "webroguegfx.h")

install(
    TARGETS webroguegfx
    PUBLIC_HEADER DESTINATION include/webroguegfx
)
