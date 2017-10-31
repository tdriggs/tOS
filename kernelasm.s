//tOS Kernel Module

	ldr sp,=stack
	b kernel_main

.section .data

.rept 8192
	.word 0
.endr
stack: