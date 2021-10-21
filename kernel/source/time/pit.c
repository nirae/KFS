/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 12:24:20 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 19:15:59 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pit.h"
#include "interrupts.h"
#include "kput.h"
#include "io.h"
#include "process.h"

uint32 ticks = 0;

static void pit_handler(t_registers regs)
{
    /* Unused parameter */
    (void)regs;
    ticks++;
    // if (ticks % 2 == 0)
        switch_task();
}

uint32 get_pit_ticks(void)
{
    return ticks;
}

/*
 *  https://wiki.osdev.org/Programmable_Interval_Timer
 */
void init_pit(uint32 hz)
{
    uint32 divisor = 1193180 / hz;

    register_interrupt_handler(IRQ0, &pit_handler);
    /* command byte */
    outb(0x43, 0x36);
    /* send in 2 times */
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
} 
