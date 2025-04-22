#include "GL2Encoder.h"
#include "HostConnection.h"
#include "ThreadInfo.h"
#include <stddef.h>
#include <string.h>

// clang-format off

#include "gl2_entry.cpp"
#include "gl2_ftable.h"


__attribute__((import_name("gl_init")))
__attribute__((import_module("webrogue_gfx")))
void imported_init_gl();

extern "C" void* webroguegfx_gl_loader(const char* procname) {
  if(!getEGLThreadInfo()->hostConn) {
    imported_init_gl();
    getEGLThreadInfo()->hostConn = HostConnection::createUnique();
    getEGLThreadInfo()->hostConn->gl2Encoder()->setClientState(
      new gfxstream::guest::GLClientState(2, 0)
    );
    // TODO GLSharedGroup should be stored in getEGLThreadInfo()->currentContext->sharedGroup
    getEGLThreadInfo()->hostConn->gl2Encoder()->setSharedGroup(
      gfxstream::guest::GLSharedGroupPtr(
        new gfxstream::guest::GLSharedGroup()
      )
    );
    getEGLThreadInfo()->hostConn->gl2Encoder()->setVersion(
      3, 0,
      3, 0
    );
  }
  for(int i = 0; i<gl2_num_funcs; i++) {
    if(strcmp(gl2_funcs_by_name[i].name, procname) == 0) {
      return gl2_funcs_by_name[i].proc;
    }
  }
  return NULL;
}
