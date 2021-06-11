/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 18:08:58 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 19:07:37 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "interrupts.h"
#include "kput.h"
#include "heap.h"

extern t_heap *kheap;

/* Kernel page directory */
t_mempage_directory *kernel_directory   = 0;
/* Current page directory; */
t_mempage_directory *current_directory  = 0;

/* Defined in memory/kmalloc.c */
extern uint32 placement_address;


/*
 *  US RW  P - Description
 *  0  0  0 - Supervisory process tried to read a non-present page entry
 *  0  0  1 - Supervisory process tried to read a page and caused a protection fault
 *  0  1  0 - Supervisory process tried to write to a non-present page entry
 *  0  1  1 - Supervisory process tried to write a page and caused a protection fault
 *  1  0  0 - User process tried to read a non-present page entry
 *  1  0  1 - User process tried to read a page and caused a protection fault
 *  1  1  0 - User process tried to write to a non-present page entry
 *  1  1  1 - User process tried to write a page and caused a protection fault
 */
/* Page fault handler */
static void page_fault_handler(t_registers regs)
{
    uint32 faulting_address;
    /* Get the faulting address in CR2 reg */
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    printk("Page fault! %s:%d ( ", __FILE__, __LINE__);
    /* Page not present */
    if (!(regs.err_code & PAGE_PRESENT)) {
        printk("present ");
    }
    /* Write operation ? */
    if (regs.err_code & PAGE_RW) {
        printk("read-only ");
    }
    /* User mode */
    if (regs.err_code & PAGE_USER) {
        printk("user-mode ");
    }
    /* Overwrittent reserved bits of page */
    if (regs.err_code & 0x8) {
        printk("reserved ");
    }
    printk(") at %p\n", faulting_address);
    printk("\n");
}

t_mempage *create_page(uint32 address, t_mempage_directory *dir)
{
    uint32 tmp = 0;
    uint32 table_i = 0;

    /* Address to index */
    address /= PAGE_SIZE;
    table_i = address / 1024;
    dir->tables[table_i] = (t_mempage_table *)kmalloc_ap(sizeof(t_mempage_table), &tmp);
    memset(dir->tables[table_i], 0, PAGE_SIZE);
    dir->physical_tables[table_i] = tmp | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    return &dir->tables[table_i]->pages[address % 1024];
}

t_mempage *get_page(uint32 address, t_mempage_directory *dir)
{
    /* Address to index */
    address /= PAGE_SIZE;
    uint32 table_idx = address / 1024;
    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    return 0;
} 

void init_paging()
{
    int         i = 0;
    t_mempage   *page;

    init_frames();
    /* Init a page directory */
    kernel_directory = (t_mempage_directory*)kmalloc_a(sizeof(t_mempage_directory));
    memset(kernel_directory, 0, sizeof(t_mempage_directory));
    current_directory = kernel_directory;

    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's 
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    i = KHEAP_START;
    while (i < (KHEAP_START + KHEAP_INITIAL_SIZE)) {
        create_page(i, kernel_directory);
        i += PAGE_SIZE;
    }

    /* Allocate frames */
    i = 0;
    while (i < placement_address + PAGE_SIZE)
    // while (i < placement_address)
    {
        page = get_page(i, kernel_directory);
        if (!page) {
            page = create_page(i, kernel_directory);
        }
        alloc_frame(page, 0, 0);
        i += PAGE_SIZE;
    }

    // Now allocate those pages we mapped earlier.
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZE) {
        page = get_page(i, kernel_directory);
        if (!page) {
            page = create_page(i, kernel_directory);
        }
        alloc_frame(page, 0, 0);
    }

    register_interrupt_handler(14, page_fault_handler);
    current_directory = kernel_directory;
    enable_paging(kernel_directory->physical_tables);

    // Initialise the kernel heap.
    // kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
    kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
    if (!kheap) {
        printk("PANIC create heap returned 0 in %s:%d\n", __FILE__, __LINE__);
        while (1) {};
    }
}
