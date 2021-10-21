/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 15:40:23 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 19:36:27 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "process.h"
#include "interrupts.h"
#include "libk.h"

// The currently running task.
t_process *current_task;
t_process *parent_task;

// The start of the task linked list.
t_process *ready_queue;
t_process *future_task;

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
    if (current_task->parent == 0)
        return;

    disable_interrupts();

    t_process *to_remove = current_task;
    to_remove->status = STATUS_DEAD;

    t_process *tmp_task = (t_process*)ready_queue;
    while (tmp_task) {
        // If this is the task we want to remove
        if (tmp_task->pid == to_remove->pid) {
            if (tmp_task->prev) {
                tmp_task->prev->next = tmp_task->next;
            }
            if (tmp_task->next) {
                tmp_task->next->prev = tmp_task->prev;
            }
            break;
        }
        tmp_task = tmp_task->next;
    }
    printk("exit !\n");
    switch_task();
    enable_interrupts();
}

void waitpid(int pid)
{
    // t_process *tmp_task = (t_process*)current_task;
    // if (pid == getpid())
        // return;
    t_process *tmp_task = (t_process*)ready_queue;
    while (tmp_task) {
        if (tmp_task->pid == pid) {
            while (tmp_task->status == 1) {
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
    parent_task = current_task;

    // Create a new process.
    t_process *new_task = create_process(clone_directory(current_directory));
    new_task->parent = parent_task;

    // Add it to the end of the ready queue.
    t_process *tmp_task = (t_process *)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;
    new_task->prev = tmp_task;
    // This will be the entry point for the new process.
    uint32 eip = get_eip();

    // We could be the parent or the child here - check.
    if (current_task->pid == parent_task->pid)
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

void print_task_list(void)
{
    t_process *tmp_task = ready_queue;
    printk("%d ", tmp_task->pid);
    tmp_task = tmp_task->next;
    while (tmp_task) {
        printk("-> %d ", tmp_task->pid);
        tmp_task = tmp_task->next;
    }
    printk("\n");
}

void switch_task()
{
    // disable_interrupts();
    // If we haven't initialised tasking yet, just return.
    if (!current_task)
        return;

    uint32 eip;

    if (current_task->next)
        future_task = current_task->next;
    else
        future_task = ready_queue;

    eip = get_eip();

    if (future_task->pid == current_task->pid) {
        // enable_interrupts();
        return;
    }

    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->eip = eip;
    GET_ESP(current_task->esp);
    GET_EBP(current_task->ebp);

    // Get the next task to run.
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task)
        current_task = ready_queue;

    if (current_task->status == STATUS_DEAD) {
        // enable_interrupts();
        return;
    }

    // Make sure the memory manager knows we've changed page directory.
    current_directory = current_task->page_directory;
    // printk("switch on %d eip[%p], esp[%p], ebp[%p] dir [%p]\n", current_task->pid, eip, esp, ebp, current_directory->physical_address);
    perform_task_switch(current_task->eip, current_directory->physical_address, current_task->ebp, current_task->esp);
    // enable_interrupts();
}

int getpid(void)
{
    return current_task->pid;
}

int getstatus(void)
{
    return current_task->status;
}

t_process *get_current_process(void)
{
    return current_task;
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
    current_task = create_process(current_directory);
    ready_queue = current_task;
    enable_interrupts();
}
