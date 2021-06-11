/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmem.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 17:58:00 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/09 11:53:54 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KMEM_H
# define KMEM_H

#include "kfs.h"

/* 16MB */
#define PHYS_MEM_SIZE 0x1000000

/* 1 page = 4KB */
#define PAGE_SIZE 4096

#define IS_PAGE_ALIGNED(x) (!(x & 0x00000FFF))
#define ALIGN_WITH_PAGE(x) x = ((x & 0xFFFFF000) + PAGE_SIZE)

// Macros used in the bitset algorithms.
/* Get the index in a bitmap */
#define BITMAP_INDEX(a) (a / (8 * sizeof(a)))
/* Get the offset in a bitmap */
#define BITMAP_OFFSET(a) (a % (8 * sizeof(a)))


#define PAGE_PRESENT    0x1
#define PAGE_RW         0x2
#define PAGE_USER       0x4

/* 
 *  Represent a memory Page
 *  
 *  32bit: 1page -> 4KB
 * 
 *  [31  12|11  9|8    7|   6   |     5    |4    3|   2  |  1 |    0   ]
 *  [frame | u/a | rsvd | dirty | accessed | rsvd | user | rw | present]
 *  
 *  frame: frame address                            20bits
 *  u/a: unused/available for kernel use            3bits
 *  rsvd: used by the CPU
 *  dirty: if the page has been written to              1bit
 *  accessed: if accessed since last refresh
 *  user: if clear: supervisor level, if set: user
 *  rw: if clear: readonly, if set: readwrite
 *  present: if page is present in memory
 * 
 * 
 *  unused -> unused + reserved -> 3 + 2 + 2 -> 7
 */
typedef struct      mempage
{
    uint32          present    : 1;
    uint32          rw         : 1;
    uint32          user       : 1;
    uint32          rsvd2      : 2;
    uint32          accessed   : 1;
    uint32          dirty      : 1;
    uint32          rsvd1      : 2;
    uint32          unused     : 3;
    uint32          frame      : 20;
}                   t_mempage;

/* Represent a memory pages table */
typedef struct      mempage_table
{
    t_mempage       pages[1024];
}                   t_mempage_table;


typedef struct      mempage_directory
{
    t_mempage_table *tables[1024];          /* table of pointers to page tables */
    uint32          physical_tables[1024];  /* table of pointers to page tables, but physical location */
    uint32          physical_address;       /* physical address of physical_tables */
}                   t_mempage_directory;


/*
 *  kernel/arch/i386/enable_paging.asm
 */
extern void enable_paging(uint32 page_directory);

/*
 *  memory/frames.c
 */
void init_frames(void);


uint32 kmalloc(uint32 sz);
uint32 kmalloc_a(uint32 sz);
uint32 kmalloc_ap(uint32 sz, uint32 *phys);
void alloc_frame(t_mempage *page, int is_kernel, int is_writeable);
t_mempage *get_page(uint32 address, t_mempage_directory *dir);
t_mempage *create_page(uint32 address, t_mempage_directory *dir);
void switch_page_directory(t_mempage_directory *dir);
void init_paging();

#endif
