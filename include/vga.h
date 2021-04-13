/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 16:15:01 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/13 12:15:13 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VGA_H
# define VGA_H

#include "kfs.h"

#define VGA_ADDRESS 0xB8000

#define VGA_ROWS 25
#define VGA_COLUMNS 80
#define VGA_BUFFER (VGA_COLUMNS * VGA_ROWS * 2)
#define VGA_MAX (VGA_COLUMNS * VGA_ROWS)

#define BLANK ' ' | (((BLACK << 4) | (WHITE & 0x0F)) << 8)

extern unsigned short   *terminal_buffer;
extern unsigned int     vga_index;

enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

void clear_screen(void);
void clear_previous_char(void);
void print_newline(void);
void scroll_up(void);
void copy_line(int dst, int src);
void clear_line(int line);
void move_left(void);
void move_right(void);
void set_vga_index(unsigned int new);
uint32 get_position_line(int pos);
uint32 get_current_vga_line(void);
uint16 get_cursor_position(void);
void set_cursor_position(uint16 position);
void move_cursor_right(int nb);
void move_cursor_left(int nb);

#endif
