/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_allocation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 10:38:02 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 16:31:41 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static void *increase_heap_with_new_hole(t_heap *heap, uint32 size)
{
    uint32      old_length;
    uint32      old_end;
    uint32      new_length;
    t_header    *header;

    /* Save the old heap length */
    old_length = heap->end_address - heap->start_address;
    /* Save the old heap end */
    old_end = heap->end_address;
    increase_heap(heap, old_length + size);
    /* get the new heap length */
    new_length = heap->end_address - heap->start_address;

    /* Create a new hole with the new heap size */
    header = new_header(old_end, new_length - old_length, HOLE);
    new_footer(header);
    btree_insert(&heap->index, 0, header, &t_header_size_compare);
    return;
}

/*
 *  Allocates a contiguous region of memory 'size' in size
 *  If page_align set, the block starting on a new page
 */
void *heap_allocation(uint32 size, uint8 page_align, t_heap *heap)
{
    uint32      complete_size;
    uint32      founded_header_size;
    t_header    *founded_hole;

    /* Add the header + footer in the size */
    complete_size = size + sizeof(t_header) + sizeof(t_footer);

    /* Find the smallest hole that fit the size */
    founded_hole = find_best_hole(heap, complete_size, page_align);
    founded_header_size = founded_hole->size;

    /* if no availables holes */
    if (founded_hole == -1) {
        increase_heap_with_new_hole(heap, complete_size);
        /* Go in recursion */
        return heap_allocation(size, page_align, heap);
    }

    /* Check if we can't split the hole in 2 parts */
    if ((founded_hole->size - complete_size) < (sizeof(t_header) + sizeof(t_footer))) {
        /* replace the size by the entire hole size */
        size += founded_hole->size - complete_size;
        complete_size = founded_hole->size;
    }
    /* if page align is required and not already aligned -> create a new hole in front of the block */
    if (page_align && !IS_PAGE_ALIGNED((uint32)founded_hole)) {
        uint32 new_location = (uint32)founded_hole + PAGE_SIZE - ((uint32)founded_hole & 0xFFF) - sizeof(t_header);
        founded_hole->size = PAGE_SIZE - ((uint32)founded_hole & 0xFFF) - sizeof(t_header);
        founded_hole->type = HOLE;
        new_footer(founded_hole);
        founded_hole = new_location;
        // founded_hole->size = founded_hole->size - hole_header->size;
    }
    /* else remove the hole */
    else {
        remove_header(heap, founded_hole);
    }

    /* Fill the new block */
    founded_hole = new_header(founded_hole, complete_size, BLOCK);
    // founded_hole->size = complete_size;
    // founded_hole->type = BLOCK;
    new_footer(founded_hole);
    btree_insert(&heap->index, 0, founded_hole, &t_header_size_compare);

    /* If the hole was splited in 2 parts, add the rest in an independant hole */
    if (founded_header_size - complete_size > 0)
    {
        t_header *hole_header = new_header(
            ((uint32)founded_hole + sizeof(t_header) + size + sizeof(t_footer)),
            founded_header_size - complete_size,
            HOLE
        );
        t_footer *hole_footer = (t_footer *)((uint32)hole_header + founded_header_size - complete_size - sizeof(t_footer));
        if ((uint32)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        btree_insert(&heap->index, 0, hole_header, &t_header_size_compare);
    }
    return (void *) ((uint32)founded_hole + sizeof(t_header));
}
