/**
 * @file    uboot_globals.c
 *
 * @brief   Defines globals to store old U-Boot stuff in.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#include <types.h>

// Stores U-Boot stack pointer upon entry to main().
uint32_t uboot_sp;

// Stores U-Boot export table.
uint32_t uboot_exports;

/*
 * SWI Stuff
 */
// Stores address of the U-Boot SWI handler
uint32_t *uboot_swi_handler_addr;

// Stores old U-Boot SWI handler instructions.
uint32_t uboot_swi_instr1;
uint32_t uboot_swi_instr2;

/*
 * IRQ Stuff
 */
// Stores address of the U-Boot IRQ handler
uint32_t *uboot_irq_handler_addr;

// Stores old U-Boot IRQ handler instructions.
uint32_t uboot_irq_instr1;
uint32_t uboot_irq_instr2;
