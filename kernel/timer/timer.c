/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 12:24:20 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 11:53:19 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.h"
#include "interrupts.h"
#include "kput.h"

uint32 ticks = 0;

static void timer_callback(t_registers regs)
{
    ticks++;
    // printk("tick\n");
    // printk("tick: %d\n", ticks);
    // if (ticks % 18 == 0)
    // {
    //     printk("second\n");
    // }
}

void init_timer(uint32 frequency)
{
    printk("init timer %d\n", frequency);
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);
    // register_interrupt_handler(0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32 divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8 l = (uint8)(divisor & 0xFF);
    uint8 h = (uint8)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    // outb(0x40, divisor & 0xFF);
    outb(0x40, l);
    // outb(0x40, (divisor >> 8) & 0xFF);
    outb(0x40, h);
} 
