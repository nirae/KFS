/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reboot.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 12:22:33 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 12:26:00 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "keyboard.h"

/*
 *  https://wiki.osdev.org/Reboot
 *  Use the 8042 keyboard controller to pulse the CPU's RESET pin 
 *  TODO: ACPI
 */
void reboot(void)
{
    uint8 good = 0x02;

    asm volatile ("cli");
    while (good & 0x02)
        good = inb(KEYBOARD_CTRL_PORT);
    
    outb(KEYBOARD_CTRL_PORT, 0xFE);
    asm volatile ("hlt");
}
