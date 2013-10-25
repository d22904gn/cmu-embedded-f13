/*
 * kernel.c: Kernel main (entry) function
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <exports.h>
#include "common.h"
#include "uboot_swi.h"
#include "syscalls.h"

#define PREFETCH_OFFSET 0x8
#define SWI_VEC_ADDR    0x8
#define LDR_OPCODE_UP   0xe59ff000
#define LDR_OPCODE_DOWN 0xe51ff000
#define LDR_IMM_MASK    0x00000fff
#define USR_STACK_START 0xa3000000
#define USR_PROG_ENTRY  0xa2000000

// References to external ASM functions
void swi_handler();
void setup_usermode(int argc, char** argv);

int main(int argc, char** argv) {
    /*
     * Backup the U-Boot stack pointer. (For easy exit)
     */
    asm("mov %[var], sp" : [var] "=r" (uboot_sp) : );
    
    /*
     * Install SWI handler
     */
    // Obtain SWI vector.
    uint swi_vec_contents = *((uint*) SWI_VEC_ADDR);

    // Check that SWI vector is valid and exit if not.
    uint swi_vector_instr = swi_vec_contents & ~LDR_IMM_MASK;
    if (swi_vector_instr != LDR_OPCODE_UP &&
        swi_vector_instr != LDR_OPCODE_DOWN) {
        puts("Unrecognized SWI Vector!\n");
        return 0x0badc0de;
    }

    // Extract the LDR immediate load U-Boot's SWI handler addr.
    int sign = (swi_vector_instr == LDR_OPCODE_UP) ? 1 : -1;
    int swi_vec_imm = (swi_vec_contents & LDR_IMM_MASK) * sign;
    uboot_handler_addr = *((uint**) (SWI_VEC_ADDR + PREFETCH_OFFSET +
                                      swi_vec_imm));

    // Save U-Boot SWI handler into global var.
    uboot_swi_instr1 = *uboot_handler_addr;
    uboot_swi_instr2 = *(uboot_handler_addr + 1);
    
    // Install our new SWI handler.
    *uboot_handler_addr = LDR_OPCODE_DOWN | 0x04;
    *(uboot_handler_addr + 1) = (uint) &swi_handler;

    /*
     * Setup usermode stuff.
     */
    setup_usermode(argc, argv);

    return 0; // To satisfy GCC
}
