/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 11:31:31 by ndubouil          #+#    #+#             */
/*   Updated: 2021/05/14 17:25:38 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html */
/* https://wiki.osdev.org/Global_Descriptor_Table */
/* https://wiki.osdev.org/Segmentation */

#ifndef GDT_H
# define GDT_H

#include "kfs.h"

#define GDT_ADDRESS 0x00000800
#define GDT_ENTRIES 7

/* https://wiki.osdev.org/GDT_Tutorial */
#define SEG_DESCTYPE(x) ((x) << 0x04)           /* Descriptor type (0 for system, 1 for code/data) */
#define SEG_PRES(x) ((x) << 0x07)               /* Present */
#define SEG_SAVL(x) ((x) << 0x0C)               /* Available for system use */
#define SEG_LONG(x) ((x) << 0x0D)               /* Long mode */
#define SEG_SIZE(x) ((x) << 0x0E)               /* Size (0 for 16-bit, 1 for 32) */
#define SEG_GRAN(x) ((x) << 0x0F)               /* Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB) */
#define SEG_PRIV(x) (((x) &  0x03) << 0x05)     /* Set privilege level (0 - 3) */
 
#define SEG_DATA_RD        0x00     /* Read-Only */
#define SEG_DATA_RDA       0x01     /* Read-Only, accessed */
#define SEG_DATA_RDWR      0x02     /* Read/Write */
#define SEG_DATA_RDWRA     0x03     /* Read/Write, accessed */
#define SEG_DATA_RDEXPD    0x04     /* Read-Only, expand-down */
#define SEG_DATA_RDEXPDA   0x05     /* Read-Only, expand-down, accessed */
#define SEG_DATA_RDWREXPD  0x06     /* Read/Write, expand-down */
#define SEG_DATA_RDWREXPDA 0x07     /* Read/Write, expand-down, accessed */
#define SEG_CODE_EX        0x08     /* Execute-Only */
#define SEG_CODE_EXA       0x09     /* Execute-Only, accessed */
#define SEG_CODE_EXRD      0x0A     /* Execute/Read */
#define SEG_CODE_EXRDA     0x0B     /* Execute/Read, accessed */
#define SEG_CODE_EXC       0x0C     /* Execute-Only, conforming */
#define SEG_CODE_EXCA      0x0D     /* Execute-Only, conforming, accessed */
#define SEG_CODE_EXRDC     0x0E     /* Execute/Read, conforming */
#define SEG_CODE_EXRDCA    0x0F     /* Execute/Read, conforming, accessed */
 
/* 0x9A */
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
/* 0x92 */
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

/* 0x96 */
#define GDT_STACK_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWREXPD

/* 0xFA */
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD

/* 0xF2 */
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

/* 0xF6 */
#define GDT_STACK_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWREXPD

typedef struct  __attribute__((packed)) gdt_entry
{
   uint16 limit_low;           // The lower 16 bits of the limit.
   uint16 base_low;            // The lower 16 bits of the base.
   uint8  base_middle;         // The next 8 bits of the base.
   uint8  access;              // Access flags, determine what ring this segment can be used in.
   uint8  granularity;
   uint8  base_high;           // The last 8 bits of the base.
}  t_gdt_entry;

typedef struct  __attribute__((packed)) gdt_ptr
{
   uint16 limit;               // The upper 16 bits of all selector limits.
   uint32 base;                // The address of the first gdt_entry_t struct.
} t_gdt_ptr;

extern void gdt_flush(uint32 gdt_ptr);

void init_gdt();

#endif
