/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 15:35:20 by ndubouil          #+#    #+#             */
/*   Updated: 2021/09/24 18:26:04 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PROCESS_H
#define PROCESS_H

#include "kmem.h"

// This structure defines a 'task' - a process.
typedef struct          process
{
   int                  pid;                // Process ID.
   uint32               esp;
   uint32               ebp;       // Stack and base pointers.
   uint32               eip;            // Instruction pointer.
   t_mempage_directory  *page_directory; // Page directory.
   struct task          *next;     // The next task in a linked list.
}                       t_process;

extern uint32 get_eip();
extern void perform_task_switch(uint32, uint32, uint32, uint32);

// Initialises the tasking system.
void init_processes(void);

// Called by the timer hook, this changes the running process.
void switch_task(void);

// Forks the current process, spawning a new one with a different
// memory space.
int fork(void);

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, uint32 size);

// Returns the pid of the current process.
int getpid(void);

#endif
