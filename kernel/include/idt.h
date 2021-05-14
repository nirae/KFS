/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 18:13:28 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 11:53:59 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDT_H
# define IDT_H

#include "kfs.h"

typedef struct __attribute__((packed))  idt_entry
{
    uint16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16 sel;                 // Kernel segment selector.
    uint8  always0;             // This must always be zero.
    uint8  flags;               // More flags. See documentation.
    uint16 base_hi;             // The upper 16 bits of the address to jump to.
}                                       t_idt_entry;

typedef struct __attribute__((packed))  idt_ptr
{
    uint16 limit;
    uint32 base;                // The address of the first element in our idt_entry_t array.
}                                       t_idt_ptr;

extern void idt_flush(u32int);

void init_idt(void);

#endif
