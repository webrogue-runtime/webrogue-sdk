// The vendored wasi-webgpu-headers implementation only covers the compute
// subset of the WebGPU C API. This file adds the render-pipeline / render-pass
// / texture-view entry points that the `webgpu` example needs, so applications
// can use the standard <webgpu/webgpu.h> API.
//
// It is compiled as part of libwr4c (which already provides the wasi:webgpu
// import wrappers through webrogue_gfx.c). Including webgpu.c is the only way
// to reach its private object layouts and conversion helpers without
// duplicating them in another translation unit.

#define wgpuSurfaceRelease wgpuSurfaceReleaseUnsupported
// The vendored implementation rejects sampler and texture bindings; ANGLE's
// renderer needs them, so the full version lives below.
#define wgpuDeviceCreateBindGroup wgpuDeviceCreateBindGroupUnsupported
// The bind group layout conversion also stops at the first texture entry
// (`todo()` in `textureBindingLayoutNativeToWasi`); the full version lives
// below as well.
#define wgpuDeviceCreateBindGroupLayout wgpuDeviceCreateBindGroupLayoutUnsupported
// The vendored buffer mapping supports a single live mapped range per buffer
// and traps on the next `wgpuBufferGetMappedRange`; the full version lives
// below.
#define wgpuBufferGetMappedRange wgpuBufferGetMappedRangeUnsupported
#define wgpuBufferGetConstMappedRange wgpuBufferGetConstMappedRangeUnsupported
#include "../wasi-webgpu-headers/webgpu.c"
#undef wgpuSurfaceRelease
#undef wgpuDeviceCreateBindGroup
#undef wgpuDeviceCreateBindGroupLayout
#undef wgpuBufferGetMappedRange
#undef wgpuBufferGetConstMappedRange

#include "wr4c_webgpu.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Helpers normally provided by wasi-webgpu-headers/imports.c
//
// We cannot link imports.c: it also defines the wasi:webgpu import wrappers,
// which webrogue_gfx.c already provides for libwr4c.
// ---------------------------------------------------------------------------

void imports_list_u8_free(imports_list_u8_t *ptr) {
    if (ptr->len > 0) {
        free(ptr->ptr);
    }
    ptr->ptr = NULL;
    ptr->len = 0;
}

void imports_string_dup(imports_string_t *ret, const char *s) {
    ret->len = strlen(s);
    ret->ptr = (uint8_t *)malloc(ret->len);
    if (!ret->ptr) oom();
    memcpy(ret->ptr, s, ret->len);
}

void imports_string_free(imports_string_t *ret) {
    if (ret->len > 0) {
        free(ret->ptr);
    }
    ret->ptr = NULL;
    ret->len = 0;
}

// ---------------------------------------------------------------------------
// Enum helpers
//
// The webgpu.h enums reserve 0 for `Undefined`/`None`, while the
// wasi:webgpu enums start at 0. Bitmask enums (usages, color write mask)
// have the same values in both and are cast directly.
// ---------------------------------------------------------------------------

static uint8_t enumNativeToWasi(uint32_t value) { return (uint8_t)(value - 1); }

static imports_option_bool_t optionalBoolNativeToWasi(WGPUOptionalBool value) {
    imports_option_bool_t out = {};
    switch (value) {
    case WGPUOptionalBool_False:
        out.is_some = true;
        out.val = false;
        break;
    case WGPUOptionalBool_True:
        out.is_some = true;
        out.val = true;
        break;
    default:
        break;
    }
    return out;
}

static wasi_webgpu_webgpu_option_gpu_blend_operation_t
blendOperationNativeToWasi(WGPUBlendOperation operation) {
    wasi_webgpu_webgpu_option_gpu_blend_operation_t out = {};
    out.is_some = operation != WGPUBlendOperation_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_blend_operation_t)enumNativeToWasi(operation);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_blend_factor_t
blendFactorNativeToWasi(WGPUBlendFactor factor) {
    wasi_webgpu_webgpu_option_gpu_blend_factor_t out = {};
    out.is_some = factor != WGPUBlendFactor_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_blend_factor_t)enumNativeToWasi(factor);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_vertex_step_mode_t
vertexStepModeNativeToWasi(WGPUVertexStepMode mode) {
    wasi_webgpu_webgpu_option_gpu_vertex_step_mode_t out = {};
    out.is_some = mode != WGPUVertexStepMode_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_vertex_step_mode_t)enumNativeToWasi(mode);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_compare_function_t
compareFunctionNativeToWasi(WGPUCompareFunction function) {
    wasi_webgpu_webgpu_option_gpu_compare_function_t out = {};
    out.is_some = function != WGPUCompareFunction_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_compare_function_t)enumNativeToWasi(function);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_stencil_operation_t
stencilOperationNativeToWasi(WGPUStencilOperation operation) {
    wasi_webgpu_webgpu_option_gpu_stencil_operation_t out = {};
    out.is_some = operation != WGPUStencilOperation_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_stencil_operation_t)enumNativeToWasi(operation);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_load_op_t loadOpNativeToWasi(WGPULoadOp loadOp) {
    wasi_webgpu_webgpu_option_gpu_load_op_t out = {};
    out.is_some = loadOp != WGPULoadOp_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_load_op_t)enumNativeToWasi(loadOp);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_store_op_t storeOpNativeToWasi(WGPUStoreOp storeOp) {
    wasi_webgpu_webgpu_option_gpu_store_op_t out = {};
    out.is_some = storeOp != WGPUStoreOp_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_store_op_t)enumNativeToWasi(storeOp);
    return out;
}

// ---------------------------------------------------------------------------
// Render pipeline descriptor conversion
// ---------------------------------------------------------------------------

static wasi_webgpu_webgpu_option_own_record_gpu_pipeline_constant_value_t
constantsNativeToWasi(size_t count, WGPUConstantEntry const *constants) {
    wasi_webgpu_webgpu_option_own_record_gpu_pipeline_constant_value_t out = {};
    out.is_some = count > 0;
    if (!out.is_some) {
        return out;
    }
    if (!constants) unreachable();

    out.val = wasi_webgpu_webgpu_constructor_record_gpu_pipeline_constant_value();
    for (size_t i = 0; i < count; i++) {
        imports_string_t key = stringNativeToWasi(&constants[i].key);
        wasi_webgpu_webgpu_method_record_gpu_pipeline_constant_value_add(
            wasi_webgpu_webgpu_borrow_record_gpu_pipeline_constant_value(out.val),
            &key, constants[i].value);
        imports_string_free(&key);
    }
    return out;
}

static wasi_webgpu_webgpu_gpu_vertex_state_t
vertexStateNativeToWasi(WGPUVertexState const *state) {
    if (!state->module) unreachable();

    wasi_webgpu_webgpu_gpu_vertex_state_t out = {};
    out.module = wasi_webgpu_webgpu_borrow_gpu_shader_module(state->module->module);
    out.entry_point = optionalStringNativeToWasi(&state->entryPoint);
    out.constants = constantsNativeToWasi(state->constantCount, state->constants);

    if (state->bufferCount == 0) {
        return out;
    }
    if (!state->buffers) unreachable();

    out.buffers.is_some = true;
    out.buffers.val.len = state->bufferCount;
    out.buffers.val.ptr = calloc(
        state->bufferCount,
        sizeof(wasi_webgpu_webgpu_option_gpu_vertex_buffer_layout_t));
    if (!out.buffers.val.ptr) oom();

    for (size_t i = 0; i < state->bufferCount; i++) {
        WGPUVertexBufferLayout const *layout = &state->buffers[i];
        wasi_webgpu_webgpu_option_gpu_vertex_buffer_layout_t *entry =
            &out.buffers.val.ptr[i];
        entry->is_some = true;
        entry->val.array_stride = layout->arrayStride;
        entry->val.step_mode = vertexStepModeNativeToWasi(layout->stepMode);

        if (layout->attributeCount == 0) {
            continue;
        }
        if (!layout->attributes) unreachable();

        entry->val.attributes.len = layout->attributeCount;
        entry->val.attributes.ptr = calloc(
            layout->attributeCount,
            sizeof(wasi_webgpu_webgpu_gpu_vertex_attribute_t));
        if (!entry->val.attributes.ptr) oom();

        for (size_t j = 0; j < layout->attributeCount; j++) {
            WGPUVertexAttribute const *attribute = &layout->attributes[j];
            entry->val.attributes.ptr[j] =
                (wasi_webgpu_webgpu_gpu_vertex_attribute_t){
                    .format = (wasi_webgpu_webgpu_gpu_vertex_format_t)
                        enumNativeToWasi(attribute->format),
                    .offset = attribute->offset,
                    .shader_location = attribute->shaderLocation,
                };
        }
    }
    return out;
}

