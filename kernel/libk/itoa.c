/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 13:58:03 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 10:42:21 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libk.h"

void		itoa(int n, char *str)
{
	int		nb;
	int		i;
    int     len;
    
    nb = n;
    len = intlen(n);
    memset(str, 0, len + 1);
    if (nb < 0) {
        nb = -nb;
    }
	i = len - 1;
    if (nb == 0) {
        str[i] = '0';
        return;
    }
	while (nb != 0)
	{
		str[i--] = (nb % 10) + '0';
		nb = nb / 10;
	}
	if (n < 0)
		str[i] = '-';
}
