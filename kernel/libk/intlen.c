/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intlen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:04:41 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 09:42:38 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libk.h"

int	intlen(int n)
{
	int		i;

	i = 1;
    if (n < 0) {
        n = -n;
        i++;
    }
	while (n >= 10)
	{
		n = n / 10;
		i++;
	}
	return (i);
}
