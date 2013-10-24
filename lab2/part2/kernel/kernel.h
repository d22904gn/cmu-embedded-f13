// Reference to our ASM SWI handler wrapper.
extern void swi_handler(void);
extern void setup_usermode(int argc, char** argv);
// Stores old U-Boot SWI handler instructions.
extern long long uboot_swi_handler;
