/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 12:08:29 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 10:39:15 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libk.h"

void hex_to_str(unsigned int addr, char *result, int size)
{
    int		len;
    char    base_str[16] = "0123456789abcdef";

	len = size - 1;
    memset(result, '0', size);
    result[size - 1] = 0;
	while (addr != 0)
	{
		result[--len] = base_str[addr % 16];
		addr = addr / 16;
	}
}
