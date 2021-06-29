/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_block_by_address.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 11:30:34 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/28 12:05:06 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static int get_block(void *ref, t_header *data)
{
    if ((data + sizeof(t_header)) == (uint32)ref)
        return 0;
    return 1;
}

t_header *find_block_by_address(t_heap *heap, uint32 address)
{
    t_header *header = 0; 
    t_footer *footer = 0;

    header = (t_header*)(address - sizeof(t_header));
    footer = (t_footer*)((uint32)header + header->size - sizeof(t_footer));

    if ((header->magic != HEAP_MAGIC) || (footer->magic != HEAP_MAGIC)) {
        return NO_BLOCK_FOUND;
    }
    return header;
}
