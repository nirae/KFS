/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:57:55 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 20:24:29 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kput.h"
#include "interrupts.h"

void printk(char *str, ...)
{
    int     *args;
    char    *format;
    int     i;
    char    tmp_addr[9];
    uint8   zero_padding;

    DISABLE_INTERRUPTS();
    args = (int *)(&str);
    format = (char *)(*args++);
    i = 0;
    while (format[i]) {
        zero_padding = 0;
        if (format[i] == '%') {
            i++;
            /* padding with zeros - max 9 */
            if (format[i] == '0') {
                i++;
                zero_padding = format[i] - '0';
                i++;
            }
            if (format[i] == 'c') {
                kputchar(*args++, WHITE);
            }
            else if (format[i] == 's') {
                kputstr(*((char **)args++), WHITE);
            }
            else if (format[i] == 'x') {
                memset(tmp_addr, 0, sizeof(tmp_addr));
                hex_to_str(*args++, tmp_addr, sizeof(tmp_addr));
                kputstr(tmp_addr, WHITE);
            }
            else if (format[i] == 'p') {
                memset(tmp_addr, 0, sizeof(tmp_addr));
                hex_to_str(*args++, tmp_addr, sizeof(tmp_addr));
                kputstr("0x", WHITE);
                kputstr(tmp_addr, WHITE);
            }
            else if (format[i] == 'd') {
                if (zero_padding > 0) {
                    while (zero_padding - intlen(*args)) {
                        kputchar('0', WHITE);
                        zero_padding--;
                    }
                }
                kputnbr(*args++, WHITE);
            }
            else {
                kputchar('%', WHITE);
                kputchar(format[i], WHITE);
            }
        }
        else {
            kputchar(format[i], WHITE);
        }
        i++;
    }
    ENABLE_INTERRUPTS();
}
