/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:01:43 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 12:14:30 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kfs.h"
#include "libk.h"
#include "io.h"

uint16 get_cursor_position(void)
{
    uint16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;
    return pos;
}


void set_cursor_position(uint16 position)
{
    outb(0x3D4, 14);
    outb(0x3D5, (position >> 8));
    outb(0x3D4, 15);
    outb(0x3D5, position);
}
