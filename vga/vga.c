/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 10:04:45 by ndubouil          #+#    #+#             */
/*   Updated: 2021/03/18 11:46:15 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kfs.h"
#include "k_lib.h"
#include "vga.h"

static unsigned short   *terminal_buffer = (unsigned short *)VGA_ADDRESS;
static unsigned int     vga_index = 0;

static uint32 get_current_vga_line(void)
{
    return vga_index / VGA_COLUMNS;
}

static uint32 get_position_line(int pos)
{
    return pos - (pos % VGA_COLUMNS);
}

void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

/*
 * https://wiki.osdev.org/Text_Mode_Cursor
 */
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void clear_screen(void)
{
    int i = 0;
    while (i < VGA_BUFFER) {
            terminal_buffer[i] = 0;
            i++;
    }
    vga_index = 0;
    disable_cursor(20, 20);
}

static void clear_line(int line)
{
    int     i;

    i = 0;
    while (i < VGA_COLUMNS) {
        terminal_buffer[line + i] = 0;
        i++;
    }
}

void clear_previous_char(void)
{
    if (vga_index - 1 > 0 && vga_index % VGA_COLUMNS) {
        vga_index--;
        terminal_buffer[vga_index] = ' ';
    }
}

static void copy_line(int dst, int src)
{
    int     i;

    i = 0;
    while (i < VGA_COLUMNS) {
        terminal_buffer[dst + i] = terminal_buffer[src + i];
        i++;
    }
}

static void scroll_up(void)
{
    unsigned int previous_line;
    int i;

    previous_line = 0;
    i = VGA_COLUMNS;
    while (i < VGA_MAX) {
        clear_line(previous_line);
        copy_line(previous_line, i);
        previous_line = i;
        i += VGA_COLUMNS;
    }
    clear_line(VGA_MAX - VGA_COLUMNS);
    vga_index = VGA_MAX - VGA_COLUMNS;
}

static void print_newline(void)
{
    if (get_current_vga_line() >= VGA_ROWS) {
        scroll_up();
    }
    else {
        vga_index = VGA_COLUMNS * (get_current_vga_line() + 1);
    }
}

void kputchar(char c, unsigned char color)
{
    if (vga_index + 1 > VGA_MAX)
        scroll_up();
    if (c == '\n') {
        print_newline();
    }
    else {
        terminal_buffer[vga_index] = (unsigned short)c | (unsigned short)color << 8;
        vga_index++;
    }
}

void kputstr(char *str, unsigned char color)
{
    int     i;

    i = 0;
    while (str[i]) {
        kputchar(str[i], color);
        i++;
    }
}

void kputnbr(int n, unsigned char color)
{
    char    str[intlen(n) + 1];

    itoa(n, str);
    kputstr(str, color);
}

void kputnbrnl(int n, unsigned char color)
{
    char    str[intlen(n) + 1];

    itoa(n, str);
    kputstr(str, color);
    kputchar('\n', color);
}