static wasi_webgpu_webgpu_gpu_blend_component_t
blendComponentNativeToWasi(WGPUBlendComponent const *component) {
    wasi_webgpu_webgpu_gpu_blend_component_t out = {};
    out.operation = blendOperationNativeToWasi(component->operation);
    out.src_factor = blendFactorNativeToWasi(component->srcFactor);
    out.dst_factor = blendFactorNativeToWasi(component->dstFactor);
    return out;
}

static wasi_webgpu_webgpu_gpu_color_target_state_t
colorTargetStateNativeToWasi(WGPUColorTargetState const *target) {
    wasi_webgpu_webgpu_gpu_color_target_state_t out = {};
    out.format = (wasi_webgpu_webgpu_gpu_texture_format_t)
        enumNativeToWasi(target->format);
    out.write_mask.is_some = true;
    out.write_mask.val = (wasi_webgpu_webgpu_gpu_color_write_t)target->writeMask;

    if (target->blend) {
        out.blend.is_some = true;
        out.blend.val.color = blendComponentNativeToWasi(&target->blend->color);
        out.blend.val.alpha = blendComponentNativeToWasi(&target->blend->alpha);
    }
    return out;
}

static wasi_webgpu_webgpu_gpu_fragment_state_t
fragmentStateNativeToWasi(WGPUFragmentState const *state) {
    if (!state->module) unreachable();

    wasi_webgpu_webgpu_gpu_fragment_state_t out = {};
    out.module = wasi_webgpu_webgpu_borrow_gpu_shader_module(state->module->module);
    out.entry_point = optionalStringNativeToWasi(&state->entryPoint);
    out.constants = constantsNativeToWasi(state->constantCount, state->constants);

    if (state->targetCount == 0) {
        return out;
    }
    if (!state->targets) unreachable();

    out.targets.len = state->targetCount;
    out.targets.ptr = calloc(
        state->targetCount,
        sizeof(wasi_webgpu_webgpu_option_gpu_color_target_state_t));
    if (!out.targets.ptr) oom();

    for (size_t i = 0; i < state->targetCount; i++) {
        WGPUColorTargetState const *target = &state->targets[i];
        out.targets.ptr[i].is_some = target->format != WGPUTextureFormat_Undefined;
        if (out.targets.ptr[i].is_some) {
            out.targets.ptr[i].val = colorTargetStateNativeToWasi(target);
        }
    }
    return out;
}

static wasi_webgpu_webgpu_gpu_primitive_state_t
primitiveStateNativeToWasi(WGPUPrimitiveState const *state) {
    wasi_webgpu_webgpu_gpu_primitive_state_t out = {};
    out.topology.is_some = state->topology != WGPUPrimitiveTopology_Undefined;
    out.topology.val = (wasi_webgpu_webgpu_gpu_primitive_topology_t)
        enumNativeToWasi(state->topology);
    out.strip_index_format.is_some =
        state->stripIndexFormat != WGPUIndexFormat_Undefined;
    out.strip_index_format.val = (wasi_webgpu_webgpu_gpu_index_format_t)
        enumNativeToWasi(state->stripIndexFormat);
    out.front_face.is_some = state->frontFace != WGPUFrontFace_Undefined;
    out.front_face.val = (wasi_webgpu_webgpu_gpu_front_face_t)
        enumNativeToWasi(state->frontFace);
    out.cull_mode.is_some = state->cullMode != WGPUCullMode_Undefined;
    out.cull_mode.val = (wasi_webgpu_webgpu_gpu_cull_mode_t)
        enumNativeToWasi(state->cullMode);
    out.unclipped_depth.is_some = true;
    out.unclipped_depth.val = state->unclippedDepth != 0;
    return out;
}

static wasi_webgpu_webgpu_gpu_stencil_face_state_t
stencilFaceStateNativeToWasi(WGPUStencilFaceState const *face) {
    wasi_webgpu_webgpu_gpu_stencil_face_state_t out = {};
    out.compare = compareFunctionNativeToWasi(face->compare);
    out.fail_op = stencilOperationNativeToWasi(face->failOp);
    out.depth_fail_op = stencilOperationNativeToWasi(face->depthFailOp);
    out.pass_op = stencilOperationNativeToWasi(face->passOp);
    return out;
}

static wasi_webgpu_webgpu_gpu_depth_stencil_state_t
depthStencilStateNativeToWasi(WGPUDepthStencilState const *state) {
    wasi_webgpu_webgpu_gpu_depth_stencil_state_t out = {};
    out.format = (wasi_webgpu_webgpu_gpu_texture_format_t)
        enumNativeToWasi(state->format);
    out.depth_write_enabled = optionalBoolNativeToWasi(state->depthWriteEnabled);
    out.depth_compare = compareFunctionNativeToWasi(state->depthCompare);
    out.stencil_front.is_some = true;
    out.stencil_front.val = stencilFaceStateNativeToWasi(&state->stencilFront);
    out.stencil_back.is_some = true;
    out.stencil_back.val = stencilFaceStateNativeToWasi(&state->stencilBack);
    out.stencil_read_mask.is_some = state->stencilReadMask != 0xFFFFFFFF;
    out.stencil_read_mask.val = state->stencilReadMask;
    out.stencil_write_mask.is_some = state->stencilWriteMask != 0xFFFFFFFF;
    out.stencil_write_mask.val = state->stencilWriteMask;
    out.depth_bias.is_some = state->depthBias != 0;
    out.depth_bias.val = state->depthBias;
    out.depth_bias_slope_scale.is_some = state->depthBiasSlopeScale != 0.0f;
    out.depth_bias_slope_scale.val = state->depthBiasSlopeScale;
    out.depth_bias_clamp.is_some = state->depthBiasClamp != 0.0f;
    out.depth_bias_clamp.val = state->depthBiasClamp;
    return out;
}

static wasi_webgpu_webgpu_gpu_multisample_state_t
multisampleStateNativeToWasi(WGPUMultisampleState const *state) {
    wasi_webgpu_webgpu_gpu_multisample_state_t out = {};
    out.count.is_some = state->count != 1;
    out.count.val = state->count;
    out.mask.is_some = state->mask != 0xFFFFFFFF;
    out.mask.val = state->mask;
    out.alpha_to_coverage_enabled.is_some = true;
    out.alpha_to_coverage_enabled.val = state->alphaToCoverageEnabled != 0;
    return out;
}

