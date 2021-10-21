/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 15:35:20 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 17:58:14 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PROCESS_H
#define PROCESS_H

#include "kmem.h"

#define STATUS_ALIVE    1
#define STATUS_ZOMBIE   2
#define STATUS_DEAD     0

// typedef struct          task
// {
//     struct process      *process;
//     struct process      *next;
//     struct process      *prev;
// }                       t_task;

// This structure defines a 'task' - a process.
typedef struct          process
{
   int                  pid;                // Process ID.
   int                  status;
   uint32               esp;
   uint32               ebp;       // Stack and base pointers.
   uint32               eip;            // Instruction pointer.
   t_mempage_directory  *page_directory; // Page directory.
   struct process       *next;     // The next task in a linked list.
   struct process       *prev;     // The next task in a linked list.
   struct process       *parent;
//    struct process       *childs;
//    struct process       *next_bro;
//    struct process       *prev_bro;
}                       t_process;

extern uint32 get_eip(void);
extern void perform_task_switch(uint32, uint32, uint32, uint32);
extern void tlb_flush(void);

// Initialises the tasking system.
// void init_processes(void);
void init_processes(void);

// Called by the timer hook, this changes the running process.
void switch_task(void);

// Forks the current process, spawning a new one with a different
// memory space.
int fork(void);

void exit(void);
void waitpid(int pid);

// Causes the current process' stack to be forcibly moved to a new location.
void relocate_stack(void *new_start, uint32 size);

void debug_process(t_process *process);
t_process *get_current_process(void);


// Returns the pid of the current process.
int getpid(void);
int getstatus(void);
void print_task_list(void);

/* Scheduler */
// void init_scheduler_list(t_process *first_process);
// void remove_process_to_scheduler(t_process *process);
// void add_process_to_scheduler(t_process *process);
// void switch_task(void);
// t_process *get_current_task_process(void);

#endif
