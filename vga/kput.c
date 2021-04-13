/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kput.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:03:26 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/13 12:12:10 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kput.h"
#include "vga.h"

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
