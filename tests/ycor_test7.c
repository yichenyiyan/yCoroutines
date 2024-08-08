#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ycoroutine.h"
#include "ycor_assert_override.h"

void cor_function() {
    int ct = 0;
    int loop_ct = (int)((uintptr_t)(ycor_get_cor()->arg));
    if (loop_ct < 0) {
        loop_ct = 0;
    }
    while (ct < loop_ct) {
        ycor_yield();
        ct++;
    }
    ycor_exit();
}

int main() {
    ycor_thread_init(NULL);

    time_t seed_t = time(NULL);
    assert((time_t)-1 != seed_t);
    srand(seed_t);

    size_t cor_amount = 100;

    // create cor
    assert(cor_amount > 0);
    ycor_t* main_cor = ycor_create(NULL, NULL, 0, NULL, NULL);
    ycor_share_stack_t* sstk = ycor_share_stack_new(0);
    // NOTE: size_t_safe_mul
    ycor_t** coarray = (ycor_t**) malloc(sizeof(void*) * cor_amount);
    assertptr(coarray);
    memset(coarray, 0, sizeof(void*) * cor_amount);
    size_t ct = 0;
    while (ct < cor_amount) {
#ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
        ycor_share_stack_t* private_sstk = ycor_share_stack_new2(
            0, ct % 2
        );
        coarray[ct] = ycor_create(
            main_cor, private_sstk, 0, cor_function,
            (void*)((uintptr_t)rand() % 1000)
        );
        private_sstk = NULL;
#else
        coarray[ct] = ycor_create(
            main_cor, sstk, 0, cor_function,
            (void*)((uintptr_t)rand() % 1000)
        );
#endif
        ct++;
    }

    // naive scheduler with very poor performance (only for demo and testing)
    printf("scheduler start: cor_amount:%zu\n", cor_amount);
    size_t null_ct = 0;
    while (1) {
        ct = 0;
        while (ct < cor_amount) {
            if (coarray[ct] != NULL) {
                ycor_resume(coarray[ct]);
                null_ct = 0;
                if (coarray[ct]->is_end != 0) {
                    printf("ycor_destroy: cor:%zu\n", ct);
                    #ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
                        ycor_share_stack_t* private_sstk = coarray[ct]->share_stack;
                    #endif
                    ycor_destroy(coarray[ct]);
                    coarray[ct] = NULL;
                    #ifdef YCOROUTINE_TO_USE_TOOL_VALGRIND
                        ycor_share_stack_destroy(private_sstk);
                        private_sstk = NULL;
                    #endif
                }
            } else {
                null_ct++;
                if (null_ct >= cor_amount) {
                    goto END;
                }
            }
            ct++;
        }
    }
END:
    ct = 0;
    while (ct < cor_amount) {
        assert(coarray[ct] == NULL);
        ct++;
    }
    ycor_share_stack_destroy(sstk);
    sstk = NULL;
    ycor_destroy(main_cor);
    main_cor = NULL;
    free(coarray);

    printf("sheduler exit");
    
    return 0;
}
