/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kdebug_dump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 15:04:49 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 11:07:31 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void kdebug_dump(uint32 addr, int limit)
{
    char *c = (char *)addr;
    char str_addr[9];
    int i;
    uint32 previous;

    if (limit <= 0)
        return;

    for (i = 0; i < limit; i++) {
        // Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0) {
            // Not the first line
            if (i != 0) {
                previous = addr - 16;
                while (previous < addr) {
                    if (*(char *)previous <= 32)
                        printk("%c", '.');
                    else
                        printk("%c", *(char *)previous);
                    previous++;
                }
                printk("\n");
            }
            // hex_to_str(addr, str_addr, sizeof(str_addr));
            printk("%p: ", addr);
        }
        hex_to_str((uint32)c[i], str_addr, 3);
        printk("%s ", str_addr);
        addr++;
    }
    // Last line
    for (i = 0; i < ((limit % 16) * 3); i++) {
        printk(" ");
    }
    if ((limit % 16) == 0) {
        previous = addr - 16;
    } else {
        previous = addr - (limit % 16);
    }
    while (previous < addr) {
        if (*(char *)previous <= 32)
            printk("%c", '.');
        else
            printk("%c", *(char *)previous);
        previous++;
    }
    printk("\n");
}
