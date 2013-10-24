/*
 * swi_handler.c: Custom SWI handler
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <exports.h>
#include "syscalls.h"

extern void swi_handler_c(int swi_num, int *r);
