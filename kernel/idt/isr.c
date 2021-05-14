/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 18:49:30 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/13 18:06:09 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "isr.h"
#include "libk.h"
#include "kput.h"

t_isr interrupt_handlers[256];

void register_interrupt_handler(uint8 n, t_isr handler)
{
    interrupt_handlers[n] = handler;
} 

// This gets called from our ASM interrupt handler stub.
void isr_handler(t_registers regs)
{
    printk("received interrupt: %d\n", regs.int_no);
    // if (interrupt_handlers[regs.int_no] != 0)
    // {
    //     t_isr handler = interrupt_handlers[regs.int_no];
    //     handler(regs);
    // }
} 

// This gets called from our ASM interrupt handler stub.
void irq_handler(t_registers regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        t_isr handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
} 
