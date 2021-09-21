/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 11:32:06 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 17:53:37 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interrupts.h"

void remap_irq_table(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_handler(t_registers regs)
{
    /* Send EOI (end of interrupt) signal to the PICs */
    if (regs.int_no >= 40)
    {
        /* reset signal to slave */
        outb(0xA0, 0x20);
    }
    /* reset signal to master */
    outb(0x20, 0x20);

    if (get_interrupt_handler(regs.int_no) != 0)
    {
        t_interrupt_handler_func_ptr handler = get_interrupt_handler(regs.int_no);
        handler(regs);
    }
} 
