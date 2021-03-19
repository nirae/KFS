/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:54:26 by ndubouil          #+#    #+#             */
/*   Updated: 2021/03/19 19:15:44 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vga.h"
#include "k_lib.h"

/*
 * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.1
 */
static char qwerty_kb_table[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x',
    'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', '6', 0, 0, 0, 0, 0
};

static char qwerty_shift_kb_table[] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '\|', 'Z', 'X',
    'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', '6', 0, 0, 0, 0, 0
};

static uint8 inb(uint16 port)
{
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

static void outb(uint16 port, uint8 value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void get_input(char *buffer, int buffer_limit)
{
    uint16      keycode = 0;
    int         i = 0;
    char        shift = 0;

    while (1) {
        keycode = inb(KEYBOARD_PORT);

        if (keycode == 0x36 || keycode == 0x2A) {
            shift = 1;
            continue;
        }
        else if (keycode == 0xB6 || keycode == 0xAA) {
            shift = 0;
            continue;
        }
        if (keycode > 0 && keycode <= 128) {
            // kputnbr(keycode, GREEN);
            if (keycode == 0x36 || keycode == 0x2A) {
                shift = 1;
                continue;
            }
            if (keycode == 0xB6 || keycode == 0xAA) {
                shift = 0;
                continue;
            }

            if (keycode == 0x1C) {
                kputchar('\n', WHITE);
                outb(KEYBOARD_PORT, 0);
                return;
            }
            else if (keycode == 0x0e) {
                clear_previous_char();
                if (strlen(buffer) > 0) {
                    buffer[i - 1] = 0;
                    i--;
                }
            }
            else {
                if (qwerty_kb_table[keycode] != 0) {
                    if (shift) {
                        kputchar(qwerty_shift_kb_table[keycode], WHITE);
                        buffer[i] = qwerty_shift_kb_table[keycode];
                    }
                    else {
                        kputchar(qwerty_kb_table[keycode], WHITE);
                        buffer[i] = qwerty_kb_table[keycode];
                    }
                    if (i >= buffer_limit) {
                        outb(KEYBOARD_PORT, 0);
                        return;
                    }
                    i++;
                }
            }
        outb(KEYBOARD_PORT, 0);
        }
    }
}

void useless_shell(void)
{
    char    buffer[256];

    memset(buffer, 0, 256);
    while (666) {
        get_input(buffer, 256);
        if (strlen(buffer) > 0) {
            printk("%s\n", buffer);
            memset(buffer, 0, 256);
        }
    }
}
