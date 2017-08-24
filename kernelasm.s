//tOS Kernel Module

	ldr sp,=stack
	b kernel_main

.section .data

.rept 1024
	.word 0
.endr
stack: