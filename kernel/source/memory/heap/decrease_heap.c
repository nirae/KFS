/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decrease_heap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 09:54:07 by ndubouil          #+#    #+#             */
/*   Updated: 2021/09/24 11:56:50 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include "panic.h"

extern t_mempage_directory *kernel_directory;

/*
 *  Contract the heap
 */
uint32 decrease_heap(t_heap *heap, uint32 new_size)
{
    uint32 i;

    /* if the new size is more than old */
    if (new_size > (heap->end_address - heap->start_address)) {
        KPANIC("Try to decrease the heap with a bigger size than old")
    }
    if (!IS_PAGE_ALIGNED(new_size)) {
        ALIGN_WITH_PAGE(new_size);
    }
    /* if the new size is less then minimum */
    if (new_size < HEAP_MIN_SIZE) {
        new_size = HEAP_MIN_SIZE;

    }
    /* Start at the old heap size */
    i = (heap->end_address - heap->start_address) - PAGE_SIZE;
    /* while the size is more then expected size -> remove pages */
    while (new_size < i)
    {
        free_frame(get_page(heap->start_address + i, kernel_directory));
        i -= PAGE_SIZE;
    }
    /* Set the new heap end address */
    heap->end_address = heap->start_address + new_size;
    return new_size;
}