WGPURenderPipeline wgpuDeviceCreateRenderPipeline(
    WGPUDevice device, WGPURenderPipelineDescriptor const *descriptor) {
    if (!device || !descriptor) unreachable();
    if (!descriptor->vertex.module) unreachable();

    wasi_webgpu_webgpu_gpu_render_pipeline_descriptor_t wasi = {};
    wasi.vertex = vertexStateNativeToWasi(&descriptor->vertex);

    wasi.primitive.is_some = true;
    wasi.primitive.val = primitiveStateNativeToWasi(&descriptor->primitive);

    if (descriptor->depthStencil) {
        wasi.depth_stencil.is_some = true;
        wasi.depth_stencil.val =
            depthStencilStateNativeToWasi(descriptor->depthStencil);
    }

    wasi.multisample.is_some = true;
    wasi.multisample.val = multisampleStateNativeToWasi(&descriptor->multisample);

    if (descriptor->fragment) {
        wasi.fragment.is_some = true;
        wasi.fragment.val = fragmentStateNativeToWasi(descriptor->fragment);
    }

    if (descriptor->layout) {
        wasi.layout.tag = WASI_WEBGPU_WEBGPU_GPU_LAYOUT_MODE_SPECIFIC;
        wasi.layout.val.specific = wasi_webgpu_webgpu_borrow_gpu_pipeline_layout(
            descriptor->layout->pipeline_layout);
    } else {
        wasi.layout.tag = WASI_WEBGPU_WEBGPU_GPU_LAYOUT_MODE_AUTO;
    }

    wasi.label = optionalStringNativeToWasi(&descriptor->label);

    wasi_webgpu_webgpu_own_gpu_render_pipeline_t pipeline =
        wasi_webgpu_webgpu_method_gpu_device_create_render_pipeline(
            wasi_webgpu_webgpu_borrow_gpu_device(device->device), &wasi);
    wasi_webgpu_webgpu_gpu_render_pipeline_descriptor_free(&wasi);

    WGPURenderPipelineImpl *pipeline_struct = malloc(sizeof(WGPURenderPipelineImpl));
    if (!pipeline_struct) oom();
    pipeline_struct->render_pipeline = pipeline;
    pipeline_struct->refCount = 1;
    return pipeline_struct;
}

WGPUBindGroupLayout wgpuRenderPipelineGetBindGroupLayout(
    WGPURenderPipeline renderPipeline, uint32_t groupIndex) {
    if (!renderPipeline) unreachable();

    wasi_webgpu_webgpu_own_gpu_bind_group_layout_t bind_group_layout =
        wasi_webgpu_webgpu_method_gpu_render_pipeline_get_bind_group_layout(
            wasi_webgpu_webgpu_borrow_gpu_render_pipeline(
                renderPipeline->render_pipeline),
            groupIndex);

    WGPUBindGroupLayoutImpl *layout_struct =
        malloc(sizeof(WGPUBindGroupLayoutImpl));
    if (!layout_struct) oom();
    layout_struct->bind_group_layout = bind_group_layout;
    layout_struct->refCount = 1;
    return layout_struct;
}

// ---------------------------------------------------------------------------
// Texture views
// ---------------------------------------------------------------------------

WGPUTextureView wgpuTextureCreateView(
    WGPUTexture texture, WGPUTextureViewDescriptor const *descriptor) {
    if (!texture) unreachable();

    wasi_webgpu_webgpu_gpu_texture_view_descriptor_t wasi = {};
    if (descriptor) {
        wasi.format.is_some = descriptor->format != WGPUTextureFormat_Undefined;
        wasi.format.val = (wasi_webgpu_webgpu_gpu_texture_format_t)
            enumNativeToWasi(descriptor->format);

        wasi.dimension.is_some =
            descriptor->dimension != WGPUTextureViewDimension_Undefined;
        wasi.dimension.val = (wasi_webgpu_webgpu_gpu_texture_view_dimension_t)
            enumNativeToWasi(descriptor->dimension);

        wasi.usage.is_some = descriptor->usage != WGPUTextureUsage_None;
        wasi.usage.val =
            (wasi_webgpu_webgpu_gpu_texture_usage_t)descriptor->usage;

        wasi.aspect.is_some = descriptor->aspect != WGPUTextureAspect_Undefined;
        wasi.aspect.val = (wasi_webgpu_webgpu_gpu_texture_aspect_t)
            enumNativeToWasi(descriptor->aspect);

        wasi.base_mip_level.is_some = true;
        wasi.base_mip_level.val = descriptor->baseMipLevel;

        wasi.mip_level_count.is_some =
            descriptor->mipLevelCount != WGPU_MIP_LEVEL_COUNT_UNDEFINED;
        wasi.mip_level_count.val = descriptor->mipLevelCount;

        wasi.base_array_layer.is_some = true;
        wasi.base_array_layer.val = descriptor->baseArrayLayer;

        wasi.array_layer_count.is_some =
            descriptor->arrayLayerCount != WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
        wasi.array_layer_count.val = descriptor->arrayLayerCount;

        wasi.swizzle.is_some = false;
        wasi.label = optionalStringNativeToWasi(&descriptor->label);
    }

    wasi_webgpu_webgpu_own_gpu_texture_view_t texture_view =
        wasi_webgpu_webgpu_method_gpu_texture_create_view(
            wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture),
            descriptor ? &wasi : NULL);
    if (descriptor) {
        wasi_webgpu_webgpu_gpu_texture_view_descriptor_free(&wasi);
    }

    WGPUTextureViewImpl *view_struct = malloc(sizeof(WGPUTextureViewImpl));
    if (!view_struct) oom();
    view_struct->texture_view = texture_view;
    view_struct->refCount = 1;
    return view_struct;
}

// ---------------------------------------------------------------------------
// Render passes
// ---------------------------------------------------------------------------

static void colorAttachmentNativeToWasi(
    WGPURenderPassColorAttachment const *attachment,
    wasi_webgpu_webgpu_option_gpu_render_pass_color_attachment_t *out) {
    out->is_some = attachment->view != NULL;
    if (!out->is_some) {
        return;
    }

    out->val.view =
        wasi_webgpu_webgpu_borrow_gpu_texture_view(attachment->view->texture_view);

    out->val.depth_slice.is_some =
        attachment->depthSlice != WGPU_DEPTH_SLICE_UNDEFINED;
    out->val.depth_slice.val = attachment->depthSlice;

    if (attachment->resolveTarget) {
        out->val.resolve_target.is_some = true;
        out->val.resolve_target.val = wasi_webgpu_webgpu_borrow_gpu_texture_view(
            attachment->resolveTarget->texture_view);
    }

    out->val.clear_value.is_some = true;
    out->val.clear_value.val = (wasi_webgpu_webgpu_gpu_color_t){
        .r = attachment->clearValue.r,
        .g = attachment->clearValue.g,
        .b = attachment->clearValue.b,
        .a = attachment->clearValue.a,
    };
    out->val.load_op = (wasi_webgpu_webgpu_gpu_load_op_t)
        enumNativeToWasi(attachment->loadOp);
    out->val.store_op = (wasi_webgpu_webgpu_gpu_store_op_t)
        enumNativeToWasi(attachment->storeOp);
}

static void depthStencilAttachmentNativeToWasi(
    WGPURenderPassDepthStencilAttachment const *attachment,
    wasi_webgpu_webgpu_option_gpu_render_pass_depth_stencil_attachment_t *out) {
    out->is_some = true;
    out->val.view =
        wasi_webgpu_webgpu_borrow_gpu_texture_view(attachment->view->texture_view);
    out->val.depth_load_op = loadOpNativeToWasi(attachment->depthLoadOp);
    out->val.depth_store_op = storeOpNativeToWasi(attachment->depthStoreOp);
    out->val.depth_read_only.is_some = true;
    out->val.depth_read_only.val = attachment->depthReadOnly != 0;
    // WebGPU requires `depthClearValue` to be a finite number, while the C
    // header uses NAN to mean "unspecified". Only forward it when the depth
    // buffer is actually cleared; otherwise leave it unset so the browser
    // applies its own default (1.0).
    if (attachment->depthLoadOp == WGPULoadOp_Clear &&
        isfinite(attachment->depthClearValue)) {
        out->val.depth_clear_value.is_some = true;
        out->val.depth_clear_value.val = attachment->depthClearValue;
    }
    out->val.stencil_load_op = loadOpNativeToWasi(attachment->stencilLoadOp);
    out->val.stencil_store_op = storeOpNativeToWasi(attachment->stencilStoreOp);
    out->val.stencil_read_only.is_some = true;
    out->val.stencil_read_only.val = attachment->stencilReadOnly != 0;
    if (attachment->stencilLoadOp == WGPULoadOp_Clear) {
        out->val.stencil_clear_value.is_some = true;
        out->val.stencil_clear_value.val = attachment->stencilClearValue;
    }
}

