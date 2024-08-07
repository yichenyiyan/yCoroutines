#include <stdio.h>
#include <stdint.h>

#include "ycoroutine.h"

// this header including should be at the last of the 'include' directives list
#include "ycor_assert_override.h"


void ycor_runtime_test(void) {
#ifdef __i386__
    _Static_assert(sizeof(void*) == 4, "require 'sizeof(void*) == 4'");
#elif  __x86_64__
    _Static_assert(sizeof(void*) == 8, "require 'sizeof(void*) == 8'");
    _Static_assert(sizeof(__uint128_t) == 16, "require 'sizeof(__uint128_t) == 16'");
#else
    #error "This platform is not currently supported"
#endif

    _Static_assert(sizeof(int) >= 4, "require 'sizeof(int) >= 4'");
    assert(sizeof(int) >= 4);
    _Static_assert(sizeof(int) <= sizeof(size_t), 
        "require 'sizeof(int) <= sizeof(size_t)'");
    assert(sizeof(int) <= sizeof(size_t));
}


// Note: dst and src must be valid address already
#define ycor_amd64_inline_short_aligned_memcpy_test_ok(dst, src, sz) \
    ( \
        (((uintptr_t)(src) & 0x0f) == 0) && (((uintptr_t)(dst) & 0x0f) == 0) \
        && \
        (((sz) & 0x0f) == 0x08) && (((sz) >> 4) >= 0) && (((sz) >> 4) <= 8) \
    )


// Note: dst and src must be valid address already
#define ycor_amd64_inline_short_aligned_memcpy(dst, src, sz) \
    do { \
        __uint128_t xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7; \
        switch((sz) >> 4){ \
        case 0:  \
            break;  \
        case 1:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            break;  \
        case 2:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            break;  \
        case 3:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            break;  \
        case 4:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            xmm3 = *((__uint128_t*)(src) + 3);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            *((__uint128_t*)(dst) + 3) = xmm3; \
            break;  \
        case 5:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            xmm3 = *((__uint128_t*)(src) + 3);  \
            xmm4 = *((__uint128_t*)(src) + 4);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            *((__uint128_t*)(dst) + 3) = xmm3; \
            *((__uint128_t*)(dst) + 4) = xmm4; \
            break;  \
        case 6:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            xmm3 = *((__uint128_t*)(src) + 3);  \
            xmm4 = *((__uint128_t*)(src) + 4);  \
            xmm5 = *((__uint128_t*)(src) + 5);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            *((__uint128_t*)(dst) + 3) = xmm3; \
            *((__uint128_t*)(dst) + 4) = xmm4; \
            *((__uint128_t*)(dst) + 5) = xmm5; \
            break;  \
        case 7:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            xmm3 = *((__uint128_t*)(src) + 3);  \
            xmm4 = *((__uint128_t*)(src) + 4);  \
            xmm5 = *((__uint128_t*)(src) + 5);  \
            xmm6 = *((__uint128_t*)(src) + 6);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            *((__uint128_t*)(dst) + 3) = xmm3; \
            *((__uint128_t*)(dst) + 4) = xmm4; \
            *((__uint128_t*)(dst) + 5) = xmm5; \
            *((__uint128_t*)(dst) + 6) = xmm6; \
            break;  \
        case 8:  \
            xmm0 = *((__uint128_t*)(src) + 0);  \
            xmm1 = *((__uint128_t*)(src) + 1);  \
            xmm2 = *((__uint128_t*)(src) + 2);  \
            xmm3 = *((__uint128_t*)(src) + 3);  \
            xmm4 = *((__uint128_t*)(src) + 4);  \
            xmm5 = *((__uint128_t*)(src) + 5);  \
            xmm6 = *((__uint128_t*)(src) + 6);  \
            xmm7 = *((__uint128_t*)(src) + 7);  \
            *((__uint128_t*)(dst) + 0) = xmm0; \
            *((__uint128_t*)(dst) + 1) = xmm1; \
            *((__uint128_t*)(dst) + 2) = xmm2; \
            *((__uint128_t*)(dst) + 3) = xmm3; \
            *((__uint128_t*)(dst) + 4) = xmm4; \
            *((__uint128_t*)(dst) + 5) = xmm5; \
            *((__uint128_t*)(dst) + 6) = xmm6; \
            *((__uint128_t*)(dst) + 7) = xmm7; \
            break;  \
        } \
        *((uint64_t*)((uintptr_t)(dst) + (sz) - 8)) = *((uint64_t*)((uintptr_t)(src) + (sz) - 8)); \
    } while(0)


