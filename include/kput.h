/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kput.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:05:13 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/13 12:16:13 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KPUT_H
# define KPUT_H

#include "kfs.h"
#include "libk.h"
#include "vga.h"

void kputstr(char *str, unsigned char color);
void kputchar(char c, unsigned char color);
void kputnbr(int n, unsigned char color);
void kputnbrnl(int n, unsigned char color);
void printk(char *str, ...);

#endif
