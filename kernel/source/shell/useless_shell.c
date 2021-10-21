/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:54:26 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 19:21:39 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "useless_shell.h"
#include "panic.h"
#include "syscall.h"
#include "process.h"

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

void get_line(char *buffer, unsigned int buffer_limit)
{
    char                key = 0;
    unsigned int        i = 0;

    while (1) {
        key = read_last_typed_char();
        if (key == 0) {
            continue;
        }
        /* If the key is a control key */
        if (key < 0) {
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
        if (GET_KEY_STATUS(get_keystatus(), CTRL_BIT)) {
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
    int     esp;
    int     ebp;
    char    buffer[256];

    while (666) {
        printk("%d %c> ", getpid(), 3);
        memset(buffer, 0, 256);
        get_line(buffer, 256);
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
            else if (strcmp(buffer, "layout") == 0) {
                switch_layout();
                printk("switch layout to : ");
                if (get_layout() == QWERTY)
                    printk("qwerty\n");
                else if (get_layout() == AZERTY)
                    printk("azerty\n");
            }
            else if (strcmp(buffer, "panic") == 0) {
                KPANIC_DUMP("asked panic");
            }
            else if (strcmp(buffer, "pid") == 0) {
                printk("%d\n", getpid());
            }
            else if (strcmp(buffer, "tasks") == 0) {
                print_task_list();
            }
            else if (strcmp(buffer, "exit") == 0) {
                // exit();
                return;
            }
            else if (strcmp(buffer, "shell") == 0) {
                int p = fork();
                disable_interrupts();
                printk("fork %d -> pid %d -> status %d\n", p, getpid(), getstatus());
                enable_interrupts();
                // print_task_list();
                if (p == 0) {
                    printk("spawning new shell !\n");
                    useless_shell();
                    exit();
                } else {
                    // printk("spawning shell parent\n");
                    waitpid(p);
                }
                printk("after exit in shell\n");
                // waitpid(p);
                    // return;
            }
            else if (strcmp(buffer, "syscall") == 0) {
                test_syscall();
            }
            else {
                printk("%09s\n", buffer);
            }
        }
    }
}
