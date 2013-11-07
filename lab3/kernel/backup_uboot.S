/**
 * @file    backup_uboot.S
 *
 * @brief   Backups the U-Boot stack pointer and export table
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#include <asm.h>

    .file   "backup_uboot.S"

FUNC(backup_uboot)
    @ Backup stack pointer
    ldr     r0, =uboot_sp
    str     sp, [r0]
    
    @ Backup export table
    ldr     r0, =uboot_exports
    str     r8, [r0]
    
    @ Return
    mov     pc, lr
