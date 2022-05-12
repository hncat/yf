#ifndef __YF_FCONTEXT_H__
#define __YF_FCONTEXT_H__

#include <inttypes.h>
#include <stdlib.h>

namespace yf {
typedef void *fcontext_t;
extern "C" {
intptr_t jump_fcontext(fcontext_t *ofc, fcontext_t nfc, intptr_t vp, bool preserve_fpu = false);
fcontext_t make_fcontext(void *sp, std::size_t size, void (*fn)(intptr_t));
};
}

#endif
