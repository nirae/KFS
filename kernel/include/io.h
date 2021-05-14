/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 12:08:39 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 12:09:47 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_H
# define IO_H

#include "kfs.h"

uint8   inb(uint16 port);
void    outb(uint16 port, uint8 value);
void    outw(uint16 port, uint16 value);

#endif
