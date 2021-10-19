/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 18:08:58 by ndubouil          #+#    #+#             */
/*   Updated: 2021/09/26 18:00:49 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "interrupts.h"
#include "kput.h"
#include "heap.h"
#include "panic.h"
#include "debug.h"

extern t_heap *kheap;

/* Kernel page directory */
t_mempage_directory *kernel_directory   = 0;
/* Current page directory; */
t_mempage_directory *current_directory  = 0;

/* Defined in memory/kheap.c */
extern uint32 placement_address;


/*
 *  US RW  P - Description
 *  0  0   0 - Supervisory process tried to read a non-present page entry
 *  0  0   1 - Supervisory process tried to read a page and caused a protection fault
 *  0  1   0 - Supervisory process tried to write to a non-present page entry
 *  0  1   1 - Supervisory process tried to write a page and caused a protection fault
 *  1  0   0 - User process tried to read a non-present page entry
 *  1  0   1 - User process tried to read a page and caused a protection fault
 *  1  1   0 - User process tried to write to a non-present page entry
 *  1  1   1 - User process tried to write a page and caused a protection fault
 */
/* Page fault handler */
static void page_fault_handler(t_registers regs)
{
    uint32 faulting_address;
    /* Get the faulting address in CR2 reg */
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    printk(KERR "PAGE FAULT \n");
    printk("present[%d], rw[%d], user[%d], rsvd[%d]\n at %p\n", \
            regs.err_code & PAGE_PRESENT, \
            regs.err_code & PAGE_RW, \
            regs.err_code & PAGE_USER, \
            regs.err_code & 0x8, \
            faulting_address);
    KPANIC("page faulting");
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
    // memset(dir->tables[table_i], 0, sizeof(t_mempage_table));
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

static t_mempage_table *clone_table(t_mempage_table *src, uint32 *physAddr)
{
    // Make a new page table, which is page aligned.
    uint32 phys;
    t_mempage_table *table = (t_mempage_table*)kmalloc_ap(sizeof(t_mempage_table), &phys);
    *physAddr = phys;
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(t_mempage_table));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            // TODO memcpy
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].rw) table->pages[i].rw = 1;
            if (src->pages[i].user) table->pages[i].user = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            // memcpy(&(table->pages[i]), &(src->pages[i]), sizeof(t_mempage));
            copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
    }
    return table;
}

t_mempage_directory *clone_directory(t_mempage_directory *src)
{
    uint32 phys;
    t_mempage_directory *dir;

    // Make a new page directory and obtain its physical address.
    dir = kmalloc_ap(sizeof(t_mempage_directory), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(t_mempage_directory));
    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    // uint32 offset = (uint32)(dir->physical_tables) - (uint32)dir;
    uint32 offset = (uint32)(dir->physical_tables) - (uint32)dir;
    // Then the physical address of dir->tablesPhysical is:

    dir->physical_address = phys + offset;

    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue; 
        if (kernel_directory->tables[i] == src->tables[i])
        {
            printk("map the table\n");
           // It's in the kernel, so just use the same pointer.
           dir->tables[i] = src->tables[i];
           dir->physical_tables[i] = src->physical_tables[i];
        }
        else
        {
            printk("clone the table\n");
           // Copy the table.
           uint32 physs;
           dir->tables[i] = clone_table(src->tables[i], &physs);
           dir->physical_tables[i] = physs | PAGE_PRESENT | PAGE_RW | PAGE_USER;
        }
    }
    return dir; 
}

