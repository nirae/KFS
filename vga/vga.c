/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 10:04:45 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 19:32:31 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kfs.h"
#include "libk.h"
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

static uint16 get_cursor_position(void)
{
    uint16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;
    return pos;
}


static void set_cursor_position(uint16 position)
{
    outb(0x3D4, 14);
    outb(0x3D5, (position >> 8));
    outb(0x3D4, 15);
    outb(0x3D5, position);
}

static void set_vga_index(unsigned int new)
{
    vga_index = new;

    if (vga_index >= VGA_MAX)
        set_cursor_position((uint16)(VGA_MAX - VGA_COLUMNS) + (vga_index % VGA_COLUMNS));
    else
        set_cursor_position((uint16)vga_index);
}

static void move_right(void)
{
    unsigned int last;

    last = VGA_MAX;
    while (last > vga_index) {
        terminal_buffer[last] = BLANK;
        terminal_buffer[last] = terminal_buffer[last - 1];
        last--;
    }

}

static void move_left(void)
{
    unsigned int cur;

    cur = vga_index;
    while (cur < VGA_MAX) {
        terminal_buffer[cur] = BLANK;
        terminal_buffer[cur] = terminal_buffer[cur + 1];
        cur++;
    }

}

void clear_screen(void)
{
    int i = 0;
    while (i < VGA_BUFFER) {
        terminal_buffer[i] = BLANK;
        i++;
    }
    set_vga_index(0);
}

static void clear_line(int line)
{
    int     i;

    i = 0;
    while (i < VGA_COLUMNS) {
        terminal_buffer[line + i] = BLANK;
        i++;
    }
}

void clear_previous_char(void)
{
    if (vga_index - 1 > 0) {
        set_vga_index(vga_index - 1);
        terminal_buffer[vga_index] = BLANK;
        move_left();
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
    set_vga_index(VGA_MAX - VGA_COLUMNS);
}

static void print_newline(void)
{
    if (get_current_vga_line() >= VGA_ROWS - 1) {
        scroll_up();
    }
    else {
        set_vga_index(VGA_COLUMNS * (get_current_vga_line() + 1));
    }
}

void move_cursor_right(int nb)
{
    set_vga_index(vga_index + nb);
}

void move_cursor_left(int nb)
{
    set_vga_index(vga_index - nb);
}

void kputchar(char c, unsigned char color)
{
    if (vga_index + 1 > VGA_MAX)
        scroll_up();
    if (c == '\n') {
        print_newline();
    }
    else {
        move_right();
        terminal_buffer[vga_index] = (unsigned short)c | (unsigned short)color << 8;
        set_vga_index(vga_index + 1);
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
