/*
 * main.c: Kernel main (entry) function
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    02 Nov 2013
 */
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "uboot_swi.h"

uint32_t global_data;

#define PREFETCH_OFFSET 8
#define SWI_VEC_ADDR    0x8u
#define LDR_OPCODE_UP   0xe59ff000u
#define LDR_OPCODE_DOWN 0xe51ff000u
#define LDR_IMM_MASK    0x00000fffu

// References to external ASM functions
void swi_handler();
void setup_usermode(int argc, char** argv);

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

    /*
     * Backup the U-Boot stack pointer. (For easy exit)
     */
    asm("mov %[var], sp" : [var] "=r" (uboot_sp) : );
    
    /*
     * Install SWI handler
     */
    // Obtain SWI vector.
    uint32_t swi_vec_contents = *((uint32_t*) SWI_VEC_ADDR);

    // Check that SWI vector is valid and exit if not.
    uint32_t swi_vector_instr = swi_vec_contents & ~LDR_IMM_MASK;
    if (swi_vector_instr != LDR_OPCODE_UP &&
        swi_vector_instr != LDR_OPCODE_DOWN) {
        puts("Unrecognized SWI Vector!\n");
        return 0x0badc0de;
    }

    // Extract the LDR immediate load U-Boot's SWI handler addr.
    int sign = (swi_vector_instr == LDR_OPCODE_UP) ? 1 : -1;
    int swi_vec_imm = (swi_vec_contents & LDR_IMM_MASK) * sign;
    uboot_handler_addr = *((uint32_t**) (SWI_VEC_ADDR + PREFETCH_OFFSET +
                                      swi_vec_imm));

    // Save U-Boot SWI handler into global var.
    uboot_swi_instr1 = *uboot_handler_addr;
    uboot_swi_instr2 = *(uboot_handler_addr + 1);
    
    // Install our new SWI handler.
    *uboot_handler_addr = LDR_OPCODE_DOWN | 0x04;
    *(uboot_handler_addr + 1) = (uint32_t) &swi_handler;

    /*
     * Setup usermode stuff.
     */
    setup_usermode(argc, argv);

    return 0; // To satisfy GCC
}
