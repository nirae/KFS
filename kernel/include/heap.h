/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 13:54:54 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 19:09:41 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

#include "kfs.h"
#include "ordered_array.h"
#include "btree.h"

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE     0x20000
#define HEAP_MAGIC          0x123890AB
#define HEAP_MIN_SIZE       0x70000

#define BLOCK               0
#define HOLE                1

#define NO_HOLE_FOUND       -1

typedef struct      header
{
    struct header   *parent;
    struct header   *left;
    struct header   *right;
    uint32          magic;   // Magic number, used for error checking and identification.
    uint8           type;   // 1 if this is a hole. 0 if this is a block.
    uint32          size;    // size of the block, including the end footer.
}                   t_header;

typedef struct  footer
{
    uint32      magic;     // Magic number, same as in t_header.
    t_header    *header; // Pointer to the block header.
}               t_footer;

typedef struct  heap
{
    t_header    *index;
    uint32      index_start_address; // The start of our allocated space.
    uint32      start_address; // The start of our allocated space.
    uint32      end_address;   // The end of our allocated space. May be expanded up to max_address.
    uint32      max_address;   // The maximum address the heap can be expanded to.
    uint8       supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
    uint8       readonly;       // Should extra pages requested by us be mapped as read-only?
}               t_heap;



/* memory/heap/decrease_heap.c */
uint32      decrease_heap(t_heap *heap, uint32 new_size);
/* memory/heap/increase_heap.c */
void        increase_heap(t_heap *heap, uint32 new_size);
/* memory/heap/create_heap.c */
t_heap      *create_heap(uint32 start, uint32 end, uint32 max, uint8 supervisor,
    uint8 readonly);
/* memory/heap/find_best_hole.c */
t_header    *find_best_hole(t_heap *heap, uint32 size, uint8 align);
/* memory/heap/heap_allocation.c */
void        *heap_allocation(uint32 size, uint8 page_align, t_heap *heap);
/* memory/heap/heap_deallocation.c */
void        heap_deallocation(void *p, t_heap *heap);

/* memory/heap/t_header.c */
int         t_header_size_compare(void *a, void *b);
/* memory/heap/t_header.c */
void        debug_heap_tree(t_heap *heap);

/* memory/heap/t_header.c */
t_header *new_header(void *addr, uint32 size, uint8 type);
/* memory/heap/t_header.c */
t_footer *new_footer(t_header *header);
/* memory/heap/t_header.c */
void *remove_header(t_heap *heap, t_header *header);

void print_kheap_tree(void);
uint32 kmalloc_a(uint32 sz);
uint32 kmalloc_p(uint32 sz, uint32 *phys);
uint32 kmalloc_ap(uint32 sz, uint32 *phys);
uint32 kmalloc(uint32 sz);
void kfree(void *p);


#endif
