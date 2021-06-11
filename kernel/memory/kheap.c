/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kheap.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 11:06:30 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 19:12:26 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

/* Defined in kernel/arch/i386/linker.ld */
extern uint32 end;
/* Defined in memory/pages.c */
extern t_mempage_directory *kernel_directory;

uint32 placement_address = (uint32)&end;
/* The Kernel heap */
t_heap *kheap = 0;

/* 
 *  Internal kmalloc: Allocate a chunk of memory
 *      size: size to allocate
 *      align: if 1 -> the chunk is page aligned
 *      phys: if set -> store the physical location inside
 */
static uint32 internal_kmalloc(uint32 size, uint8 align, uint32 *phys)
{
    /* If the Kernel heap exist -> real virtual allocation */
    if (kheap != 0) {
        void *addr = heap_allocation(size, (uint8)align, kheap);
        if (phys != 0) {
            t_mempage *page = get_page((uint32)addr, kernel_directory);
            *phys = page->frame*0x1000 + (uint32)addr&0xFFF;
        }
        return (uint32)addr;
    }
    /* else -> physical allocation */
    else {
        // if (align == 1 && (placement_address & 0x00000FFF) )
        if (align == 1 && !IS_PAGE_ALIGNED(placement_address)) {
            ALIGN_WITH_PAGE(placement_address);
        }
        if (phys) {
            *phys = placement_address;
        }
        uint32 tmp = placement_address;
        placement_address += size;
        return tmp;
    }
}

void kfree(void *p)
{
    heap_deallocation(p, kheap);
}
/*
 *  Allocation with page alignement
 */
uint32 kmalloc_a(uint32 size)
{
    return internal_kmalloc(size, 1, 0);
}

/*
 *  Allocation and fill phys arg with the physical address
 */
uint32 kmalloc_p(uint32 size, uint32 *phys)
{
    return internal_kmalloc(size, 0, phys);
}

/*
 *  Allocation with page alignement and fill phys arg with the physical address
 */
uint32 kmalloc_ap(uint32 size, uint32 *phys)
{
    return internal_kmalloc(size, 1, phys);
}

uint32 kmalloc(uint32 size)
{
    return internal_kmalloc(size, 0, 0);
}

void print_kheap_tree(void)
{
    debug_heap_tree(kheap);
}
