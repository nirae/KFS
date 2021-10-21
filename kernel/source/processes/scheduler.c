/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 15:26:01 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 20:10:03 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "process.h"
#include "interrupts.h"
#include "libk.h"

// The start of the task linked list.
t_task      *schedule_queue;
t_task      *future_task;
t_task      *current_task;

extern t_mempage_directory *current_directory;

t_process *get_current_task_process(void)
{
    return current_task->process;
}

int getpid(void)
{
    return current_task->process->pid;
}

int getstatus(void)
{
    return current_task->process->status;
}

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

t_task *create_task(t_process *process)
{
    t_task *task;

    task = (t_task *)kmalloc_a(sizeof(t_task));
    memset(task, 0, sizeof(t_task));
    task->process = process;
    return task;
}

void add_process_to_scheduler(t_process *process)
{
    t_task *task;
    t_task *tmp;

    task = create_task(process);
    tmp = schedule_queue;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = task;
}

void remove_process_to_scheduler(t_process *process)
{
    t_task *tmp;
    t_task *to_remove;
    
    tmp = schedule_queue;
    while (tmp->next) {
        // If this is the task we want to remove
        if (tmp->next->process->pid == process->pid) {
            to_remove = tmp->next;
            if (tmp->next->next) {
                tmp->next = tmp->next->next;
            } else {
                tmp->next = 0;
            }
            break;
        }
        tmp = tmp->next;
    }
}

void init_scheduler_list(t_process *first_process)
{
    current_task = create_task(first_process);
    schedule_queue = current_task;
}

void print_task_list(void)
{
    t_task *tmp_task = schedule_queue;
    printk("%d ", tmp_task->process->pid);
    tmp_task = tmp_task->next;
    while (tmp_task) {
        printk("-> %d ", tmp_task->process->pid);
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
        future_task = schedule_queue;

    eip = get_eip();

    if (future_task->process->pid == current_task->process->pid) {
        // enable_interrupts();
        return;
    }

    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->process->eip = eip;
    GET_ESP(current_task->process->esp);
    GET_EBP(current_task->process->ebp);

    // Get the next task to run.
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task)
        current_task = schedule_queue;

    if (current_task->process->status == STATUS_DEAD) {
        // enable_interrupts();
        return;
    }

    // Make sure the memory manager knows we've changed page directory.
    current_directory = current_task->process->page_directory;
    // printk("switch on %d eip[%p], esp[%p], ebp[%p] dir [%p]\n", current_task->pid, eip, esp, ebp, current_directory->physical_address);
    perform_task_switch(current_task->process->eip, current_directory->physical_address, current_task->process->ebp, current_task->process->esp);
    // enable_interrupts();
}