WGPURenderPassEncoder wgpuCommandEncoderBeginRenderPass(
    WGPUCommandEncoder commandEncoder, WGPURenderPassDescriptor const *descriptor) {
    if (!commandEncoder || !descriptor) unreachable();

    wasi_webgpu_webgpu_gpu_render_pass_descriptor_t wasi = {};

    if (descriptor->colorAttachmentCount > 0) {
        if (!descriptor->colorAttachments) unreachable();
        wasi.color_attachments.len = descriptor->colorAttachmentCount;
        wasi.color_attachments.ptr = calloc(
            descriptor->colorAttachmentCount,
            sizeof(wasi_webgpu_webgpu_option_gpu_render_pass_color_attachment_t));
        if (!wasi.color_attachments.ptr) oom();
        for (size_t i = 0; i < descriptor->colorAttachmentCount; i++) {
            colorAttachmentNativeToWasi(
                &descriptor->colorAttachments[i], &wasi.color_attachments.ptr[i]);
        }
    }

    if (descriptor->depthStencilAttachment) {
        depthStencilAttachmentNativeToWasi(descriptor->depthStencilAttachment,
                                           &wasi.depth_stencil_attachment);
    }

    if (descriptor->occlusionQuerySet) {
        wasi.occlusion_query_set.is_some = true;
        wasi.occlusion_query_set.val = wasi_webgpu_webgpu_borrow_gpu_query_set(
            descriptor->occlusionQuerySet->query_set);
    }

    if (descriptor->timestampWrites) {
        WGPUPassTimestampWrites const *writes = descriptor->timestampWrites;
        wasi.timestamp_writes.is_some = true;
        wasi.timestamp_writes.val.query_set =
            wasi_webgpu_webgpu_borrow_gpu_query_set(writes->querySet->query_set);
        wasi.timestamp_writes.val.beginning_of_pass_write_index.is_some =
            writes->beginningOfPassWriteIndex != WGPU_QUERY_SET_INDEX_UNDEFINED;
        wasi.timestamp_writes.val.beginning_of_pass_write_index.val =
            writes->beginningOfPassWriteIndex;
        wasi.timestamp_writes.val.end_of_pass_write_index.is_some =
            writes->endOfPassWriteIndex != WGPU_QUERY_SET_INDEX_UNDEFINED;
        wasi.timestamp_writes.val.end_of_pass_write_index.val =
            writes->endOfPassWriteIndex;
    }

    wasi.max_draw_count.is_some = false;
    wasi.label = optionalStringNativeToWasi(&descriptor->label);

    wasi_webgpu_webgpu_own_gpu_render_pass_encoder_t render_pass =
        wasi_webgpu_webgpu_method_gpu_command_encoder_begin_render_pass(
            wasi_webgpu_webgpu_borrow_gpu_command_encoder(
                commandEncoder->command_encoder),
            &wasi);
    wasi_webgpu_webgpu_gpu_render_pass_descriptor_free(&wasi);

    WGPURenderPassEncoderImpl *pass_struct =
        malloc(sizeof(WGPURenderPassEncoderImpl));
    if (!pass_struct) oom();
    pass_struct->render_pass_encoder = render_pass;
    pass_struct->refCount = 1;
    return pass_struct;
}

void wgpuRenderPassEncoderSetPipeline(
    WGPURenderPassEncoder renderPassEncoder, WGPURenderPipeline pipeline) {
    if (!renderPassEncoder || !pipeline) unreachable();
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_pipeline(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        wasi_webgpu_webgpu_borrow_gpu_render_pipeline(
            pipeline->render_pipeline));
}

void wgpuRenderPassEncoderSetBindGroup(
    WGPURenderPassEncoder renderPassEncoder, uint32_t groupIndex,
    WGPUBindGroup bindGroup, size_t dynamicOffsetCount,
    uint32_t const *dynamicOffsets) {
    if (!renderPassEncoder) unreachable();

    wasi_webgpu_webgpu_borrow_gpu_bind_group_t bind_group_borrow = {};
    wasi_webgpu_webgpu_borrow_gpu_bind_group_t *bind_group_ptr = NULL;
    if (bindGroup) {
        bind_group_borrow =
            wasi_webgpu_webgpu_borrow_gpu_bind_group(bindGroup->bind_group);
        bind_group_ptr = &bind_group_borrow;
    }

    wasi_webgpu_webgpu_list_gpu_buffer_dynamic_offset_t offsets = {};
    wasi_webgpu_webgpu_list_gpu_buffer_dynamic_offset_t *offsets_ptr = NULL;
    if (dynamicOffsetCount > 0) {
        if (!dynamicOffsets) unreachable();
        offsets.ptr =
            (wasi_webgpu_webgpu_gpu_buffer_dynamic_offset_t *)dynamicOffsets;
        offsets.len = dynamicOffsetCount;
        offsets_ptr = &offsets;
    }

    wasi_webgpu_webgpu_set_bind_group_error_t error;
    bool success = wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_bind_group(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        groupIndex, bind_group_ptr, offsets_ptr, NULL, NULL, &error);
    if (!success) {
        wasi_webgpu_webgpu_set_bind_group_error_free(&error);
        todo();
    }
}

void wgpuRenderPassEncoderSetVertexBuffer(
    WGPURenderPassEncoder renderPassEncoder, uint32_t slot, WGPUBuffer buffer,
    uint64_t offset, uint64_t size) {
    if (!renderPassEncoder) unreachable();

    wasi_webgpu_webgpu_borrow_gpu_buffer_t buffer_borrow = {};
    wasi_webgpu_webgpu_borrow_gpu_buffer_t *buffer_ptr = NULL;
    if (buffer) {
        buffer_borrow = wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer);
        buffer_ptr = &buffer_borrow;
    }

    wasi_webgpu_webgpu_gpu_size64_t offset_value = offset;
    wasi_webgpu_webgpu_gpu_size64_t size_value = size;
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_vertex_buffer(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        slot, buffer_ptr, &offset_value,
        size == WGPU_WHOLE_SIZE ? NULL : &size_value);
}

void wgpuRenderPassEncoderDraw(
    WGPURenderPassEncoder renderPassEncoder, uint32_t vertexCount,
    uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    if (!renderPassEncoder) unreachable();

    wasi_webgpu_webgpu_gpu_size32_t instance_count = instanceCount;
    wasi_webgpu_webgpu_gpu_size32_t first_vertex = firstVertex;
    wasi_webgpu_webgpu_gpu_size32_t first_instance = firstInstance;
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_draw(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        vertexCount, instanceCount == 0 ? NULL : &instance_count,
        firstVertex == 0 ? NULL : &first_vertex,
        firstInstance == 0 ? NULL : &first_instance);
}

void wgpuRenderPassEncoderEnd(WGPURenderPassEncoder renderPassEncoder) {
    if (!renderPassEncoder) unreachable();
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_end(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder));
}

// ---------------------------------------------------------------------------
// Extra helpers exposed through wr4c
// ---------------------------------------------------------------------------

WGPUTextureFormat wr4c_webgpu_preferred_format(WGPUInstance instance) {
    if (!instance) unreachable();
    wasi_webgpu_webgpu_gpu_texture_format_t format =
        wasi_webgpu_webgpu_method_gpu_get_preferred_canvas_format(
            wasi_webgpu_webgpu_borrow_gpu(instance->gpu));
    return (WGPUTextureFormat)(format + 1);
}

// ---------------------------------------------------------------------------
// Textures and samplers
// ---------------------------------------------------------------------------

