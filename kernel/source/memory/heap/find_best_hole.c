/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_best_hole.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 10:33:50 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 11:59:42 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static int smallest_hole_cmp(void *ref, t_header *data)
{
    if (data->type != HOLE)
        return 1;
    if (data->size >= (uint32)ref)
        return 0;
    return 1;
}

static int smallest_hole_aligned_cmp(void *ref, t_header *data)
{
    int offset = 0;

    if (data->type != HOLE)
        return 1;

    if (data->size >= (uint32)ref) {
        if (!IS_PAGE_ALIGNED((uint32)data + sizeof(t_header))) {
            offset = PAGE_SIZE - ((uint32)data + sizeof(t_header)) % PAGE_SIZE;
            if (((t_header *)(data))->size - offset >= (uint32)ref) {
                return 0;
            }
        }
    }
    return 1;
}

t_header *find_best_hole(t_heap *heap, uint32 size, uint8 align)
{
    t_header *node = 0;

    if (align)
        node = (t_header *)btree_search(heap->index, size, &smallest_hole_aligned_cmp);
    else
        node = (t_header *)btree_search(heap->index, size, &smallest_hole_cmp);

    if (!node)
        return NO_HOLE_FOUND;
    else
        return node;
}
