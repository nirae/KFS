/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:57:55 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/16 17:44:09 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kput.h"

void printk(char *str, ...)
{
    int *args;
    char *format;
    int i;

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
