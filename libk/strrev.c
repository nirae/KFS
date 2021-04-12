/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strrev.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:19:50 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/12 09:42:38 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libk.h"

char	*strrev(char *str)
{
	char	tmp;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (str[i] != '\0')
		i++;
	i--;
	while (i > len)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
		i--;
		len++;
	}
	return (str);
}
