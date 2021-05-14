/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 15:00:30 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 12:17:46 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

#include "kfs.h"
#include "kput.h"
#include "libk.h"

#define GET_ESP(x) asm volatile("mov %%esp, %0" : "=r"(x) ::)
#define GET_EBP(x) asm volatile("mov %%ebp, %0" : "=r"(x) ::)

void kdump(uint32 addr, int limit);

#endif
