	.file	"hello.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello world!\000"
.LC1:
        .ascii  "Goodbye world!\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp 
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #8
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	ldr	r0, .L3
	bl	puts
	ldr	r0, .L5
	bl	puts
	mov	r3, #42
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	.LC0
.L5:
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"
