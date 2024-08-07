#ifndef YCOR_ASSERT_OVERRIDE_H_
#define YCOR_ASSERT_OVERRIDE_H_

#include "ycoroutine.h"

#ifdef __cplusplus
extern "C" {
#endif


#define likely(expr)                ycor_likely(expr)
#define unlikely(expr)              ycor_unlikely(expr)
#define assert(expr)               ycor_assert(expr)
#define assertptr(ptr)              ycor_assert_ptr(ptr)
#define assert_alloc_bool(expr)     ycor_assert_alloc_bool(expr)
#define assert_alloc_ptr(ptr)       ycor_assert_alloc_ptr(ptr)


#ifdef __cplusplus
}
#endif

#endif



