/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 18:49:30 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 12:02:53 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interrupts.h"
#include "kput.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(t_registers regs)
{
    printk("received interrupt: %d\n", regs.int_no);
    // if (interrupt_handlers[regs.int_no] != 0)
    // {
    //     t_interrupt_handler_func_ptr handler = interrupt_handlers[regs.int_no];
    //     handler(regs);
    // }
}
