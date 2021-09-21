/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qemu_shutdown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 12:21:06 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 16:17:56 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "panic.h"

/*
 *  https://wiki.osdev.org/Shutdown
 */
void qemu_shutdown(void)
{
    CLEAN_REGISTERS();
    outw(0x604, 0x2000);
}