// Note: dst and src must be valid address already
#define ycor_amd64_optimized_memcpy_drop_in(dst, src, sz) \
    do {\
        if(ycor_amd64_inline_short_aligned_memcpy_test_ok((dst), (src), (sz))){ \
            ycor_amd64_inline_short_aligned_memcpy((dst), (src), (sz)); \
        }else{ \
            memcpy((dst), (src), (sz)); \
        } \
    } while(0)


static void ycor_default_protector_last_word(void) {
    ycor_t* cor = ycor_get_cor();
    // do some log about the offending 'cor'
    fprintf(stderr, "error: ycor_default_protector_last_word triggered\n");
    fprintf(stderr, "error: cor:%p should call `ycor_exit()` instead of direct "
        "`return` in cor_fp:%p to finish its execution\n", cor, (void*)cor->fp);
    assert(0);
}


__thread ycor_t* ycor_gtls_cor;
static __thread ycor_cofuncp_t ycor_gtls_last_word_fp = ycor_default_protector_last_word;

#ifdef __i386__
    static __thread void* ycor_gtls_fpucw_mxcsr[2];
#elif  __x86_64__
    static __thread void* ycor_gtls_fpucw_mxcsr[1];
#else
    #error "This platform is not currently supported"
#endif


void ycor_thread_init(ycor_cofuncp_t last_word_cor_fp) {
    ycor_save_fpucw_mxcsr(ycor_gtls_fpucw_mxcsr);

    if ((void*)last_word_cor_fp != NULL)
        ycor_gtls_last_word_fp = last_word_cor_fp;
}


// This function `ycor_funcp_protector` should never be
// called. If it's been called, that means the offending
// `co` didn't call ycor_exit(co) instead of `return` to
// finish its execution.

void ycor_funcp_protector(void) {
    if ((void*)(ycor_gtls_last_word_fp) != NULL)
        ycor_gtls_last_word_fp();
    else
        ycor_default_protector_last_word();
    
    assert(0);
}


ycor_share_stack_t* ycor_share_stack_new(size_t sz) {
    return ycor_share_stack_new2(sz, 1);
}

#define ycor_size_t_safe_add_assert(a, b) \
    do { \
        assert((a) + (b) >= (a)); \
    } while(0)



