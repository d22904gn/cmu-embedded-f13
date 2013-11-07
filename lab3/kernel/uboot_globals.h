/**
 * @file    uboot_globals.h
 *
 * @brief   Declares globals used for storing old U-Boot stuff.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#ifndef UBOOT_GLOBALS_H
#define UBOOT_GLOBALS_H

#include <types.h>

// Stores U-Boot stack pointer upon entry to main().
extern uint32_t uboot_sp;

// Stores U-Boot exports table.
extern uint32_t uboot_exports;

/*
 * SWI Stuff
 */
// Stores address of the U-Boot SWI handler
extern uint32_t *uboot_swi_handler_addr;

// Stores old U-Boot SWI handler instructions.
extern uint32_t uboot_swi_instr1;
extern uint32_t uboot_swi_instr2;

/*
 * IRQ Stuff
 */
// Stores address of the U-Boot IRQ handler
extern uint32_t *uboot_irq_handler_addr;

// Stores old U-Boot IRQ handler instructions.
extern uint32_t uboot_irq_instr1;
extern uint32_t uboot_irq_instr2;

#endif
