/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 19:05:48 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 11:03:24 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "heap.h"

/*
 * Create a fresh heap
 */
t_heap *create_heap(uint32 start, uint32 end_addr, uint32 max, uint8 supervisor, uint8 readonly)
{
    uint32  original_start = start;
    /* check if start and end are page aligned */
    // if (!(start % PAGE_SIZE == 0) || !(end_addr % PAGE_SIZE == 0)) {
    if (!IS_PAGE_ALIGNED(start) || !IS_PAGE_ALIGNED(end_addr)) {
        return 0;
    }
    
    t_heap *heap = (t_heap *)kmalloc(sizeof(t_heap));
    // Initialise the index.
    
    /* Set the real start address, after the index */
    // start += sizeof(type_t) * HEAP_INDEX_SIZE;
    if (!IS_PAGE_ALIGNED(start)) {
        ALIGN_WITH_PAGE(start);
    }
    // Write the start, end and max addresses into the heap structure.
    heap->index_start_address = original_start;
    heap->start_address = start;
    heap->end_address = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    // We start off with one large hole in the index.
    // t_header *hole = (t_header *)start;
    // hole->size = end_addr-start;
    // hole->magic = HEAP_MAGIC;
    // hole->type = HOLE;
    // hole->parent = 0;
    // hole->left = 0;
    // hole->right = 0;
    t_header *hole = new_header(start, end_addr - start, HOLE);

    heap->index = hole;


    return heap;
}
