/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:57:55 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/11 17:27:17 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kput.h"

void printk(char *str, ...)
{
    int     *args;
    char    *format;
    int     i;
    char    tmp_addr[9];

    args = (int *)(&str);
    format = (char *)(*args++);
    i = 0;
    while (format[i]) {
        if (format[i] == '%') {
            i++;
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
}
