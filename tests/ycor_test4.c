#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>

#include "ycoroutine.h"
#include "ycor_assert_override.h"

// 多线程测试

uint64_t gl_race_ycor_yield_ct = 0;
pthread_mutex_t gl_race_ycor_yield_ct_mutex = PTHREAD_MUTEX_INITIALIZER;

void foo(int ct) {
    printf(
        "co:%p save_stack:%p share_stack:%p yield_ct:%d\n",
        ycor_get_cor(), ycor_get_cor()->save_stack.ptr,
        ycor_get_cor()->share_stack->ptr, ct
    );
    pthread_mutex_lock(&gl_race_ycor_yield_ct_mutex);
    gl_race_ycor_yield_ct++;
    pthread_mutex_unlock(&gl_race_ycor_yield_ct_mutex);
    ycor_yield();
    (*((int*)(ycor_get_arg())))++;
}

void cor_function() {
    ycor_t* this_cor = ycor_get_cor();
    assert(!ycor_is_main_cor(this_cor));
    assert(this_cor->fp == (void*)cor_function);
    assert(this_cor->is_end == 0);
    int ct = 0;
    while(ct < 6){
        foo(ct);
        ct++;
    }
    printf(
        "co:%p save_stack:%p share_stack:%p co_exit()\n",
        this_cor, this_cor->save_stack.ptr,
        this_cor->share_stack->ptr
    );
    pthread_mutex_lock(&gl_race_ycor_yield_ct_mutex);
    gl_race_ycor_yield_ct++;
    pthread_mutex_unlock(&gl_race_ycor_yield_ct_mutex);
    ycor_exit();
    assert(0);
}

void* thread_function(void* pthread_in_arg) {
    pthread_t t = pthread_self();
    size_t idx = 0;
    assert(sizeof(t) > 0);
    printf("\ntid:0x");
    while(idx < sizeof(t)){
        printf("%02x", *(((uint8_t*)(&t)) + idx));
        idx++;
    }
    printf("\n");
    
    ycor_thread_init(NULL);

    ycor_t* main_cor = ycor_create(NULL, NULL, 0, NULL, NULL);
    assertptr(main_cor);

    ycor_share_stack_t* sstk = ycor_share_stack_new(0);
    assertptr(sstk);
    ycor_share_stack_t* sstk2 = ycor_share_stack_new(0);
    assertptr(sstk2);

    int cor_ct_arg_point_to_me = 0;
    int cor2_ct_arg_point_to_me = 0;
    int cor3_ct_arg_point_to_me = 0;
    ycor_t* cor = ycor_create(main_cor, sstk, 0, cor_function, &cor_ct_arg_point_to_me);
    assertptr(cor);
    ycor_t* cor2 = ycor_create(main_cor, sstk2, 0, cor_function, &cor2_ct_arg_point_to_me);
    assertptr(cor2);
    ycor_t* cor3 = ycor_create(main_cor, sstk2, 0, cor_function, &cor3_ct_arg_point_to_me);
    assertptr(cor3);

    int ct = 0;
    while(ct < 6){
        assert(cor->is_end == 0);
        ycor_resume(cor);
        assert(cor_ct_arg_point_to_me == ct);

        assert(cor2->is_end == 0);
        ycor_resume(cor2);
        assert(cor2_ct_arg_point_to_me == ct);

        assert(cor3->is_end == 0);
        ycor_resume(cor3);
        assert(cor3_ct_arg_point_to_me == ct);

        printf("main_cor:%p\n", main_cor);
        ct++;
    }
    ycor_resume(cor);
    assert(cor_ct_arg_point_to_me == ct);
    assert(cor->is_end);

    ycor_resume(cor2);
    assert(cor2_ct_arg_point_to_me == ct);
    assert(cor2->is_end);

    ycor_resume(cor3);
    assert(cor3_ct_arg_point_to_me == ct);
    assert(cor3->is_end);

    printf("main_cor:%p\n", main_cor);

    printf(
        "\ncopy-stack cor:%p:\n    max stack copy size:%zu\n"
        "    save (from share stack to save stack) counter of the private save stack:%zu\n"
        "    restore (from save stack to share stack) counter of the private save stack:%zu\n",
        cor, cor->save_stack.max_cpsz, 
        cor->save_stack.ct_save, 
        cor->save_stack.ct_restore
    );
    printf("\n(Since the share stack used by the co has only one user `cor`, "
        "so there is no need to save/restore the stack every time during resume &"
        " yield execution, thus you can call it a co has 'standalone stack' "
        "which just is a very special case of copy-stack.)\n");

    printf(
        "\ncopy-stack cor2:%p:\n    max stack copy size:%zu\n"
        "    save (from share stack to save stack) counter of the private save stack:%zu\n"
        "    restore (from save stack to share stack) counter of the private save stack:%zu\n",
        cor2, cor2->save_stack.max_cpsz, 
        cor2->save_stack.ct_save, 
        cor2->save_stack.ct_restore
    );
    printf(
        "\ncopy-stack cor3:%p:\n    max stack copy size:%zu\n"
        "    save (from share stack to save stack) counter of the private save stack:%zu\n"
        "    restore (from save stack to share stack) counter of the private save stack:%zu\n",
        cor3, cor3->save_stack.max_cpsz, 
        cor3->save_stack.ct_save, 
        cor3->save_stack.ct_restore
    );

    printf("\n(The cor2 & cor3 share the share stack sstk2, thus it is "
        "necessary to save/restore the stack every time during resume &"
        " yield execution, thus it is a ordinary case of copy-stack.)\n");

    uint64_t tmp_gl_ct;
    pthread_mutex_lock(&gl_race_ycor_yield_ct_mutex);
    tmp_gl_ct = gl_race_ycor_yield_ct;
    pthread_mutex_unlock(&gl_race_ycor_yield_ct_mutex);
    printf("\ngl_race_ycor_yield_ct:%" PRIu64"\n", tmp_gl_ct);

    ycor_destroy(cor);
    cor = NULL;
    ycor_destroy(cor2);
    cor2 = NULL;
    ycor_destroy(cor3);
    cor3 = NULL;

    ycor_share_stack_destroy(sstk);
    sstk = NULL;
    ycor_share_stack_destroy(sstk2);
    sstk2 = NULL;

    ycor_destroy(main_cor);
    main_cor = NULL;

    return 0;
}

int main() {
#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
    if(1) {
        printf("%s doesn't have valgrind test yet, "
            "so bypass this test right now.\n",__FILE__
        );
        exit(0);
    }
#endif

    pthread_t t1,t2;

    assert(0 == pthread_create(
        &t1, NULL, thread_function, NULL
    ));

    assert(0 == pthread_create(
        &t2, NULL, thread_function, NULL
    ));

    assert(0 == pthread_join(t1, NULL));
    assert(0 == pthread_join(t2, NULL));
    
    return 0;
}
