/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:54:26 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/13 16:16:06 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vga.h"
#include "libk.h"
#include "keyboard.h"

/*
 *  shift the buffer to the right
 *  usefull when adding characters in the middle of a line
 */
static void move_right_buffer(char *buffer, int size, int index)
{
    while (size > index) {
        buffer[size] = 0;
        buffer[size] = buffer[size - 1];
        size--;
    }

}

/*
 *  shift the buffer to the left
 *  usefull when remove characters in the middle of a line
 */
static void move_left_buffer(char *buffer, int size, int index)
{
    while (index < size) {
        buffer[index] = 0;
        buffer[index] = buffer[index + 1];
        index++;
    }

}

void get_input(char *buffer, int buffer_limit)
{
    char        key = 0;
    int         i = 0;

    while (1) {
        key = get_pressed_char();
        /* If the key is a control key */
        if (key <= 0) {
            /* Left Arrow */
            if (key == LEFT_ARROW_KEY) {
                if (i > 0) {
                    move_cursor_left(1);
                    i--;
                }
            }
            /* Right Arrow */
            else if (key == RIGHT_ARROW_KEY) {
                if (i < strlen(buffer)) {
                    move_cursor_right(1);
                    i++;
                }
            }
            continue;
        }
        /* Enter key */
        if (key == '\n') {
            kputchar(key, WHITE);
            return;
        }
        /* If CTRL is pressed */
        if (GET_KEY_STATUS(keystatus, CTRL_BIT)) {
            /* CTRL + L */
            if (key == 'l') {
                memset(buffer, 0, 256);
                clear_screen();
                return;
            }
        }
        /* Return key */
        if (key == '\b') {
            if (strlen(buffer) > 0 && i > 0) {
                clear_previous_char();
                buffer[i - 1] = 0;
                i--;
                move_left_buffer(buffer, buffer_limit, i);
            }
        }
        /* All other keys */
        else {
            kputchar(key, WHITE);
            move_right_buffer(buffer, buffer_limit, i);
            buffer[i] = key;
            if (strlen(buffer) == buffer_limit) {
                return;
            }
            i++;
        }
    }
}

void useless_shell(void)
{
    char    buffer[256];

    while (666) {
        memset(buffer, 0, 256);
        get_input(buffer, 256);
        if (strlen(buffer) > 0) {
            if (strcmp(buffer, "clear") == 0) {
                clear_screen();
            }
            else {
                printk("%s\n", buffer);
            }
        }
    }
}
