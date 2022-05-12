#ifndef __YF_MACRO_H__
#define __YF_MACRO_H__

#if defined __GNUC__ || defined __llvm__
#define YF_LIKELY(x) __builtin_expect(!!(x), 1)
#define YF_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define YF_LIKELY(x) (x)
#define YF_UNLIKELY(x) (x)
#endif

#endif
