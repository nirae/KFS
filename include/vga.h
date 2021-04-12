/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 16:15:01 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 19:26:06 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VGA_H
# define VGA_H

#define VGA_ADDRESS 0xB8000

#define VGA_ROWS 25
#define VGA_COLUMNS 80
#define VGA_BUFFER (VGA_COLUMNS * VGA_ROWS * 2)
#define VGA_MAX (VGA_COLUMNS * VGA_ROWS)

#define BLANK ' ' | (((BLACK << 4) | (WHITE & 0x0F)) << 8)

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
void kputstr(char *str, unsigned char color);
void kputchar(char c, unsigned char color);
void kputnbr(int n, unsigned char color);
void kputnbrnl(int n, unsigned char color);
void printk(char *str, ...);
void clear_previous_char(void);

void move_cursor_right(int nb);
void move_cursor_left(int nb);

#endif
