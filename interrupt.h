#pragma once

#define IRQ_KEYBOARD (1<<3)
#define IRQ_MOUSE (1<<4)
#define IRQ_TIMER (1<<6)
#define IRQ_STATUS ((volatile unsigned*) 0x14000000)
#define IRQ_ENABLE ((volatile unsigned*) 0x14000008)
#define IRQ_DISABLE ((volatile unsigned*) 0x1400000c)

extern void* itable_start;
extern void* itable_end;

void interrupt_init();
void handler_reset_c();
void handler_undef_c();
void handler_svc_c();
void handler_prefetchabort_c();
void handler_data_c();
void handler_res_c();
void handler_irq_c();
void handler_fiq_c();