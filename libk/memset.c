/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memset.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:33:37 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 09:42:38 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libk.h"

void	*memset(void *b, int c, unsigned int len)
{
	unsigned int	i;

	i = 0;
	while (i < len)
    {
		((char *)(b))[i] = c;
        i++;
    }
	return b;
}
