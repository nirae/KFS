/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 19:05:48 by ndubouil          #+#    #+#             */
/*   Updated: 2021/09/24 12:09:31 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "heap.h"

/*
 * Create a fresh heap
 */
t_heap *create_heap(uint32 start, uint32 end_addr, uint32 max, uint8 supervisor, uint8 readonly)
{
    t_heap *heap;
    uint32  original_start;

    original_start = start;

    if (start % PAGE_SIZE != 0 || end_addr % PAGE_SIZE != 0)
        return 0;

    if (!IS_PAGE_ALIGNED(start))
        ALIGN_WITH_PAGE(start);

    heap = (t_heap *)kmalloc(sizeof(t_heap));
    heap->index_start_address = original_start;
    heap->start_address = start;
    heap->end_address = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;
    /* Start with one large hole with all the space */
    t_header *hole = new_header(start, end_addr - start, HOLE);
    heap->index = hole;
    return heap;
}
