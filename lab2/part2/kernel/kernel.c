/*
 * kernel.c: Kernel main (entry) function
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <exports.h>
#include "syscalls.h"

#define PREFETCH_OFFSET 0x8
#define SWI_VEC_ADDR    0x8
#define LDR_OPCODE_UP   0xe59ff000
#define LDR_OPCODE_DOWN 0xe51ff000
#define LDR_IMM_MASK    0xfffff000
#define USR_STACK_START 0xa3000000
#define USR_PROG_ENTRY  0xa2000000

// Reference to our ASM SWI handler wrapper.
extern void swi_handler();

int main(int argc, char *argv[]) {
    /*
     * Install SWI handler
     */
    
    // Obtain SWI vector.
    int swi_vector = *SWI_VECTOR_ADDR;
    
    // Check that SWI vector is valid and exit if not.
    int vec_instr = swi_vector & ~SWI_OFFSET_MASK;
    if (vec_instr != LDR_OPCODE_UP || vec_instr != LDR_OPCODE_DOWN) {
        puts("Unrecognized SWI Vector!\n");
        return 0x0badc0de;
    }
    
    // Extract the LDR immediate load U-Boot's SWI handler addr.
    int sign = (vec_instr == LDR_OPCODE_UP) ? 1 : -1;
    int vec_imm = (vec_instr & LDR_IMM_MASK) * sign;
    int *swi_handler = SWI_VEC_ADDR + PREFETCH_OFFSET + vec_imm;
    
    // Save U-Boot SWI handler into global var.
    uboot_swi_handler = *swi_handler;
    
    // Load our own SWI handler.
    *swi_handler = (LDR_OPCODE_DOWN | 0x04);
    *(swi_handler + 1) = &swi_handler;
    
    // Load user program and exit after user program is done.
    asm("msr CPSR_c, #0xD0\n\t"); // Switch to user mode.
    // TODO XXX
    
    return 0; // To satisfy GCC
}
