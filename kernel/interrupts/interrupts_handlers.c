/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupts_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 11:49:01 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 13:59:31 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interrupts.h"
#include "libk.h"

t_interrupt_handler_func_ptr interrupt_handlers[256];

void init_interrupt_handlers(void)
{
    memset(&interrupt_handlers, 0, sizeof(t_interrupt_handler_func_ptr) * 256);
}

t_interrupt_handler_func_ptr get_interrupt_handler(uint8 n)
{
    return interrupt_handlers[n];
}

void register_interrupt_handler(uint8 n, t_interrupt_handler_func_ptr handler)
{
    interrupt_handlers[n] = handler;
} 

void disable_interrupts(void)
{
    asm volatile("cli");
}

void enable_interrupts(void)
{
    asm volatile("sti");
}