static wasi_webgpu_webgpu_gpu_extent3_d_t
extent3DNativeToWasi(WGPUExtent3D const *extent) {
    wasi_webgpu_webgpu_gpu_extent3_d_t out = {};
    out.width = extent->width;
    out.height.is_some = true;
    out.height.val = extent->height;
    out.depth_or_array_layers.is_some = true;
    out.depth_or_array_layers.val = extent->depthOrArrayLayers;
    return out;
}

WGPUTexture wgpuDeviceCreateTexture(WGPUDevice device,
                                    WGPUTextureDescriptor const *descriptor) {
    if (!device || !descriptor) unreachable();

    wasi_webgpu_webgpu_gpu_texture_descriptor_t wasi = {};
    wasi.size = extent3DNativeToWasi(&descriptor->size);
    wasi.mip_level_count.is_some = true;
    wasi.mip_level_count.val = descriptor->mipLevelCount;
    wasi.sample_count.is_some = true;
    wasi.sample_count.val = descriptor->sampleCount;
    wasi.dimension.is_some =
        descriptor->dimension != WGPUTextureDimension_Undefined;
    wasi.dimension.val = (wasi_webgpu_webgpu_gpu_texture_dimension_t)
        enumNativeToWasi(descriptor->dimension);
    wasi.format = (wasi_webgpu_webgpu_gpu_texture_format_t)
        enumNativeToWasi(descriptor->format);
    wasi.usage = (wasi_webgpu_webgpu_gpu_texture_usage_t)descriptor->usage;

    if (descriptor->viewFormatCount > 0) {
        if (!descriptor->viewFormats) unreachable();
        wasi.view_formats.is_some = true;
        wasi.view_formats.val.len = descriptor->viewFormatCount;
        wasi.view_formats.val.ptr = calloc(
            descriptor->viewFormatCount,
            sizeof(wasi_webgpu_webgpu_gpu_texture_format_t));
        if (!wasi.view_formats.val.ptr) oom();
        for (size_t i = 0; i < descriptor->viewFormatCount; i++) {
            wasi.view_formats.val.ptr[i] =
                (wasi_webgpu_webgpu_gpu_texture_format_t)
                    enumNativeToWasi(descriptor->viewFormats[i]);
        }
    }

    wasi.texture_binding_view_dimension.is_some = false;
    wasi.label = optionalStringNativeToWasi(&descriptor->label);

    wasi_webgpu_webgpu_own_gpu_texture_t texture =
        wasi_webgpu_webgpu_method_gpu_device_create_texture(
            wasi_webgpu_webgpu_borrow_gpu_device(device->device), &wasi);
    wasi_webgpu_webgpu_gpu_texture_descriptor_free(&wasi);

    WGPUTextureImpl *texture_struct = malloc(sizeof(WGPUTextureImpl));
    if (!texture_struct) oom();
    texture_struct->texture = texture;
    texture_struct->refCount = 1;
    return texture_struct;
}

static wasi_webgpu_webgpu_option_gpu_address_mode_t
addressModeNativeToWasi(WGPUAddressMode mode) {
    wasi_webgpu_webgpu_option_gpu_address_mode_t out = {};
    out.is_some = mode != WGPUAddressMode_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_address_mode_t)enumNativeToWasi(mode);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_filter_mode_t
filterModeNativeToWasi(WGPUFilterMode mode) {
    wasi_webgpu_webgpu_option_gpu_filter_mode_t out = {};
    out.is_some = mode != WGPUFilterMode_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_filter_mode_t)enumNativeToWasi(mode);
    return out;
}

static wasi_webgpu_webgpu_option_gpu_mipmap_filter_mode_t
mipmapFilterModeNativeToWasi(WGPUMipmapFilterMode mode) {
    wasi_webgpu_webgpu_option_gpu_mipmap_filter_mode_t out = {};
    out.is_some = mode != WGPUMipmapFilterMode_Undefined;
    out.val = (wasi_webgpu_webgpu_gpu_mipmap_filter_mode_t)
        enumNativeToWasi(mode);
    return out;
}

WGPUSampler wgpuDeviceCreateSampler(WGPUDevice device,
                                    WGPUSamplerDescriptor const *descriptor) {
    if (!device) unreachable();

    wasi_webgpu_webgpu_gpu_sampler_descriptor_t wasi = {};
    if (descriptor) {
        wasi.address_mode_u = addressModeNativeToWasi(descriptor->addressModeU);
        wasi.address_mode_v = addressModeNativeToWasi(descriptor->addressModeV);
        wasi.address_mode_w = addressModeNativeToWasi(descriptor->addressModeW);
        wasi.mag_filter = filterModeNativeToWasi(descriptor->magFilter);
        wasi.min_filter = filterModeNativeToWasi(descriptor->minFilter);
        wasi.mipmap_filter =
            mipmapFilterModeNativeToWasi(descriptor->mipmapFilter);
        wasi.lod_min_clamp.is_some = true;
        wasi.lod_min_clamp.val = descriptor->lodMinClamp;
        wasi.lod_max_clamp.is_some = true;
        wasi.lod_max_clamp.val = descriptor->lodMaxClamp;
        wasi.compare = compareFunctionNativeToWasi(descriptor->compare);
        wasi.max_anisotropy.is_some = descriptor->maxAnisotropy != 1;
        wasi.max_anisotropy.val = descriptor->maxAnisotropy;
        wasi.label = optionalStringNativeToWasi(&descriptor->label);
    }

    wasi_webgpu_webgpu_own_gpu_sampler_t sampler =
        wasi_webgpu_webgpu_method_gpu_device_create_sampler(
            wasi_webgpu_webgpu_borrow_gpu_device(device->device),
            descriptor ? &wasi : NULL);
    if (descriptor) {
        wasi_webgpu_webgpu_gpu_sampler_descriptor_free(&wasi);
    }

    WGPUSamplerImpl *sampler_struct = malloc(sizeof(WGPUSamplerImpl));
    if (!sampler_struct) oom();
    sampler_struct->sampler = sampler;
    sampler_struct->refCount = 1;
    return sampler_struct;
}

