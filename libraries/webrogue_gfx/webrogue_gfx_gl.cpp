#include "GL2Encoder.h"
#include "HostConnection.h"
#include "ThreadInfo.h"
#include <stddef.h>
#include <string.h>

// clang-format off

#include "gl2_entry.cpp"
#include "gl2_ftable.h"

extern "C" void *__cxa_allocate_exception(size_t thrown_size) throw() {
    printf("__cxa_allocate_exception\n");
    size_t actual_size = thrown_size+1024;
    void* result = malloc(actual_size);
    ::memset(result, 0, actual_size);
    
    return ((char*)result)+512;
}

extern "C" void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *)) {
    printf("__cxa_throw\n");
    __builtin_unreachable();
    abort();
}

__attribute__((import_name("gl_init")))
__attribute__((import_module("webrogue_gfx")))
void imported_init_gl();

extern "C" void* webrogueGLLoader(const char* procname) {
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
