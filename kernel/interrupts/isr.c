/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 18:49:30 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 17:52:10 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interrupts.h"
#include "kput.h"
#include "panic.h"

extern t_interrupt_handler_func_ptr interrupt_handlers[256];

/* https://wiki.osdev.org/Exceptions */
void isr_handler(t_registers regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        t_interrupt_handler_func_ptr handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    switch (regs.int_no) {
    case 0:
        printk(KERR "Division by Zero\n");
        break;
    case 1:
        printk(KERR "Debug\n");
        break;
    case 2:
        printk(KERR "Non Maskable Interrupt\n");
        break;
    case 3:
        printk(KERR "Breakpoint\n");
        break;
    case 4:
        printk(KERR "Overflow\n");
        break;
    case 5:
        printk(KERR "Bound Range Exceeded\n");
        break;
    case 6:
        printk(KERR "Invalid Opcode\n");
        break;
    case 7:
        printk(KERR "Device Not Available\n");
        break;
    case 8:
        printk(KERR "Double Fault\n");
        break;
    case 10:
        printk(KERR "Invalid TSS\n");
        break;
    case 11:
        printk(KERR "Segment Not Present\n");
        break;
    case 12:
        printk(KERR "Stack-Segment Fault\n");
        break;
    case 13:
        printk(KERR "General Protection Fault\n");
        break;
    case 14:
        printk(KERR "Page Fault\n");
        break;
    default:
        printk(KERR "ISR code = %d\n", regs.int_no);
        break;
    }
    KPANIC("");
}
