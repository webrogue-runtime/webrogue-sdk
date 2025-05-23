// Generated Code - DO NOT EDIT !!
// generated by 'emugen'

#ifndef GUARD_gl_encoder_context_t
#define GUARD_gl_encoder_context_t

#include "gfxstream/guest/ChecksumCalculator.h"
#include "render-utils/IOStream.h"
#include "gl_client_context.h"


#include "glUtils.h"
#include "GLEncoderUtils.h"

struct gl_encoder_context_t : public gl_client_context_t {

	gfxstream::IOStream *m_stream;
	gfxstream::guest::ChecksumCalculator *m_checksumCalculator;

	gl_encoder_context_t(gfxstream::IOStream *stream, gfxstream::guest::ChecksumCalculator *checksumCalculator);
	virtual uint64_t lockAndWriteDma(void*, uint32_t) { return 0; }
};

#endif  // GUARD_gl_encoder_context_t
