/*
* Copyright (C) 2016 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ProcessPipe.h"

#include <errno.h>
#define ALOGE(...)
#define ALOGW(...)
#define ALOGV(...)
#define ALOGD(...)
#include <pthread.h>

#include "HostConnection.h"

#ifndef __Fuchsia__

static int sStreamHandle = -1;

#endif  // !__Fuchsia__
// sProcUID is a unique ID per process assigned by the host.
// It is different from getpid().
static uint64_t           sProcUID = 0;
static HostConnectionType sConnType = HOST_CONNECTION_VIRTIO_GPU_PIPE;

namespace {

static std::mutex sNeedInitMutex;
static bool sNeedInit = true;

}  // namespace

static void processPipeDoInit(uint32_t noRenderControlEnc) {
    // No need to setup auxiliary pipe stream in this case
    if (noRenderControlEnc) return;

    ALOGE("Fuchsia: requires noRenderControlEnc");
    abort();
}

bool processPipeInit(int streamHandle, HostConnectionType connType, uint32_t noRenderControlEnc) {
    sConnType = connType;
#ifndef __Fuchsia__
    sStreamHandle = streamHandle;
#endif // !__Fuchsia

    {
        std::lock_guard<std::mutex> lock(sNeedInitMutex);

        if (sNeedInit) {
            sNeedInit = false;
            processPipeDoInit(noRenderControlEnc);

            if (noRenderControlEnc) {
                return true;
            }
        }
    }

    return true;
}

uint64_t getPuid() { return sProcUID; }
