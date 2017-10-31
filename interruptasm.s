.section .text
    .global itable_start, itable_end
itable_start:
.word itable
itable:
    ldr pc, handler_reset_address
    ldr pc, handler_undefined_address
    ldr pc, handler_svc_address
    ldr pc, handler_prefetchabort_address
    ldr pc, handler_dataabort_address
    ldr pc, handler_reserved_address
    ldr pc, handler_irq_address
    ldr pc, handler_fiq_address
handler_reset_address: 
    .word handler_reset
handler_undefined_address:
    .word handler_undefined
handler_svc_address:
    .word handler_svc
handler_prefetchabort_address:
    .word handler_prefetchabort
handler_dataabort_address:
    .word handler_dataabort
handler_reserved_address:
    .word handler_reserved
handler_irq_address:
    .word handler_irq
handler_fiq_address:
    .word handler_fiq
itable_end:
.word .

handler_reset:
    ldr sp,=reset_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_reset_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_undefined:
    ldr sp,=undef_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_undef_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_svc:
    ldr sp,=svc_stack
    push {lr}
    push {r0-r12}
    mov r0,sp
    bl handler_svc_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_prefetchabort:
    ldr sp,=pfa_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_prefetchabort_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_dataabort:
    ldr sp,=data_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_data_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_reserved:
    ldr sp,=res_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_res_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_irq:
    ldr sp,=irq_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_irq_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0
handler_fiq:
    ldr sp,=fiq_stack
    sub lr,lr,#4
    push {lr}
    push {r0-r12}
    bl handler_fiq_c
    pop {r0-r12}
    pop {lr}
    subs pc,lr,#0

.section .data
    .rept 1024
    .word 0
    .endr
    reset_stack:

    .rept 1024
    .word 0
    .endr
    undef_stack:

    .rept 1024
    .word 0
    .endr
    svc_stack:

    .rept 1024
    .word 0
    .endr
    pfa_stack:

    .rept 1024
    .word 0
    .endr
    data_stack:

    .rept 1024
    .word 0
    .endr
    res_stack:

    .rept 1024
    .word 0
    .endr
    irq_stack:

    .rept 1024
    .word 0
    .endr
    fiq_stack:

