/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_heap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 09:55:11 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 10:02:38 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static void debug_tree_func(t_header *header)
{
    printk("%p: %d %s -> p:%p\n",
        header,
        header->size,
        header->type ? "HOLE" : "BLOCK",
        header->parent
    );
    if (header->left)
        printk("    left: %p\n", header->left);
    if (header->right)
        printk("    right: %p\n", header->right);
}

void debug_heap_tree(t_heap *heap)
{
    // header_btree_apply_prefix(heap->index, &debug_tree_func);
    btree_apply_prefix(heap->index, &debug_tree_func);
}
