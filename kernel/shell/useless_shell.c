/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:54:26 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 17:31:23 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "useless_shell.h"

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

void get_input(char *buffer, unsigned int buffer_limit)
{
    char                key = 0;
    unsigned int        i = 0;

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

// void strsplit(char *str, int limit, int max, ...)
// {
//     int *args;
//     char *input;
//     int lim = 0;
//     int i;
//     int y;

//     args = (int *)(&str);
//     input = (char *)(*args++);
//     lim = *args++;
//     max = *args++;
//     i = 0;
//     /* While the tokens limit */
//     while (lim) {
//         memset(((void *)(*args)), 0, max);
//         y = 0;
//         while (input[i]) {
//             if (y > max) {
//                 return;
//             }
//             if (input[i] != ' ') {
//                 (*((char **)args))[y] = input[i];
//                 y++;
//             }
//             else {
//                 while (input[i] == ' ')
//                     i++;
//                 break;
//             }
//             i++;
//         }
//         args++;
//         lim--;
//     }
// }

// int get_next_token(char *buffer, char *result, int size)
// {
//     int i = 0;
//     int y = 0;

//     memset(result, 0, size);
//     while (buffer[i] == ' ') {
//         i++;
//     }
//     while (buffer[i]) {
//         if (buffer[i] == ' ' || buffer[i] == '\t') {
//             return i;
//         }
//         result[y] = buffer[i];
//         y++;
//         if (y >= size) {
//             return i;
//         }
//         i++;
//     }
// }

void useless_shell(void)
{
    int     esp;
    int     ebp;
    char    buffer[256];

    while (666) {
        // kputchar(254, WHITE);
        printk("%c> ", 3);
        memset(buffer, 0, 256);
        get_input(buffer, 256);
        if (strlen(buffer) > 0) {
            if (strcmp(buffer, "clear") == 0) {
                clear_screen();
            }
            else if (strcmp(buffer, "reboot") == 0) {
                reboot();
            }
            else if (strcmp(buffer, "shutdown") == 0) {
                qemu_shutdown();
            }
            else if (strcmp(buffer, "stack") == 0) {
                GET_ESP(esp);
                GET_EBP(ebp);
                kdump(esp, ebp - esp);
            }
            else if (strcmp(buffer, "ticks") == 0) {
                printk("%d\n", get_pit_ticks());
            }
            else if (strcmp(buffer, "time") == 0) {
                t_rtc_time rtc_time = get_rtc_time();

                printk("%02d/%02d/%04d %02d:%02d:%02d\n", rtc_time.day, rtc_time.month, rtc_time.year, rtc_time.hour, rtc_time.minute, rtc_time.second);
            }
            else {
                printk("%09s\n", buffer);
            }
        }
    }
}
