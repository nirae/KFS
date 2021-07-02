/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 10:08:48 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 17:05:58 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KPANIC_H
# define KPANIC_H

#include "kfs.h"
#include "kput.h"
#include "debug.h"

extern void halt(void);

#define DUMP_STACK() int esp, ebp; GET_EBP(ebp); GET_ESP(esp); kdump(esp, ebp - esp)

#define CLEAN_REGISTERS() asm volatile ("xor %eax, %eax"); \
                        asm volatile ("xor %ebx, %ebx"); \
                        asm volatile ("xor %ecx, %ecx"); \
                        asm volatile ("xor %edx, %edx");

#define KPANIC(x)   printk("[ KFS - KERNEL PANIC at (%s:%d) ] %s\n", __FILE__, __LINE__, x); \
                    CLEAN_REGISTERS(); \
                    while (666) {halt();};

#define KPANIC_DUMP(x)   printk("[ KFS - KERNEL PANIC at (%s:%d) ] %s\n", __FILE__, __LINE__, x); \
                    DUMP_STACK(); CLEAN_REGISTERS(); \
                    while (666) {halt();};

#define KERR "[ KFS - KERNEL ERROR ] "

#endif
