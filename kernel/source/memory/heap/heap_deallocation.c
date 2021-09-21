/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_deallocation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 10:39:49 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/28 13:07:41 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include "panic.h"

/*
    Releases a block allocated with 'heap_allocation'.
*/
void heap_deallocation(void *p, t_heap *heap)
{
    if (p == 0)
        return;

    /* Get the header and footer */
    t_header *header = (t_header*)((uint32)p - sizeof(t_header));
    t_footer *footer = (t_footer*)((uint32)header + header->size - sizeof(t_footer));

    if ((header->magic != HEAP_MAGIC) || (footer->magic != HEAP_MAGIC)) {
        KPANIC("no header or footer found for deallocation");
    }
    /* The block is now a hole */
    header->type = HOLE;

    /* Unify with the left block */
    t_footer *test_footer = (t_footer*)((uint32)header - sizeof(t_footer));
    if ((uint32)test_footer > heap->index_start_address) {
        /* If the block is just after a hole footer */
        if (test_footer->magic == HEAP_MAGIC && test_footer->header->type == HOLE) {
            /* Merge the block with the previous one */
            uint32 cache_size = header->size; // Cache our current size.
            header = test_footer->header;     // Rewrite our header with the new one.
            footer->header = header;          // Rewrite our footer to point to the new header.
            header->size += cache_size;       // Change the size.
        }
    }
    /* Unify with the right block */
    t_header *test_header = (t_header*)((uint32)footer + sizeof(t_footer));
    /* If the block is just after a hole header */
    if (test_header->magic == HEAP_MAGIC && test_header->type == HOLE) {
        /* Merge the block with the next one */
        header->size += test_header->size;
        test_footer = (t_footer*) ((uint32)test_header + test_header->size - sizeof(t_footer));
        footer = test_footer;
        remove_header(heap, test_header);
    }
    /* if the footer location is the heap end */
    if ((uint32)footer + sizeof(t_footer) == heap->end_address) {
        /* decrease the heap */
        uint32 old_length = heap->end_address - heap->start_address;
        uint32 new_length = decrease_heap(heap, (uint32)header - heap->start_address);
        /* if the size after resizing is more then 0 */
        if (header->size - (old_length - new_length) > 0)
        {
            /* resize the hole */
            header->size -= new_length;
            footer = new_footer(header);
        }
        else
        {
            remove_header(heap, test_header);
        }
    }
}
