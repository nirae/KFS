/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_header.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 10:41:38 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/28 13:05:23 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

int t_header_size_compare(void *a, void *b)
{
    return (((t_header *)(a))->size - ((t_header *)(b))->size);
}

t_footer *new_footer(t_header *header)
{
    t_footer *footer = (uint32)header + header->size - sizeof(t_footer) ;
    
    footer->magic = HEAP_MAGIC;
    footer->header = header;
    return footer;
}

t_header *new_header(void *addr, uint32 size, uint8 type)
{
    t_header *header = (t_header *)addr;

    header->size    = size;
    header->magic   = HEAP_MAGIC;
    header->type    = type;
    header->parent  = 0;
    header->left    = 0;
    header->right   = 0;

    return header;
}



void *remove_header(t_heap *heap, t_header *header)
{
    /* if the header have a parent, remove it in the parent child */
    if (header->parent) {
        if (((t_header *)(header->parent))->left == header) {
            ((t_header *)(header->parent))->left = 0;
        }
        if (((t_header *)(header->parent))->right == header) {
            ((t_header *)(header->parent))->right = 0;
        }
        header->parent = 0;
    }
    /* if the header haven't parent, it's the first node in the heap */
    else {
        heap->index = 0;
    }
    /* re-insert the childs in the tree */
    if (header->left) {
        btree_insert(&heap->index, 0, ((t_header *)(header->left)), &t_header_size_compare);
        header->left = 0;
    }
    if (header->right) {
        btree_insert(&heap->index, 0, ((t_header *)(header->right)), &t_header_size_compare);
        header->right = 0;
    }
}
