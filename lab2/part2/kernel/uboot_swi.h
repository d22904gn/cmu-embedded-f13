/*
 * uboot_swi.h: Declares globals to store old U-Boot stuff in.
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include "common.h"

#ifndef UBOOT_SWI_H
#define UBOOT_SWI_H

// Stores address of the U-Boot SWI handler
uint *uboot_handler_addr;

// Stores U-Boot stack pointer upon entry to main().
uint uboot_sp;

// Stores old U-Boot SWI handler instructions.
uint uboot_swi_instr1;
uint uboot_swi_instr2;

#endif
