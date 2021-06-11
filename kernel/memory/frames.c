/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frames.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 17:18:42 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/08 11:09:08 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "kput.h"

/*
 *  Frames -> Equally sized blocks of physical memory
 *  bitmap -> 1: used, 0: free
 */
uint32 *frames;
uint32 nframes;

/*
 *  Set a bit in the frames bitmap
 */
static void set_frame(uint32 addr)
{
    frames[BITMAP_INDEX(addr / 0x1000)] |= (0x1 << BITMAP_OFFSET(addr / 0x1000));
}

/*
 *  Unset a bit in the frames bitmap
 */
static void unset_frame(uint32 addr)
{
    frames[BITMAP_INDEX(addr / 0x1000)] &= ~(0x1 << BITMAP_OFFSET(addr / 0x1000));
}

/*
 *  Test if a bit is set in the frames bitmap
 */
static uint32 test_frame(uint32 addr)
{
   return (frames[BITMAP_INDEX(addr / 0x1000)] & (0x1 << BITMAP_OFFSET(addr / 0x1000)));
}

static uint32 get_first_free_frame(void)
{
   uint32 i = 0;
   uint32 j = 0;

   for (i = 0; i < BITMAP_INDEX(nframes); i++) {
        /* if no available space */
        if (frames[i] == 0xFFFFFFFF) {
            continue;
        }
        for (j = 0; j < (sizeof(uint32) * 8); j++) {
            if (!(frames[i] & 0x1 << j)) {
                return i * sizeof(uint32) * 8 + j;
            }
        }
   }
   return 0;
}

void init_frames(void)
{
    nframes = PHYS_MEM_SIZE / 0x1000;
    frames = (uint32 *)kmalloc(BITMAP_INDEX(nframes));
    memset(frames, 0, BITMAP_INDEX(nframes));
}

void alloc_frame(t_mempage *page, int is_kernel, int is_writeable)
{
    uint32 free_frame = 0;

    /* if already allocated */
    if (page->frame != 0) {
        return;
    }
    else {
        free_frame = get_first_free_frame();
        if (free_frame == (uint32)-1) {
            // PANIC is just a macro that prints a message to the screen then hits an infinite loop.
            //    PANIC("No free frames!");
            printk("PANIC no free frames\n");
            while (1) {};
        }
        set_frame(free_frame * 0x1000);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = free_frame;
    }
}

void free_frame(t_mempage *page)
{
    uint32 frame = page->frame;

    /* if the page haven't an allocated frame */
    if (!frame) {
        return;
    }
    unset_frame(frame);
    page->frame = 0;
}