void init_paging()
{
    int         i = 0;
    t_mempage   *page;
    t_mempage   *kpage;

    init_frames();
    /* Init a page directory */
    uint32 phys;
    kernel_directory = (t_mempage_directory *)kmalloc_a(sizeof(t_mempage_directory));
    // kernel_directory = (t_mempage_directory *)kmalloc_ap(sizeof(t_mempage_directory), &phys);
    memset(kernel_directory, 0, sizeof(t_mempage_directory));

    kernel_directory->physical_address = (uint32)kernel_directory->physical_tables;
    // kernel_directory->physical_address = phys;

    current_directory = kernel_directory;

    /*
     *  Map some pages for the kernel heap to get the initial size
     *  Can't allocate frames for these pages for the moment
     */
    i = KHEAP_START;
    while (i < (KHEAP_START + KHEAP_INITIAL_SIZE)) {
        create_page(i, kernel_directory);
        i += PAGE_SIZE;
    }

    /*
     *  Allocate frames
     */
    i = 0;
    while (i < placement_address + PAGE_SIZE) {
        page = get_page(i, kernel_directory);
        if (!page) {
            page = create_page(i, kernel_directory);
        }
        alloc_frame(page, 0, 0);
        i += PAGE_SIZE;
    }

    /*
     *  Now allocate frames for the kernel heap pages
     */
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZE) {
        page = get_page(i, kernel_directory);
        if (!page) {
            page = create_page(i, kernel_directory);
        }
        alloc_frame(page, 0, 0);
    }

    register_interrupt_handler(14, page_fault_handler);
    // current_directory = kernel_directory;
    // enable_paging(kernel_directory->physical_tables);
    enable_paging(kernel_directory->physical_address);
    // return;
    /*
     *  Create the kernel heap
     */
    kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX, 0, 0);
    if (!kheap) {
        KPANIC("Failed to create the kernel heap");
    }
    // increase_heap(0x1000000, kheap);
    // current_directory = clone_directory(kernel_directory);
    t_mempage_directory *new_directory = clone_directory(kernel_directory);
    // enable_paging(new_directory->physical_address);


    // printk("KDIR ! %p\nADDR = %p, PHYS = %p\n", kernel_directory, kernel_directory->physical_address, kernel_directory->physical_tables);
    // printk("NEW ! %p\nADDR = %p, PHYS = %p\n", new_directory, new_directory->physical_address, new_directory->physical_tables);
    // i = 0;
    // while (i < 1024) {
    //     if (kernel_directory->tables[i] != 0 && kernel_directory->physical_tables[i] != 0) {
    //         if (kernel_directory->tables[i] != new_directory->tables[i]) {
    //             printk(
    //                 "directory->tables[%d] different !\n[%x] -> [%x]\n",
    //                 i,
    //                 kernel_directory->tables[i],
    //                 new_directory->tables[i]
    //             );
    //         }
    //         if (kernel_directory->physical_tables[i] != new_directory->physical_tables[i]) {
    //             printk(
    //                 "directory->physical_tables[%d] different !\n[%x] -> [%x]\n",
    //                 i,
    //                 kernel_directory->physical_tables[i],
    //                 new_directory->physical_tables[i]
    //             );
    //         }
    //         printk("table[%d]: %p, phys: %p\n", i, kernel_directory->tables[i], kernel_directory->physical_tables[i]);
    //         for (int j = 0; j < 1024; j++) {
    //             if (kernel_directory->tables[i]->pages[j].present != new_directory->tables[i]->pages[j].present) {
    //                 printk("different !\n");
    //                 printk(
    //                     "kern_dir p[%d] rw[%d] u[%d] f[%p]\n",
    //                     kernel_directory->tables[i]->pages[j].present,
    //                     kernel_directory->tables[i]->pages[j].rw,
    //                     kernel_directory->tables[i]->pages[j].user,
    //                     kernel_directory->tables[i]->pages[j].frame
    //                 );
    //                 printk(
    //                     "new_dir p[%d] rw[%d] u[%d] f[%p]\n",
    //                     new_directory->tables[i]->pages[j].present,
    //                     new_directory->tables[i]->pages[j].rw,
    //                     new_directory->tables[i]->pages[j].user,
    //                     new_directory->tables[i]->pages[j].frame
    //                 );
    //             }          
    //         }
    //     }
    //     i++;
    // }

    current_directory = new_directory;
    enable_paging(new_directory->physical_address);
}
