#pragma once

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