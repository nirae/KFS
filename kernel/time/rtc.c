/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 22:21:04 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 15:18:21 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 *  https://wiki.osdev.org/CMOS
 *  https://wiki.osdev.org/RTC
 *  https://forum.osdev.org/viewtopic.php?f=1&t=17433
 */

#include "rtc.h"
#include "interrupts.h"
#include "io.h"

#define CURRENT_YEAR 2021
 
static t_rtc_time rtc_time = {0};

/*
 *  https://wiki.osdev.org/CMOS#Format_of_Bytes
 */
static uint8 rtc_status = 0;

uint8 get_rtc_register(uint32 reg) {
      outb(0x70, reg);
      return inb(0x71);
}

void set_rtc_register(uint8 reg, uint8 value)
{
    outb(0x70, reg);
    outb(0x71, value);
}

uint8 bcd_mode_to_bin(uint8 value)
{
    return (value & 0x0F) + ((value/ 16) * 10);
}

void init_rtc(void)
{
    rtc_status = get_rtc_register(0x0B);
    rtc_status |=  0x02; /* 24h clock */
    
    set_rtc_register(0x0B, rtc_status);
}

void read_rtc(void) { 
    rtc_time.second = get_rtc_register(0x00);
    rtc_time.minute = get_rtc_register(0x02);
    rtc_time.hour = get_rtc_register(0x04);
    rtc_time.day = get_rtc_register(0x07);
    rtc_time.month = get_rtc_register(0x08);
    rtc_time.year = get_rtc_register(0x09);
 
    /* If BCD mode */
    if (!(rtc_status & 0x04)) {
        rtc_time.second = bcd_mode_to_bin(rtc_time.second);
        rtc_time.minute = bcd_mode_to_bin(rtc_time.minute);
        rtc_time.hour = bcd_mode_to_bin(rtc_time.hour);
        rtc_time.day = bcd_mode_to_bin(rtc_time.day);
        rtc_time.month = bcd_mode_to_bin(rtc_time.month);
        rtc_time.year = bcd_mode_to_bin(rtc_time.year);
    }

    /* 21 -> 2021 */
    rtc_time.year += (CURRENT_YEAR / 100) * 100;
}

t_rtc_time get_rtc_time(void)
{
    read_rtc();
    return rtc_time;
}

