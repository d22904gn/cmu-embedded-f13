/**
 * @file    context.h
 *
 * @brief   Context switching routine prototypes
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

void launch_task();
void ctx_switch_full(volatile void* next_ctx, volatile void* cur_ctx);
void ctx_switch_half(volatile void* next_ctx) __attribute__((noreturn));
