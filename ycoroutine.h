#ifndef YCOROUTINE_H_
#define YCOROUTINE_H_

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
#include <valgrind/valgrind.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define YCOROUTINE_VERSION_MAJOR 1
#define YCOROUTINE_VERSION_MINOR 2
#define YCOROUTINE_VERSION_PATCH 3


#ifdef __i386__
    #define YCOROUTINE_REG_IDX_RETADDR 0
    #define YCOROUTINE_REG_IDX_SP      1
    #define YCOROUTINE_REG_IDX_BP      2
    #define YCOROUTINE_REG_IDX_FPU     6
#elif __x86_64__
    #define YCOROUTINE_REG_IDX_RETADDR 4
    #define YCOROUTINE_REG_IDX_SP      5
    #define YCOROUTINE_REG_IDX_BP      7
    #define YCOROUTINE_REG_IDX_FPU     8
#else
    #error "This platform is not currently supported"
#endif


/* 保存堆栈 */
typedef struct {
    void* ptr;
    size_t size;
    size_t valid_sz;
    /* 最大拷贝字节数(max copy size in bytes) */
    size_t max_cpsz;
    /* 从共享堆栈拷贝到此保存堆栈(copy from share stack to this save stack) */
    size_t ct_save ;
    /* 从此保存堆栈拷贝到共享堆栈(copy from this save stack to share stack) */
    size_t ct_restore;
} ycor_save_stack_t; 


struct ycor_s;
/* 协程类型 */
typedef struct ycor_s ycor_t;


/* 共享堆栈 */
typedef struct {
    void* ptr;
    size_t sz;
    void* align_highptr;
    void* align_retptr;
    size_t align_validsz;
    size_t align_limit;
    ycor_t* owner;

    char guard_page_enabled;
    void* real_ptr;
    size_t real_sz;

#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
    unsigned long valgrind_stk_id;
#endif
} ycor_share_stack_t; 

typedef void (*ycor_cofuncp_t)(void);


struct ycor_s {

#ifdef __i386__
    #ifdef YCOR_CONFIG_SHARE_FPU_MXCSR_ENY
        void* reg[6];
    #else
        void* reg[8];
    #endif
#elif __x86_64__
    #ifdef YCOR_CONFIG_SHARE_FPU_MXCSR_ENY
        void* reg[8];
    #else
        void* reg[9];
    #endif
#else
    #error "This platform is not currently supported"
#endif

    ycor_t* main_cor;
    void* arg;
    char is_end;

    /* 回调函数 */
    ycor_cofuncp_t fp;

    ycor_save_stack_t    save_stack;
    ycor_share_stack_t* share_stack;

};


#define ycor_likely(expr)   (__builtin_expect((expr), 1))
#define ycor_unlikely(expr) (__builtin_expect((expr), 0))

#define ycor_assert(expr)       ( (ycor_likely(expr)) ? ((void)0) : (abort()) ) 
#define ycor_assert_ptr(ptr)    ( (ycor_likely((ptr) != NULL)) ? ((void)0) : (abort()))

#define ycor_assert_alloc_bool(expr) \
    do { \
        if (ycor_unlikely(!(expr))) { \
            fprintf(stderr, "Aborting: failed to allocate memory: %s:%d:%s\n",\
                __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            abort(); \
        } \
    } while(0)

#define ycor_assert_alloc_ptr(ptr) \
    do { \
        if (ycor_unlikely((ptr) == NULL)) { \
            fprintf(stderr, "Aborting: failed to allocate memory: %s:%d:%s\n",\
                __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            abort(); \
        } \
    } while(0)


#if defined(ycor_attr_no_asan)
    #error "ycor_attr_no_asan already defined"
#endif

#if defined(YCOR_USE_ASAN)
    #if defined(__has_feature)
        #if __has_feature(__address_sanitizer__)
            #define ycor_attr_no_asan \
                __attribute__((__no_sanitize_address__))
        #endif
    #endif
    #if defined(__SANITIZE_ADDRESS__) && !defined(ycor_attr_no_asan)
        #define ycor_attr_no_asan \
            __attribute__((__no_sanitize_address__))
    #endif
#endif

#ifndef ycor_attr_no_asan
    #define ycor_attr_no_asan
#endif

extern void ycor_runtime_test(void);
extern void ycor_thread_init(ycor_cofuncp_t last_word_cor_fp);
extern void ycorsw(ycor_t* from_cor, ycor_t* to_cor) __asm__("ycorsw");         // asm
extern void ycor_save_fpucw_mxcsr(void* p) __asm__("ycor_save_fpucw_mxcsr");    // asm
extern void ycor_funcp_protector_asm(void) __asm__("ycor_funcp_protector_asm"); // asm
extern void ycor_funcp_protector(void);
extern ycor_share_stack_t* ycor_share_stack_new(size_t sz);
ycor_share_stack_t* ycor_share_stack_new2(size_t sz, char guard_page_enabled);
extern void ycor_share_stack_destroy(ycor_share_stack_t* sstk);


extern ycor_t* ycor_create(
        ycor_t* main_cor,
        ycor_share_stack_t* share_stack,
        size_t save_stack_sz,
        ycor_cofuncp_t fp, void* arg
    );

// ycor's Global Thread Local Storage variable 'ycor'
extern __thread ycor_t* ycor_gtls_cor;

ycor_attr_no_asan
extern void ycor_resume(ycor_t* resume_cor);


// extern void ycor_yield1(ycor_t* yield_cor);

#define ycor_yield1(yield_cor) \
    do { \
        ycor_assert_ptr((yield_cor)); \
        ycor_assert_ptr((yield_cor)->main_cor); \
        ycorsw((yield_cor), (yield_cor)->main_cor); \
    } while(0)


#define ycor_yield() \
    do { \
        ycor_yield1(ycor_gtls_cor); \
    } while(0)


#define ycor_get_arg() (ycor_gtls_cor)
#define ycor_get_cor() ({ (void)0; ycor_gtls_cor; })
#define ycor_cor()     ({ (void)0; ycor_gtls_cor; })

extern void ycor_destroy(ycor_t* cor);

#define ycor_is_main_cor(cor)  ({ ((cor)->main_cor) == NULL; })

#define ycor_exit1(cor) \
    do { \
        (cor)->is_end = 1; \
        ycor_assert((cor)->share_stack->owner == (cor)); \
        (cor)->share_stack->owner = NULL; \
        (cor)->share_stack->align_validsz = 0; \
        ycor_yield1((cor)); \
        ycor_assert(0); \ 
    } while(0)


#define ycor_exit() \
    do { \
        ycor_exit1(ycor_gtls_cor); \
    } while(0)


#ifdef __cplusplus
}
#endif

#endif /* YCOROUTINE_H_ */