#include "interrupt.h"
#include "util.h"
#include "syscalls.h"
#include "errno.h"
#include "file.h"
#include "util.h"
 
void interrupt_init(){
    kmemcpy((void*)0, itable_start, itable_end-itable_start);
}

void handler_reset_c(){
    kprintf("RESET\n");
}

void handler_undef_c(){
    kprintf("UNDEF\n");
}

void handler_svc_c(unsigned  * regs)
{
	switch(regs[0])
	{
        case SYSCALL_OPEN:
        {
            //r1 = filename
            //r2 = flags
            regs[0] = file_open((char*)regs[1], regs[2]);
            break;
        }
        case SYSCALL_CLOSE:
        {
            //r1 = file descriptor
            regs[0] = file_close(regs[1]);
            break;
        }
        case SYSCALL_READ:
        {
        	//r1 = file descriptor
        	//r2 = output buffer pointer
        	//r3 = count
        	regs[0] = file_read(regs[1], (void*)regs[2], regs[3]);
        	break;
        }
        case SYSCALL_WRITE:
        {
        	kprintf("%.*s", regs[3], (char*)regs[2]);
        	regs[0] = 1;
        }
        default:
        {
            regs[0] = -ENOSYS;
        }
    }
}

void handler_prefetchabort_c(){
    kprintf("PFA\n");
}

void handler_data_c(){
    kprintf("DATA\n");
}

void handler_res_c(){
    kprintf("RES\n");
}

void handler_irq_c(){
    kprintf("IRQ\n");
}

void handler_fiq_c(){
    kprintf("FIQ\n");
}
