/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   increase_heap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 09:52:44 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 09:54:23 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

extern t_mempage_directory *kernel_directory;

/*
 *  Expand the heap
 */
void increase_heap(t_heap *heap, uint32 new_size)
{
    t_mempage   *page;
    uint32      i;

    /* if the new size is less than old */
    if (new_size <= (heap->end_address - heap->start_address)) {
        printk("PANIC %s:%d\n", __FILE__, __LINE__);
        while (1) {};
    }
    /* if the heap + the new size is bigger then heap maximum */
    if (heap->start_address + new_size > heap->max_address) {
        printk("PANIC %s:%d\n", __FILE__, __LINE__);
        while (1) {};
    }
    if (!IS_PAGE_ALIGNED(new_size)) {
        ALIGN_WITH_PAGE(new_size);
    }

    /* Start at the old heap size */
    i = heap->end_address - heap->start_address;
    /* while the size is less then expected size -> add new pages */
    while (i < new_size) {
        page = get_page(heap->start_address + i, kernel_directory);
        if (!page) {
            page = create_page(heap->start_address + i, kernel_directory);
        }
        alloc_frame(page, heap->supervisor, !heap->readonly);
        i += PAGE_SIZE;
    }
    /* Set the new heap end address */
    heap->end_address = heap->start_address + new_size;
}
