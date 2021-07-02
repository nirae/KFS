/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 17:21:41 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 17:36:04 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_H
# define SYSCALL_H

#include "kfs.h"
#include "libk.h"
#include "interrupts.h"

int test_syscall(void);
void init_syscalls(void);

#endif
