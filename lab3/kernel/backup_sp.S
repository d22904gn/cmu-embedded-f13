/**
 * @file    backup_sp.S
 *
 * @brief   Backups the stack pointer in main().
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#include <asm.h>

    .file   "backup_sp.S"

FUNC(backup_sp)
    ldr     r0, =uboot_sp
    str     sp, [r0]
    mov     pc, lr
