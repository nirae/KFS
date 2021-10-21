/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relocate_stack.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 13:34:01 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 17:23:41 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "process.h"
#include "interrupts.h"
#include "libk.h"

extern uint32 first_esp;
extern t_mempage_directory *current_directory;

static void alloc_stack(void *new_start, uint32 size)
{
    t_mempage       *page;
    uint32          i;
    
    i = (uint32)new_start;
    while (i >= ((uint32)new_start - size)) {
        page = get_page(i, current_directory);
        if (!page) {
            page = create_page(i, current_directory);
        }
        alloc_frame(page, 0, 1);
        i -= PAGE_SIZE;
    }
}

void relocate_stack(void *new_start, uint32 size)
{
    uint32 old_esp = 0;
    uint32 old_ebp = 0;
    uint32 new_esp = 0;
    uint32 new_ebp = 0;
    uint32 offset = (uint32)new_start - first_esp;

    /* Allocate the space for the new stack */
    alloc_stack(new_start, size);

    tlb_flush();

    GET_ESP(old_esp);
    GET_EBP(old_ebp);
    /* Calculate the new stack pointers */
    new_esp = old_esp + offset;
    new_ebp  = old_ebp  + offset;
    /* Copy the stack */
    memcpy((void *)new_esp, (void *)old_esp, first_esp - old_esp);
    // Backtrace through the original stack, copying new values into
    // the new stack.  
    uint32 i = (uint32)new_start;
    while (i > (uint32)new_start - size) {
        uint32 tmp = *(uint32*)i;
        // If the value of tmp is inside the range of the old stack, assume it is a base pointer
        // and remap it. This will unfortunately remap ANY value in this range, whether they are
        // base pointers or not.
        if ((old_esp < tmp) && (tmp < first_esp)) {
            tmp = tmp + offset;
            uint32 *tmp2 = (uint32*)i;
            *tmp2 = tmp;
        }
        i -= 4;
    }
    /* Set the new stack */
    SET_ESP(new_esp);
    SET_EBP(new_ebp);
}
