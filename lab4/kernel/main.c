/** @file main.c
 *
 * @brief kernel main
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    14 Nov 2013
 */
 
#include <assert.h>
#include <types.h>
#include <exports.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#define PREFETCH_OFFSET 8
#define SWI_VEC_ADDR    0x8u
#define IRQ_VEC_ADDR    0x18u
#define LDR_OPCODE_UP   0xe59ff000u
#define LDR_OPCODE_DOWN 0xe51ff000u
#define LDR_IMM_MASK    0x00000fffu

uint32_t global_data;

// References to external functions
void swi_handler();
void irq_wrapper();
void enter_usermode(int argc, char** argv);

// Generic handler hijacking function.
int hijack_handler(uint32_t vec_addr, uint32_t new_handler) {
    int sign, vec_imm;
    uint32_t vec_contents, vector_instr;
    uint32_t *handler_addr;
    
    // Obtain vector contents
    vec_contents = *((uint32_t*) vec_addr);

    // Check that vector is valid.
    vector_instr = vec_contents & ~LDR_IMM_MASK;
    if (vector_instr != LDR_OPCODE_UP &&
        vector_instr != LDR_OPCODE_DOWN) {
        printf("Unrecognized vector at 0x%x!\n", vec_addr);
        return 0x0badc0de;
    }

    // Extract the LDR immediate and save handler addr.
    sign = (vector_instr == LDR_OPCODE_UP) ? 1 : -1;
    vec_imm = (vec_contents & LDR_IMM_MASK) * sign;
    handler_addr =
        *((uint32_t**) (vec_addr + PREFETCH_OFFSET + vec_imm));
    
    // Install our new handler.
    *handler_addr = LDR_OPCODE_DOWN | 0x04u;
    *(handler_addr + 1) = new_handler;
    
    return 0;
}

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;
	
    /*
     * Install handlers
     */
    int hijack_result;
    
    // SWI
    hijack_result = hijack_handler(SWI_VEC_ADDR, 
                                   (uint32_t) (&swi_handler));
    if (hijack_result != 0) return hijack_result;
    
    // IRQ
    hijack_result = hijack_handler(IRQ_VEC_ADDR,
                                   (uint32_t) (&irq_wrapper));
    if (hijack_result != 0) return hijack_result;

    /*
     * Setup timing stuff
     */
    // Reset timer (I.e. turn it on)
    reg_write(OSTMR_OSCR_ADDR, 0);
    
    // Mask all interrupts except for timer interrupts
    reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
    reg_set(INT_ICMR_ADDR, 1 << INT_OSTMR_1);
    reg_write(INT_ICLR_ADDR, 0);
    
    // Init match registers.
    // OSTMR0 -> sleep()
    // OSTMR1 -> time()
    reg_write(OSTMR_OSMR_ADDR(0), 0);
    reg_write(OSTMR_OSMR_ADDR(1), 0);
    reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0 | OSTMR_OIER_E1);

    /*
     * Enter usermode.
     */
    enter_usermode(argc, argv);
    
	assert(0);        /* should never get here */
    
    return 0;
}
