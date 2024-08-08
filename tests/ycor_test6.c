#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ycoroutine.h"    
#include "ycor_assert_override.h"

void foo(int ct) {
    printf(
        "cor:%p save_stack:%p share_stack:%p yield_ct:%d\n",
        ycor_get_cor(), ycor_get_cor()->save_stack.ptr,
        ycor_get_cor()->share_stack->ptr, ct
    );
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
        "cor:%p save_stack:%p share_stack:%p !offending return!\n",
        this_cor, this_cor->save_stack.ptr,
        this_cor->share_stack->ptr
    );
    printf("Intended to Abort to test the ycor protector :)\n");
    return;
    ycor_exit();
    assert(0);
}

static void co_protector_last_word(){
    ycor_t* cor = ycor_get_cor();
    // do some log about the offending `cor`
    fprintf(stderr,"error: customized cor_protector_last_word triggered \n");
    fprintf(stderr, "error: cor:%p should call `ycor_exit(cor)` instead of direct "
        "`return` in cor_fp:%p to finish its execution\n", cor, (void*)cor->fp);
    assert(0);
}

int main() {
#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
    if(0){
        printf("%s doesn't have valgrind test yet, "
            "so bypass this test right now.\n",__FILE__
        );
        exit(0);
    }
#endif

    ycor_thread_init(co_protector_last_word);

    ycor_t* main_cor = ycor_create(NULL, NULL, 0, NULL, NULL);
    assertptr(main_cor);

    ycor_share_stack_t* sstk = ycor_share_stack_new(0);
    assertptr(sstk);

    int cor_ct_arg_point_to_me = 0;
    ycor_t* cor = ycor_create(main_cor, sstk, 0, cor_function, &cor_ct_arg_point_to_me);
    assertptr(cor);

    int ct = 0;
    while(ct < 6){
        assert(cor->is_end == 0);
        ycor_resume(cor);
        assert(cor_ct_arg_point_to_me == ct);

        printf("main_cor:%p\n", main_cor);
        ct++;
    }
    ycor_resume(cor);
    assert(cor_ct_arg_point_to_me == ct);
    assert(cor->is_end);

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
        " yield execution, thus you can call it a cor has 'standalone stack' "
        "which just is a very special case of copy-stack.)\n");

    ycor_destroy(cor);
    cor = NULL;

    ycor_share_stack_destroy(sstk);
    sstk = NULL;

    ycor_destroy(main_cor);
    main_cor = NULL;

    return 0;
}
