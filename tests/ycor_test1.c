#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ycoroutine.h"    
#include "ycor_assert_override.h"

void cor_function() {
    int *iretp = (int *)ycor_get_arg();
    ycor_t* this_co = ycor_get_cor();
    int ct = 0;
    while (ct < 8) {
        printf(
            "cor:%p save_stack:%p share_stack:%p yield_ct:%d\n",
            this_co, this_co->save_stack.ptr,
            this_co->share_stack->ptr, ct
        );
        ycor_yield();
        (*iretp)++;
        ct++;
    }
    printf(
        "co:%p save_stack:%p share_stack:%p co_exit()\n",
        this_co, this_co->save_stack.ptr,
        this_co->share_stack->ptr
    );
    ycor_exit();
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
    ycor_thread_init(NULL);

    ycor_t* main_cor = ycor_create(NULL, NULL, 0, NULL, NULL);

    ycor_share_stack_t* sstk = ycor_share_stack_new(0);

    int cor_ct_arg_point_to_me = 0;
    ycor_t* cor = ycor_create(main_cor, sstk, 0, cor_function, &cor_ct_arg_point_to_me);

    int ct = 0;
    while (ct < 8) {
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

    ycor_destroy(cor);
    cor = NULL;
    ycor_share_stack_destroy(sstk);
    sstk = NULL;
    ycor_destroy(main_cor);
    main_cor = NULL;

    return 0;
}