ycor_share_stack_t* ycor_share_stack_new2(size_t sz, char guard_page_enabled) {
    if (sz == 0) 
        sz = 1024 * 1024 * 2;
    
    if (sz  < 4096 && sz > 0)
        sz = 4096;

    assert(sz > 0);

    size_t u_pgsz = 0;
    if (guard_page_enabled != 0) {
        /* although gcc's Built-in Functions to Perform Arithmetic with
           Overflow Checking is better, but it would require gcc >= 5.0 */
        long pgsz = sysconf(_SC_PAGE_SIZE);
        // pgsz must be > 0 && a power of two
        assert(pgsz > 0 && (((pgsz - 1) & pgsz) == 0));
        u_pgsz = (size_t)((unsigned long)pgsz);
        // it should be always true in real life
        assert(u_pgsz == (unsigned long)pgsz && ((u_pgsz << 1) >> 1) == u_pgsz);
        if (sz <= u_pgsz) {
            sz = u_pgsz << 1;
        } else {
            size_t new_sz;
            if ((sz & (u_pgsz - 1)) != 0) {
                new_sz = (sz & (~(u_pgsz - 1)));
                assert(new_sz >= u_pgsz);
                ycor_size_t_safe_add_assert(new_sz, (u_pgsz << 1));
                new_sz = new_sz + (u_pgsz << 1);
                assert(sz / u_pgsz + 2 == new_sz / u_pgsz);
            } else {
                ycor_size_t_safe_add_assert(sz, u_pgsz);
                new_sz = sz + u_pgsz;
                assert(sz / u_pgsz + 1 == new_sz / u_pgsz);
            }
            sz = new_sz;
            assert((sz / u_pgsz > 1) && ((sz & (u_pgsz - 1)) == 0));
        }
    }

    ycor_share_stack_t* p = (ycor_share_stack_t*)malloc(sizeof(ycor_share_stack_t));
    assert_alloc_ptr(p);
    memset(p, 0, sizeof(ycor_share_stack_t));

    if (guard_page_enabled != 0) {
        p->real_ptr = mmap(NULL, sz, 
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assert_alloc_bool(p->real_ptr != MAP_FAILED);
        p->guard_page_enabled = 1;
        assert(0 == mprotect(p->real_ptr, u_pgsz, PROT_READ));

        p->ptr = (void*)(((uintptr_t)p->real_ptr) + u_pgsz);
        p->real_sz = sz;
        assert(sz >= (u_pgsz << 1));
        p->sz = sz - u_pgsz;
    } else {
        p->sz = sz;
        p->ptr = malloc(sz);
        assert_alloc_ptr(p->ptr);
    }

    p->owner = NULL;
#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
    p->valgrind_stk_id = VALGRIND_STACK_REGISTER(p->ptr, (void*)(o->ptr + p->sz));
#endif

#if defined(__i386__) || defined(__x86_64__)
    uintptr_t u_p = (uintptr_t)(p->sz - (sizeof(void*) << 1) + (uintptr_t)p->ptr);
    u_p = (u_p >> 4) << 4;
    p->align_highptr = (void*)u_p;
    p->align_retptr = (void*)(u_p - sizeof(void*));
    *((void**)(p->align_retptr)) = (void*)(ycor_funcp_protector_asm);
    assert(p->sz > (16 + (sizeof(void*) << 1) + sizeof(void*)));
    p->align_limit = p->sz - 16 - (sizeof(void*) << 1);
#else
    #error "This platform is not currently supported"
#endif
    return p;
}


void ycor_share_stack_destroy(ycor_share_stack_t* sstk) {
    assert(sstk != NULL && sstk->ptr != NULL);
#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
    VALGRIND_STACK_DEREGISTER(sstk->valgrind_stk_id);
#endif

    if (sstk->guard_page_enabled) {
        assert(0 == munmap(sstk->real_ptr, sstk->real_sz));
        sstk->real_ptr = NULL;
        sstk->ptr = NULL;
    } else {
        free(sstk->ptr);
        sstk->ptr = NULL;
    }

    free(sstk);
}


ycor_t* ycor_create(ycor_t* main_cor, ycor_share_stack_t* share_stack,
                size_t save_stack_sz, ycor_cofuncp_t fp, void* arg) {
    ycor_t* p = (ycor_t*)malloc(sizeof(ycor_t));
    assert_alloc_ptr(p);
    memset(p, 0, sizeof(ycor_t));

    if (main_cor != NULL) {
        assertptr(share_stack);
        p->share_stack = share_stack;
#ifdef __i386__
        p->reg[YCOROUTINE_REG_IDX_RETADDR] = (void*)fp;
        p->reg[YCOROUTINE_REG_IDX_SP] = p->share_stack->align_retptr;
        #ifndef YCOR_CONFIG_SHARE_FPU_MXCSR_ENV
            p->reg[YCOROUTINE_REG_IDX_FPU] = ycor_gtls_fpucw_mxcsr[0];
            p->reg[YCOROUTINE_REG_IDX_FPU + 1] = ycor_gtls_fpucw_mxcsr[1];
        #endif
#elif  __x86_64__
        p->reg[YCOROUTINE_REG_IDX_RETADDR] = (void*)fp;
        p->reg[YCOROUTINE_REG_IDX_SP] = p->share_stack->align_retptr;
        #ifndef YCOR_CONFIG_SHARE_FPU_MXCSR_ENV
            p->reg[YCOROUTINE_REG_IDX_FPU] = ycor_gtls_fpucw_mxcsr[0];
        #endif
#else 
        #error "This platform is not currently supported"
#endif

        p->main_cor = main_cor;
        p->arg = arg;
        p->fp = fp;
        if (save_stack_sz == 0) {
            save_stack_sz = 64;
        }
        p->save_stack.ptr = malloc(save_stack_sz);
        assert_alloc_ptr(p->save_stack.ptr);
        p->save_stack.size = save_stack_sz;
#if defined(__i386__) || defined(__x86_64__)
        p->save_stack.valid_sz = 0;
#else
        #error "This platform is not currently supported"
#endif
        return p;
    } else {
        /* main cor init */
        p->main_cor = NULL;
        p->arg = arg;
        p->fp = fp;
        p->share_stack = NULL;
        return p;
    }
    assert(0);
}


ycor_attr_no_asan
void ycor_resume(ycor_t* resume_cor) {
    assert(resume_cor != NULL && resume_cor->main_cor != NULL
        && resume_cor->is_end == 0);

    if (resume_cor->share_stack->owner != resume_cor) {
        if (resume_cor->share_stack->owner != NULL) {
            ycor_t* owner_cor = resume_cor->share_stack->owner;
            assert(owner_cor->share_stack == resume_cor->share_stack);
#if defined(__i386__) || defined(__x86_64__)
            assert(
                (
                    (uintptr_t)(owner_cor->share_stack->align_retptr)
                    >=
                    (uintptr_t)(owner_cor->reg[YCOROUTINE_REG_IDX_SP])
                )
                &&
                (
                    (uintptr_t)(owner_cor->share_stack->align_highptr)
                    -
                    (uintptr_t)(owner_cor->share_stack->align_limit)
                    <=
                    (uintptr_t)(owner_cor->reg[YCOROUTINE_REG_IDX_SP])
                )
            );
            owner_cor->save_stack.valid_sz = 
                (uintptr_t)(owner_cor->share_stack->align_retptr)
                -
                (uintptr_t)(owner_cor->reg[YCOROUTINE_REG_IDX_SP]);
            if (owner_cor->save_stack.size < owner_cor->save_stack.valid_sz) {
                free(owner_cor->save_stack.ptr);
                owner_cor->save_stack.ptr = NULL;
                while (1) {
                    owner_cor->save_stack.size = owner_cor->save_stack.size << 1;
                    assert(owner_cor->save_stack.size > 0);
                    if (owner_cor->save_stack.size >= owner_cor->save_stack.valid_sz) 
                        break;
                }
                owner_cor->save_stack.ptr = malloc(owner_cor->save_stack.size);
                assert_alloc_ptr(owner_cor->save_stack.ptr);
            }

            if (owner_cor->save_stack.valid_sz > 0) {
    #ifdef __x86_64__
                ycor_amd64_optimized_memcpy_drop_in(
                    owner_cor->save_stack.ptr,
                    owner_cor->reg[YCOROUTINE_REG_IDX_SP],
                    owner_cor->save_stack.valid_sz
                );
    #else
                memcpy(
                    owner_cor->save_stack.ptr,
                    owner_cor->reg[YCOROUTINE_REG_IDX_SP],
                    owner_cor->save_stack.valid_sz
                );
    #endif
                owner_cor->save_stack.ct_restore++;
            }

            if (owner_cor->save_stack.valid_sz > owner_cor->save_stack.max_cpsz) {
                owner_cor->save_stack.max_cpsz = owner_cor->save_stack.valid_sz;
            }
            owner_cor->share_stack->owner = NULL;
            owner_cor->share_stack->align_validsz = 0;
#else
            #error "This platform is not currently supported"
#endif
        }
        assert(resume_cor->share_stack->owner == NULL);
#if defined(__i386__) || defined(__x86_64__)
        assert(
            resume_cor->save_stack.valid_sz
            <=
            resume_cor->share_stack->align_limit - sizeof(void*)
        );

        if (resume_cor->save_stack.valid_sz > 0) {
    #ifdef __x86_64__
            ycor_amd64_optimized_memcpy_drop_in(
                (void*)(
                    (uintptr_t)(resume_cor->share_stack->align_retptr)
                    -
                    resume_cor->save_stack.valid_sz
                ),
                resume_cor->save_stack.ptr,
                resume_cor->save_stack.valid_sz
            );
    #else
            memcpy(
                (void*)(
                    (uintptr_t)(resume_cor->share_stack->align_retptr)
                    -
                    resume_cor->save_stack.valid_sz
                ),
                resume_cor->save_stack.ptr,
                resume_cor->save_stack.valid_sz
            );
    #endif
            resume_cor->save_stack.ct_restore++;
        }

        if (resume_cor->save_stack.valid_sz > resume_cor->save_stack.max_cpsz) {
            resume_cor->save_stack.max_cpsz = resume_cor->save_stack.valid_sz;
        }
        resume_cor->share_stack->align_validsz = resume_cor->save_stack.valid_sz + sizeof(void*);
        resume_cor->share_stack->owner = resume_cor;
#else
        #error "This platform is not currently supported"
#endif
    }

    ycor_gtls_cor = resume_cor;
    ycorsw(resume_cor->main_cor, resume_cor);
    ycor_gtls_cor = resume_cor->main_cor;
}


void ycor_destroy(ycor_t* cor) {
    assertptr(cor);
    if (ycor_is_main_cor(cor)) {
        free(cor);
    } else {
        if (cor->share_stack->owner == cor) {
            cor->share_stack->owner = NULL;
            cor->share_stack->align_validsz = 0;
        }
        free(cor->save_stack.ptr);
        cor->save_stack.ptr = NULL;
        free(cor);
    }
}