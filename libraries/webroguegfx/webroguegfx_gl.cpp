#include "GL2Encoder.h"
#include "HostConnection.h"
#include "ThreadInfo.h"
#include <stddef.h>
#include <string.h>

extern "C" {
#include "webroguegfx_internal.h"
}

#include "gl2_entry.cpp"
#include "gl2_ftable.h"

__attribute__((import_name("gl_init")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_init_gl(uint8_t *out_status);

static thread_local PFNGLFLUSHPROC sGLFlush = nullptr;

extern "C" void webroguegfx_init_gl() {
  if (!getEGLThreadInfo()->hostConn) {
    uint8_t status = 0;
    imported_webrogue_gfx_init_gl(&status);
    if (!status) {
      return;
    }
    getEGLThreadInfo()->hostConn = HostConnection::createUnique();
    getEGLThreadInfo()->hostConn->gl2Encoder()->setClientState(
        new gfxstream::guest::GLClientState(3, 0));
    // TODO GLSharedGroup should be stored in
    // getEGLThreadInfo()->currentContext->sharedGroup
    getEGLThreadInfo()->hostConn->gl2Encoder()->setSharedGroup(
        gfxstream::guest::GLSharedGroupPtr(
            new gfxstream::guest::GLSharedGroup()));
    getEGLThreadInfo()->hostConn->gl2Encoder()->setVersion(3, 0, 3, 0);
    for (int i = 0; i < gl2_num_funcs; i++) {
      if (strcmp(gl2_funcs_by_name[i].name, "glFlush") == 0) {
        sGLFlush = (PFNGLFLUSHPROC)gl2_funcs_by_name[i].proc;
      }
    }
  }
}

extern "C" void webrogue_gfx_internal_glFlush(void) {
  if (sGLFlush) {
    sGLFlush();
  }
}

extern "C" void *webroguegfx_gl_loader(const char *procname) {
  webroguegfx_init_gl();
  for (int i = 0; i < gl2_num_funcs; i++) {
    if (strcmp(gl2_funcs_by_name[i].name, procname) == 0) {
      return gl2_funcs_by_name[i].proc;
    }
  }
  return NULL;
}
