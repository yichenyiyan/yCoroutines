ycor_save_stack_t 结构体

typedef struct {
    void* ptr;
    size_t size;
    size_t valid_sz;
    size_t max_cpsz;
    size_t ct_save;
    size_t ct_restore;
} ycor_save_stack_t;
保存协程的堆栈信息。协程在切换时需要保存当前堆栈的内容，这个结构体用于存储这些信息。

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
共享堆栈是多个协程可以共用的堆栈。
当一个协程被挂起时，它的堆栈内容会保存到一个 ycor_save_stack_t 结构体中，而下一个被调度的协程会使用这个共享堆栈。


ycor_t 结构体

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
    ycor_cofuncp_t fp;
    ycor_save_stack_t save_stack;
    ycor_share_stack_t* share_stack;
};
协程的上下文，包括寄存器、堆栈等信息。

