@ 18-349 Group 2
@ Wee Loong Kuan (wkuan)
@ Chin Yang Oh (chinyano)
@ Jennifer Lee (jcl1)
@ Note: GCC generated labels have been renamed for clarity.
    .file     "part1-strTable.c"
    .text
    .align    2
    .global   strTable
    .type     strTable, %function
strTable:
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 0, uses_anonymous_args = 0
    
    @ Removed 5 MOVs and 1 STMFD for 3 registers; we only need to use 1 
    @ callee-saved register since we are allowed to clobber r0-r3 and ip by
    @ ATPCS (Section 4.3)
    @ Replaced with 1 STMFD for 1 register.
    stmfd   sp!, {r4}
    cmp     r3, #0
    beq     .Lreturn
    
.Lread_d_char:
    ldrb    r4, [r1]    @ zero_extendqisi2
    
    @ Completely unrolled modulo loop.
    @ ==> Removed 4 normal instructions & 2 conditional branches
    @ ==> Added 8 normal instructions.
    mov     ip, r4
    subs    ip, ip, #69
    subpls  ip, ip, #69
    subpls  ip, ip, #69
    subpls  ip, ip, #69  
    addmis   ip, ip, #23
    addmis   ip, ip, #23
    addmi   ip, ip, #23
    
    cmp     ip, r2
    
    @ Removed conditional BL, one LDR and CMP with a conditional STR.
    strleb  r4, [r0, ip]

    add     r1, r1, #1
    
    @ Replaced CMP since zero flag will be set via SUBS.
    subs    r3, r3, #1
    
    bne     .Lread_d_char
    
.Lreturn:
    @ Removed 1 LDMFD for 3 registers; replaced with LDMFD for 1 register.
    ldmfd   sp!, {r4}
    mov     pc, lr
    .size   strTable, .-strTable
    .ident  "GCC: (GNU) 3.4.5"