uint32_t wgpuTextureGetWidth(WGPUTexture texture) {
    if (!texture) unreachable();
    return wasi_webgpu_webgpu_method_gpu_texture_width(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

uint32_t wgpuTextureGetHeight(WGPUTexture texture) {
    if (!texture) unreachable();
    return wasi_webgpu_webgpu_method_gpu_texture_height(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

uint32_t wgpuTextureGetDepthOrArrayLayers(WGPUTexture texture) {
    if (!texture) unreachable();
    return wasi_webgpu_webgpu_method_gpu_texture_depth_or_array_layers(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

uint32_t wgpuTextureGetMipLevelCount(WGPUTexture texture) {
    if (!texture) unreachable();
    return wasi_webgpu_webgpu_method_gpu_texture_mip_level_count(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

uint32_t wgpuTextureGetSampleCount(WGPUTexture texture) {
    if (!texture) unreachable();
    return wasi_webgpu_webgpu_method_gpu_texture_sample_count(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

WGPUTextureDimension wgpuTextureGetDimension(WGPUTexture texture) {
    if (!texture) unreachable();
    return (WGPUTextureDimension)(
        wasi_webgpu_webgpu_method_gpu_texture_dimension(
            wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture)) +
        1);
}

WGPUTextureFormat wgpuTextureGetFormat(WGPUTexture texture) {
    if (!texture) unreachable();
    return (WGPUTextureFormat)(
        wasi_webgpu_webgpu_method_gpu_texture_format(
            wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture)) +
        1);
}

WGPUTextureUsage wgpuTextureGetUsage(WGPUTexture texture) {
    if (!texture) unreachable();
    return (WGPUTextureUsage)wasi_webgpu_webgpu_method_gpu_texture_usage(
        wasi_webgpu_webgpu_borrow_gpu_texture(texture->texture));
}

// ---------------------------------------------------------------------------
// Copy commands
// ---------------------------------------------------------------------------

static wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t
texelCopyTextureInfoNativeToWasi(WGPUTexelCopyTextureInfo const *info) {
    if (!info->texture) unreachable();
    wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t out = {};
    out.texture =
        wasi_webgpu_webgpu_borrow_gpu_texture(info->texture->texture);
    out.mip_level.is_some = true;
    out.mip_level.val = info->mipLevel;
    out.origin.is_some = true;
    out.origin.val.x.is_some = true;
    out.origin.val.x.val = info->origin.x;
    out.origin.val.y.is_some = true;
    out.origin.val.y.val = info->origin.y;
    out.origin.val.z.is_some = true;
    out.origin.val.z.val = info->origin.z;
    out.aspect.is_some = info->aspect != WGPUTextureAspect_Undefined;
    out.aspect.val =
        (wasi_webgpu_webgpu_gpu_texture_aspect_t)enumNativeToWasi(info->aspect);
    return out;
}

static wasi_webgpu_webgpu_gpu_texel_copy_buffer_info_t
texelCopyBufferInfoNativeToWasi(WGPUTexelCopyBufferInfo const *info) {
    if (!info->buffer) unreachable();
    wasi_webgpu_webgpu_gpu_texel_copy_buffer_info_t out = {};
    out.buffer = wasi_webgpu_webgpu_borrow_gpu_buffer(info->buffer->buffer);
    out.offset.is_some = true;
    out.offset.val = info->layout.offset;
    out.bytes_per_row.is_some =
        info->layout.bytesPerRow != WGPU_COPY_STRIDE_UNDEFINED;
    out.bytes_per_row.val = info->layout.bytesPerRow;
    out.rows_per_image.is_some =
        info->layout.rowsPerImage != WGPU_COPY_STRIDE_UNDEFINED;
    out.rows_per_image.val = info->layout.rowsPerImage;
    return out;
}

void wgpuCommandEncoderCopyBufferToTexture(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyBufferInfo const *source,
    WGPUTexelCopyTextureInfo const *destination,
    WGPUExtent3D const *copySize) {
    if (!commandEncoder || !source || !destination || !copySize) unreachable();
    wasi_webgpu_webgpu_gpu_texel_copy_buffer_info_t source_wasi =
        texelCopyBufferInfoNativeToWasi(source);
    wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t destination_wasi =
        texelCopyTextureInfoNativeToWasi(destination);
    wasi_webgpu_webgpu_gpu_extent3_d_t copy_size_wasi =
        extent3DNativeToWasi(copySize);
    wasi_webgpu_webgpu_method_gpu_command_encoder_copy_buffer_to_texture(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(
            commandEncoder->command_encoder),
        &source_wasi, &destination_wasi, &copy_size_wasi);
}

void wgpuCommandEncoderCopyTextureToBuffer(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyTextureInfo const *source,
    WGPUTexelCopyBufferInfo const *destination,
    WGPUExtent3D const *copySize) {
    if (!commandEncoder || !source || !destination || !copySize) unreachable();
    wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t source_wasi =
        texelCopyTextureInfoNativeToWasi(source);
    wasi_webgpu_webgpu_gpu_texel_copy_buffer_info_t destination_wasi =
        texelCopyBufferInfoNativeToWasi(destination);
    wasi_webgpu_webgpu_gpu_extent3_d_t copy_size_wasi =
        extent3DNativeToWasi(copySize);
    wasi_webgpu_webgpu_method_gpu_command_encoder_copy_texture_to_buffer(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(
            commandEncoder->command_encoder),
        &source_wasi, &destination_wasi, &copy_size_wasi);
}

void wgpuCommandEncoderCopyTextureToTexture(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyTextureInfo const *source,
    WGPUTexelCopyTextureInfo const *destination,
    WGPUExtent3D const *copySize) {
    if (!commandEncoder || !source || !destination || !copySize) unreachable();
    wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t source_wasi =
        texelCopyTextureInfoNativeToWasi(source);
    wasi_webgpu_webgpu_gpu_texel_copy_texture_info_t destination_wasi =
        texelCopyTextureInfoNativeToWasi(destination);
    wasi_webgpu_webgpu_gpu_extent3_d_t copy_size_wasi =
        extent3DNativeToWasi(copySize);
    wasi_webgpu_webgpu_method_gpu_command_encoder_copy_texture_to_texture(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(
            commandEncoder->command_encoder),
        &source_wasi, &destination_wasi, &copy_size_wasi);
}

// ---------------------------------------------------------------------------
// Render pass encoder state
// ---------------------------------------------------------------------------

void wgpuRenderPassEncoderSetViewport(WGPURenderPassEncoder renderPassEncoder,
                                      float x, float y, float width,
                                      float height, float minDepth,
                                      float maxDepth) {
    if (!renderPassEncoder) unreachable();
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_viewport(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        x, y, width, height, minDepth, maxDepth);
}

void wgpuRenderPassEncoderSetScissorRect(WGPURenderPassEncoder renderPassEncoder,
                                         uint32_t x, uint32_t y, uint32_t width,
                                         uint32_t height) {
    if (!renderPassEncoder) unreachable();
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_scissor_rect(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        x, y, width, height);
}

void wgpuRenderPassEncoderSetBlendConstant(
    WGPURenderPassEncoder renderPassEncoder, WGPUColor const *color) {
    if (!renderPassEncoder || !color) unreachable();
    wasi_webgpu_webgpu_gpu_color_t wasi = {
        .r = color->r,
        .g = color->g,
        .b = color->b,
        .a = color->a,
    };
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_blend_constant(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        &wasi);
}

void wgpuRenderPassEncoderSetStencilReference(
    WGPURenderPassEncoder renderPassEncoder, uint32_t reference) {
    if (!renderPassEncoder) unreachable();
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_stencil_reference(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        reference);
}

void wgpuRenderPassEncoderSetIndexBuffer(
    WGPURenderPassEncoder renderPassEncoder, WGPUBuffer buffer,
    WGPUIndexFormat format, uint64_t offset, uint64_t size) {
    if (!renderPassEncoder || !buffer) unreachable();
    wasi_webgpu_webgpu_gpu_size64_t offset_value = offset;
    wasi_webgpu_webgpu_gpu_size64_t size_value = size;
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_set_index_buffer(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
        (wasi_webgpu_webgpu_gpu_index_format_t)enumNativeToWasi(format),
        &offset_value, size == WGPU_WHOLE_SIZE ? NULL : &size_value);
}

void wgpuRenderPassEncoderDrawIndexed(WGPURenderPassEncoder renderPassEncoder,
                                      uint32_t indexCount,
                                      uint32_t instanceCount,
                                      uint32_t firstIndex, int32_t baseVertex,
                                      uint32_t firstInstance) {
    if (!renderPassEncoder) unreachable();
    wasi_webgpu_webgpu_gpu_size32_t instance_count = instanceCount;
    wasi_webgpu_webgpu_gpu_size32_t first_index = firstIndex;
    wasi_webgpu_webgpu_gpu_signed_offset32_t base_vertex = baseVertex;
    wasi_webgpu_webgpu_gpu_size32_t first_instance = firstInstance;
    wasi_webgpu_webgpu_method_gpu_render_pass_encoder_draw_indexed(
        wasi_webgpu_webgpu_borrow_gpu_render_pass_encoder(
            renderPassEncoder->render_pass_encoder),
        indexCount, instanceCount == 0 ? NULL : &instance_count,
        firstIndex == 0 ? NULL : &first_index,
        baseVertex == 0 ? NULL : &base_vertex,
        firstInstance == 0 ? NULL : &first_instance);
}

// ---------------------------------------------------------------------------
// Bind groups with sampler and texture-view bindings
// ---------------------------------------------------------------------------

WGPUBindGroup wgpuDeviceCreateBindGroup(WGPUDevice device,
                                        WGPUBindGroupDescriptor const *descriptor) {
    if (!device || !descriptor) unreachable();
    if (descriptor->entryCount > 0 && !descriptor->entries) unreachable();

    wasi_webgpu_webgpu_gpu_bind_group_entry_t *entries_array =
        malloc(descriptor->entryCount * sizeof(*entries_array));
    if (descriptor->entryCount > 0 && !entries_array) oom();

    for (size_t i = 0; i < descriptor->entryCount; i++) {
        WGPUBindGroupEntry const *entry = &descriptor->entries[i];
        wasi_webgpu_webgpu_gpu_binding_resource_t resource = {};
        if (entry->buffer) {
            resource.tag =
                WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_BUFFER_BINDING;
            resource.val.gpu_buffer_binding =
                (wasi_webgpu_webgpu_gpu_buffer_binding_t){
                    .buffer = wasi_webgpu_webgpu_borrow_gpu_buffer(
                        entry->buffer->buffer),
                    .offset = {.is_some = true, .val = entry->offset},
                    .size = {.is_some = entry->size != WGPU_WHOLE_SIZE,
                             .val = entry->size},
                };
        } else if (entry->sampler) {
            resource.tag =
                WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_SAMPLER;
            resource.val.gpu_sampler = wasi_webgpu_webgpu_borrow_gpu_sampler(
                entry->sampler->sampler);
        } else if (entry->textureView) {
            resource.tag =
                WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_TEXTURE_VIEW;
            resource.val.gpu_texture_view =
                wasi_webgpu_webgpu_borrow_gpu_texture_view(
                    entry->textureView->texture_view);
        } else {
            unreachable();
        }

        entries_array[i] = (wasi_webgpu_webgpu_gpu_bind_group_entry_t){
            .binding = entry->binding,
            .resource = resource,
        };
    }

    wasi_webgpu_webgpu_gpu_bind_group_descriptor_t wasi = {
        .layout = wasi_webgpu_webgpu_borrow_gpu_bind_group_layout(
            descriptor->layout->bind_group_layout),
        .entries = {.ptr = entries_array, .len = descriptor->entryCount},
        .label = optionalStringNativeToWasi(&descriptor->label),
    };

    wasi_webgpu_webgpu_own_gpu_bind_group_t bind_group =
        wasi_webgpu_webgpu_method_gpu_device_create_bind_group(
            wasi_webgpu_webgpu_borrow_gpu_device(device->device), &wasi);
    wasi_webgpu_webgpu_gpu_bind_group_descriptor_free(&wasi);
    free(entries_array);

    WGPUBindGroupImpl *bind_group_struct = malloc(sizeof(WGPUBindGroupImpl));
    if (!bind_group_struct) oom();
    bind_group_struct->bind_group = bind_group;
    bind_group_struct->refCount = 1;
    return bind_group_struct;
}

// ---------------------------------------------------------------------------
// Bind group layouts with texture and storage-texture bindings
//
// The vendored conversion helpers call `todo()` as soon as an entry has a
// texture binding (the layout of every shader with a sampler hits this).
// ---------------------------------------------------------------------------

static wasi_webgpu_webgpu_option_gpu_texture_binding_layout_t
textureBindingLayoutNativeToWasiExt(WGPUTextureBindingLayout const *layout) {
    wasi_webgpu_webgpu_option_gpu_texture_binding_layout_t wasi = {};
    if (layout->sampleType == WGPUTextureSampleType_BindingNotUsed) return wasi;

    wasi.is_some = true;
    wasi.val.sample_type.is_some = layout->sampleType != WGPUTextureSampleType_Undefined;
    wasi.val.sample_type.val =
        (wasi_webgpu_webgpu_gpu_texture_sample_type_t)(layout->sampleType - 2);
    wasi.val.view_dimension.is_some =
        layout->viewDimension != WGPUTextureViewDimension_Undefined;
    wasi.val.view_dimension.val =
        (wasi_webgpu_webgpu_gpu_texture_view_dimension_t)(layout->viewDimension - 1);
    wasi.val.multisampled =
        (imports_option_bool_t){.is_some = true, .val = layout->multisampled != 0};
    return wasi;
}

static wasi_webgpu_webgpu_option_gpu_storage_texture_binding_layout_t
storageTextureBindingLayoutNativeToWasiExt(
    WGPUStorageTextureBindingLayout const *layout) {
    wasi_webgpu_webgpu_option_gpu_storage_texture_binding_layout_t wasi = {};
    if (layout->access == WGPUStorageTextureAccess_BindingNotUsed) return wasi;

    wasi.is_some = true;
    wasi.val.access.is_some = layout->access != WGPUStorageTextureAccess_Undefined;
    wasi.val.access.val =
        (wasi_webgpu_webgpu_gpu_storage_texture_access_t)(layout->access - 2);
    wasi.val.format =
        (wasi_webgpu_webgpu_gpu_texture_format_t)enumNativeToWasi(layout->format);
    wasi.val.view_dimension.is_some =
        layout->viewDimension != WGPUTextureViewDimension_Undefined;
    wasi.val.view_dimension.val =
        (wasi_webgpu_webgpu_gpu_texture_view_dimension_t)(layout->viewDimension - 1);
    return wasi;
}

WGPUBindGroupLayout wgpuDeviceCreateBindGroupLayout(
    WGPUDevice device, WGPUBindGroupLayoutDescriptor const *descriptor) {
    if (!device || !descriptor) unreachable();
    if (descriptor->entryCount > 0 && !descriptor->entries) unreachable();

    wasi_webgpu_webgpu_list_gpu_bind_group_layout_entry_t entries_wasi = {
        .ptr = malloc(descriptor->entryCount *
                      sizeof(wasi_webgpu_webgpu_gpu_bind_group_layout_entry_t)),
        .len = descriptor->entryCount,
    };
    if (descriptor->entryCount > 0 && !entries_wasi.ptr) oom();

    for (size_t i = 0; i < descriptor->entryCount; i++) {
        WGPUBindGroupLayoutEntry const *entry = &descriptor->entries[i];
        // TODO: binding arrays
        if (entry->bindingArraySize > 1) todo();

        entries_wasi.ptr[i] = (wasi_webgpu_webgpu_gpu_bind_group_layout_entry_t){
            .binding = entry->binding,
            // WGPUShaderStage uses the same bits as gpu-shader-stage
            .visibility = (wasi_webgpu_webgpu_gpu_shader_stage_t)entry->visibility,
            .buffer = bufferBindingLayoutNativeToWasi(&entry->buffer),
            .sampler = samplerBindingLayoutNativeToWasi(&entry->sampler),
            .texture = textureBindingLayoutNativeToWasiExt(&entry->texture),
            .storage_texture =
                storageTextureBindingLayoutNativeToWasiExt(&entry->storageTexture),
        };
    }

    wasi_webgpu_webgpu_gpu_bind_group_layout_descriptor_t descriptor_wasi = {
        .entries = entries_wasi,
        .label = optionalStringNativeToWasi(&descriptor->label),
    };

    wasi_webgpu_webgpu_own_gpu_bind_group_layout_t bind_group_layout =
        wasi_webgpu_webgpu_method_gpu_device_create_bind_group_layout(
            wasi_webgpu_webgpu_borrow_gpu_device(device->device), &descriptor_wasi);

    wasi_webgpu_webgpu_gpu_bind_group_layout_descriptor_free(&descriptor_wasi);

    WGPUBindGroupLayoutImpl *bind_group_layout_struct =
        malloc(sizeof(WGPUBindGroupLayoutImpl));
    if (!bind_group_layout_struct) oom();
    bind_group_layout_struct->bind_group_layout = bind_group_layout;
    bind_group_layout_struct->refCount = 1;
    return bind_group_layout_struct;
}

// ---------------------------------------------------------------------------
// Shader module compilation info
// ---------------------------------------------------------------------------

typedef struct {
    WGPUCompilationInfoCallbackInfo cb;
} CompilationInfoCtx;

static uint32_t callGetCompilationInfo(void *args, void *result) {
    return wasi_webgpu_webgpu_method_gpu_shader_module_get_compilation_info(
        *(wasi_webgpu_webgpu_borrow_gpu_shader_module_t *)args, result);
}

static void onGetCompilationInfoDone(void *result_untyped, void *userdata) {
    wasi_webgpu_webgpu_own_gpu_compilation_info_t *info = result_untyped;
    CompilationInfoCtx *ctx = userdata;

    wasi_webgpu_webgpu_list_own_gpu_compilation_message_t messages = {};
    wasi_webgpu_webgpu_method_gpu_compilation_info_messages(
        wasi_webgpu_webgpu_borrow_gpu_compilation_info(*info), &messages);

    WGPUCompilationMessage *out = NULL;
    imports_string_t *strings = NULL;
    if (messages.len > 0) {
        out = calloc(messages.len, sizeof(*out));
        strings = calloc(messages.len, sizeof(*strings));
        if (!out || !strings) oom();

        for (size_t i = 0; i < messages.len; i++) {
            wasi_webgpu_webgpu_borrow_gpu_compilation_message_t message =
                wasi_webgpu_webgpu_borrow_gpu_compilation_message(
                    messages.ptr[i]);
            wasi_webgpu_webgpu_method_gpu_compilation_message_message(
                message, &strings[i]);
            out[i].nextInChain = NULL;
            out[i].message =
                (WGPUStringView){.data = (char *)strings[i].ptr,
                                 .length = strings[i].len};
            out[i].type = (WGPUCompilationMessageType)(
                wasi_webgpu_webgpu_method_gpu_compilation_message_type(
                    message) +
                1);
            out[i].lineNum =
                wasi_webgpu_webgpu_method_gpu_compilation_message_line_num(
                    message);
            out[i].linePos =
                wasi_webgpu_webgpu_method_gpu_compilation_message_line_pos(
                    message);
            out[i].offset =
                wasi_webgpu_webgpu_method_gpu_compilation_message_offset(
                    message);
            out[i].length =
                wasi_webgpu_webgpu_method_gpu_compilation_message_length(
                    message);
        }
    }

    WGPUCompilationInfo compilationInfo = {
        .nextInChain = NULL,
        .messageCount = messages.len,
        .messages = out,
    };
    ctx->cb.callback(WGPUCompilationInfoRequestStatus_Success,
                     &compilationInfo, ctx->cb.userdata1, ctx->cb.userdata2);

    for (size_t i = 0; i < messages.len; i++) {
        imports_string_free(&strings[i]);
    }
    free(out);
    free(strings);
    wasi_webgpu_webgpu_list_own_gpu_compilation_message_free(&messages);
    wasi_webgpu_webgpu_gpu_compilation_info_drop_own(*info);
}

WGPUFuture wgpuShaderModuleGetCompilationInfo(
    WGPUShaderModule shaderModule,
    WGPUCompilationInfoCallbackInfo callbackInfo) {
    if (!shaderModule) unreachable();
    validateCallbackMode(callbackInfo.mode);

    CompilationInfoCtx *ctx = malloc(sizeof(*ctx));
    if (!ctx) oom();
    ctx->cb = callbackInfo;

    wasi_webgpu_webgpu_borrow_gpu_shader_module_t args =
        wasi_webgpu_webgpu_borrow_gpu_shader_module(shaderModule->module);
    return (WGPUFuture){.id = async_call((async_call_desc){
        .call = &callGetCompilationInfo,
        .args = &args,
        .args_size = sizeof args,
        .result_size =
            sizeof(wasi_webgpu_webgpu_own_gpu_compilation_info_t),
        .on_done = &onGetCompilationInfoDone,
        .userdata = ctx,
        .tag = (uint32_t)callbackInfo.mode,
    })};
}

// ---------------------------------------------------------------------------
// Dawn extensions and handle types that the wasi:webgpu API does not expose
// ---------------------------------------------------------------------------

WGPUInstance wgpuAdapterGetInstance(WGPUAdapter adapter) {
    // Only used when an embedder supplies a device; Webrogue always creates
    // its own device, so there is no instance to hand back.
    (void)adapter;
    return NULL;
}

WGPUAdapter wgpuDeviceGetAdapter(WGPUDevice device) {
    (void)device;
    return NULL;
}

WGPUSurface wgpuInstanceCreateSurface(WGPUInstance instance,
                                      WGPUSurfaceDescriptor const *descriptor) {
    // Surfaces are created from wr4c windows through
    // wr4c_create_webgpu_surface().
    (void)instance;
    (void)descriptor;
    unreachable();
}

void wgpuExternalTextureAddRef(WGPUExternalTexture texture) { (void)texture; }
void wgpuExternalTextureRelease(WGPUExternalTexture texture) { (void)texture; }

// ---------------------------------------------------------------------------
// Buffer mapped ranges
//
// The vendored `getMappedRange` keeps at most one live range per buffer and
// calls `todo()` when a second range is requested for the same mapping. ANGLE
// keeps buffers mapped and asks for a pointer again on every
// `glBufferSubData`, so instead of trapping, flush the previous range back to
// the buffer before fetching a new copy.
// ---------------------------------------------------------------------------

static void flushBufferMappedRange(WGPUBuffer buffer) {
    if (!buffer->mapping) return;

    if (buffer->map_mode & WASI_WEBGPU_WEBGPU_GPU_MAP_MODE_WRITE) {
        wasi_webgpu_webgpu_get_mapped_range_error_t err_set;
        bool success_set = wasi_webgpu_webgpu_method_gpu_buffer_get_mapped_range_set_with_copy(
            wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
            buffer->mapping,
            &buffer->offset,
            NULL,  // copy up to the end of the mapped range
            &err_set);
        if (!success_set) {
            wasi_webgpu_webgpu_get_mapped_range_error_free(&err_set);
            unreachable();
        }
    }

    freeBufferMapping(buffer);
}

static void* getMappedRangeExt(WGPUBuffer buffer, size_t offset, size_t size, bool writable) {
    if (!buffer) unreachable();

    // https://github.com/webgpu-native/webgpu-headers/blob/main/doc/articles/BufferMapping.md#mapped-range-behavior
    if (wgpuBufferGetMapState(buffer) != WGPUBufferMapState_Mapped) return NULL;
    if (writable && !(buffer->map_mode & WASI_WEBGPU_WEBGPU_GPU_MAP_MODE_WRITE)) return NULL;

    flushBufferMappedRange(buffer);

    wasi_webgpu_webgpu_gpu_size64_t offset_wasi = (wasi_webgpu_webgpu_gpu_size64_t)offset;
    wasi_webgpu_webgpu_gpu_size64_t size_wasi = (wasi_webgpu_webgpu_gpu_size64_t)size;

    imports_list_u8_t* mapping = malloc(sizeof(imports_list_u8_t));
    if (!mapping) oom();
    wasi_webgpu_webgpu_get_mapped_range_error_t err;

    bool success = wasi_webgpu_webgpu_method_gpu_buffer_get_mapped_range_get_with_copy(
        wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
        &offset_wasi,
        size == WGPU_WHOLE_MAP_SIZE ? NULL : &size_wasi,
        mapping,
        &err);

    if (!success) {
        wasi_webgpu_webgpu_get_mapped_range_error_free(&err);
        unreachable();
    }

    buffer->mapping = mapping;
    buffer->offset = offset;
    return mapping->ptr;
}

void const* wgpuBufferGetConstMappedRange(WGPUBuffer buffer, size_t offset, size_t size) {
    return getMappedRangeExt(buffer, offset, size, false);
}

void* wgpuBufferGetMappedRange(WGPUBuffer buffer, size_t offset, size_t size) {
    return getMappedRangeExt(buffer, offset, size, true);
}

