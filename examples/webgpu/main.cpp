#include <webgpu/webgpu.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

WGPUStringView toStringView(const char* str) {
    return { str, std::strlen(str) };
}

const char* computeShaderSource = R"(
@group(0) @binding(0) var<storage, read_write> data: array<f32>;

@compute @workgroup_size(4)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    data[id.x] = data[id.x] * 2.0;
}
)";

int main() {
    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);
    assert(instance);

    WGPUAdapter adapter = nullptr;
    WGPURequestAdapterOptions adapterOpts = {};
    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_WaitAnyOnly;
    adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter a, WGPUStringView, void* ud1, void*) {
        if (status == WGPURequestAdapterStatus_Success) *static_cast<WGPUAdapter*>(ud1) = a;
    };
    adapterCb.userdata1 = &adapter;

    WGPUFutureWaitInfo waitInfo = { wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCb), false };
    wgpuInstanceWaitAny(instance, 1, &waitInfo, UINT64_MAX);
    assert(adapter);

    WGPUDevice device = nullptr;
    WGPUDeviceDescriptor deviceDesc = {};
    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_WaitAnyOnly;
    deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice d, WGPUStringView, void* ud1, void*) {
        if (status == WGPURequestDeviceStatus_Success) *static_cast<WGPUDevice*>(ud1) = d;
    };
    deviceCb.userdata1 = &device;

    waitInfo = { wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb), false };
    wgpuInstanceWaitAny(instance, 1, &waitInfo, UINT64_MAX);
    assert(device);

    WGPUQueue queue = wgpuDeviceGetQueue(device);

    std::vector<float> inputData = { 1.0f, 2.0f, 3.0f, 4.0f };
    uint64_t bufferSize = inputData.size() * sizeof(float);

    WGPUBufferDescriptor storageDesc = {};
    storageDesc.size = bufferSize;
    storageDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc | WGPUBufferUsage_CopyDst;
    WGPUBuffer storageBuffer = wgpuDeviceCreateBuffer(device, &storageDesc);

    WGPUBufferDescriptor stagingDesc = {};
    stagingDesc.size = bufferSize;
    stagingDesc.usage = WGPUBufferUsage_MapRead | WGPUBufferUsage_CopyDst;
    WGPUBuffer stagingBuffer = wgpuDeviceCreateBuffer(device, &stagingDesc);

    wgpuQueueWriteBuffer(queue, storageBuffer, 0, inputData.data(), bufferSize);

    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code = toStringView(computeShaderSource);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslSource.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);

    WGPUComputePipelineDescriptor pipelineDesc = {};
    pipelineDesc.compute.module = shaderModule;
    pipelineDesc.compute.entryPoint = toStringView("main");
    WGPUComputePipeline pipeline = wgpuDeviceCreateComputePipeline(device, &pipelineDesc);

    WGPUBindGroupLayout bindGroupLayout = wgpuComputePipelineGetBindGroupLayout(pipeline, 0);

    WGPUBindGroupEntry binding = {};
    binding.binding = 0;
    binding.buffer = storageBuffer;
    binding.offset = 0;
    binding.size = bufferSize;

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = 1;
    bindGroupDesc.entries = &binding;
    WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);

    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

    WGPUComputePassDescriptor computePassDesc = {};
    WGPUComputePassEncoder computePass = wgpuCommandEncoderBeginComputePass(encoder, &computePassDesc);
    wgpuComputePassEncoderSetPipeline(computePass, pipeline);
    wgpuComputePassEncoderSetBindGroup(computePass, 0, bindGroup, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(computePass, 1, 1, 1);
    wgpuComputePassEncoderEnd(computePass);

    wgpuCommandEncoderCopyBufferToBuffer(encoder, storageBuffer, 0, stagingBuffer, 0, bufferSize);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    bool mapped = false;
    WGPUBufferMapCallbackInfo mapCb = {};
    mapCb.mode = WGPUCallbackMode_WaitAnyOnly;
    mapCb.callback = [](WGPUMapAsyncStatus status, WGPUStringView, void* ud1, void*) {
        if (status == WGPUMapAsyncStatus_Success) *static_cast<bool*>(ud1) = true;
    };
    mapCb.userdata1 = &mapped;

    waitInfo = { wgpuBufferMapAsync(stagingBuffer, WGPUMapMode_Read, 0, bufferSize, mapCb), false };
    wgpuInstanceWaitAny(instance, 1, &waitInfo, UINT64_MAX);
    assert(mapped);

    const float* outputData = static_cast<const float*>(wgpuBufferGetConstMappedRange(stagingBuffer, 0, bufferSize));
    std::cout << "GPU Output: ";
    for (size_t i = 0; i < inputData.size(); ++i) {
        std::cout << outputData[i] << " "; // Expected: 2 4 6 8
    }
    std::cout << std::endl;
    wgpuBufferUnmap(stagingBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuComputePassEncoderRelease(computePass);
    wgpuCommandEncoderRelease(encoder);
    wgpuBindGroupRelease(bindGroup);
    wgpuBindGroupLayoutRelease(bindGroupLayout);
    wgpuComputePipelineRelease(pipeline);
    wgpuShaderModuleRelease(shaderModule);
    wgpuBufferRelease(stagingBuffer);
    wgpuBufferRelease(storageBuffer);
    wgpuQueueRelease(queue);
    wgpuDeviceRelease(device);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);

    return 0;
}