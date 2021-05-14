/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 18:13:28 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/12 19:13:33 by ndubouil         ###   ########.fr       */
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
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

void init_idt(void);

#endif
