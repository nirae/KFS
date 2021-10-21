/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 15:40:23 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 19:58:21 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "process.h"
#include "interrupts.h"
#include "libk.h"

// The currently running task.
// t_process *current_task;
t_process *parent_task;

// The start of the task linked list.
// t_process *ready_queue;
// t_process *future_task;

// int current_counter = 0;

// Some externs are needed to access members in paging.c...
extern t_mempage_directory *kernel_directory;
extern t_mempage_directory *current_directory;
extern uint32 first_esp;

// The next available process ID.
uint32 next_pid = 1;

t_process   *create_process(t_mempage_directory *directory)
{
    t_process *new = (t_process*)kmalloc_a(sizeof(t_process));
    memset(new, 0, sizeof(t_process));
    new->pid = next_pid;
    next_pid++;
    new->page_directory = directory;
    new->status = STATUS_ALIVE;

    return new;
}

// void        remove_process(t_process *process)
// {
//     kfree(process);
// }

void exit(void)
{
    if (get_current_task_process()->parent == 0)
        return;

    disable_interrupts();
    t_process *to_remove = get_current_task_process();
    to_remove->status = STATUS_DEAD;
    remove_process_to_scheduler(to_remove);
    printk("exit !\n");
    switch_task();
    enable_interrupts();
}

extern t_task      *schedule_queue;

void waitpid(int pid)
{
    t_task *tmp_task = schedule_queue;
    while (tmp_task) {
        if (tmp_task->process->pid == pid) {
            while (tmp_task->process->status == STATUS_ALIVE) {
            ;    // printk("%d waiting %d\n", getpid(), pid);
            };
            return;
        }
        tmp_task = tmp_task->next;
    }
}


int fork(void)
{
    // We are modifying kernel structures, and so cannot
    // TODO macro
    disable_interrupts();

    // Take a pointer to this process' task struct for later reference.
    parent_task = get_current_task_process();

    // Create a new process.
    t_process *new_task = create_process(clone_directory(current_directory));
    new_task->parent = parent_task;

    // Add it to the end of the ready queue.
    add_process_to_scheduler(new_task);
    // This will be the entry point for the new process.
    uint32 eip = get_eip();

    // We could be the parent or the child here - check.
    if (get_current_task_process()->pid == parent_task->pid)
    {
        // We are the parent, so set up the esp/ebp/eip for our child.
        GET_ESP(new_task->esp);
        GET_EBP(new_task->ebp);
        new_task->eip = eip;
        switch_task();
        // enable_interrupts();
        return new_task->pid;
    }
    else
    {
        // enable_interrupts();
        // We are the child.
        return 0;
    }

}

void debug_process(t_process *process)
{
    printk(
        "pid [%d]\nstatus [%d]\n esp [%p]\nebp [%d]\neip [%p]\n page_dir [%p]\nnext [%p]\nparent [%p]\n",
        process->pid,
        process->status,
        process->esp,
        process->ebp,
        process->eip,
        process->page_directory,
        process->next,
        process->parent
    );
}

void init_processes(void)
{ 
    disable_interrupts();
    relocate_stack((void *)STACK_LOCATION, STACK_SIZE);
    // Initialise the first task (kernel task)
    t_process *init = create_process(current_directory);
    init_scheduler_list(init);
    enable_interrupts();
}
