/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 17:22:18 by ndubouil          #+#    #+#             */
/*   Updated: 2021/07/02 17:57:01 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall.h"
#include "interrupts.h"

static void first_syscall(void)
{
    printk("this is my first syscall!\n");
}

static void *syscalls[1] =
{
   &first_syscall
};

uint32 syscalls_number = 1;

int test_syscall(void)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (0));
    return a;
}

void syscall_handler(t_registers *regs)
{
    void    *sys;
    int     sys_ret;

    /* Check if the syscall exist */
    if (regs->eax >= syscalls_number)
        return;

    sys = syscalls[regs->eax];

    printk("syscall number %d called\n", regs->eax);

    /* Push all parameters */
    asm volatile (" \
        push %1; \
        push %2; \
        push %3; \
        push %4; \
        push %5; \
        call *%6; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        " : "=a" (sys_ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (sys));
    regs->eax = sys_ret;
} 

void init_syscalls(void)
{
    register_interrupt_handler(0x80, &syscall_handler);
}
