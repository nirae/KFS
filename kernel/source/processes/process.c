/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 15:40:23 by ndubouil          #+#    #+#             */
/*   Updated: 2021/09/25 20:00:25 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kmem.h"
#include "process.h"
#include "interrupts.h"

// The currently running task.
t_process *current_task;
t_process *parent_task;

// The start of the task linked list.
t_process *ready_queue;
t_process *previous_task;

// Some externs are needed to access members in paging.c...
extern t_mempage_directory *kernel_directory;
extern t_mempage_directory *current_directory;
extern uint32 first_esp;

// The next available process ID.
uint32 next_pid = 1;

int fork(void)
{
    // We are modifying kernel structures, and so cannot
    // TODO macro
    disable_interrupts();

    // Take a pointer to this process' task struct for later reference.
    parent_task = current_task;

    // Clone the address space.
    t_mempage_directory *directory = clone_directory(current_directory);

    // Create a new process.
    t_process *new_task = (t_process*)kmalloc_a(sizeof(t_process));

    new_task->pid = next_pid;
    next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    // Add it to the end of the ready queue.
    t_process *tmp_task = (t_process*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    // This will be the entry point for the new process.
    uint32 eip = get_eip();


    // We could be the parent or the child here - check.
    if (current_task->pid == parent_task->pid)
    {
        // We are the parent, so set up the esp/ebp/eip for our child.
        uint32 esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        uint32 ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        enable_interrupts();
        asm volatile("sti");

        return new_task->pid;
    }
    else
    {
        enable_interrupts();
        // We are the child.
        return 0;
    }

}



void switch_task(void)
{
    // If we haven't initialised tasking yet, just return.
    if (!current_task)
        return;

    // Read esp, ebp now for saving later on.
    uint32 esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    // Read the instruction pointer. We do some cunning logic here:
    // One of two things could have happened when this function exits - 
    //   (a) We called the function and it returned the EIP as requested.
    //   (b) We have just switched tasks, and because the saved EIP is essentially
    //       the instruction after get_eip(), it will seem as if get_eip has just
    //       returned.
    // In the second case we need to return immediately. To detect it we put a dummy
    // value in EAX further down at the end of this function. As C returns values in EAX,
    // it will look like the return value is this dummy value! (0x12345).
    if (current_task->next)
        previous_task = current_task->next;
    eip = get_eip();

    // Have we just switched tasks?
    // if (eip == 0x12345)
    if (previous_task->pid == current_task->pid)
        return;

    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;
    
    // Get the next task to run.
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task) {
        current_task = ready_queue;
    }

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    // Make sure the memory manager knows we've changed page directory.
    current_directory = current_task->page_directory;
    // Here we:
    // * Stop interrupts so we don't get interrupted.
    // * Temporarily puts the new EIP location in ECX.
    // * Loads the stack and base pointers from the new task struct.
    // * Changes page directory to the physical address (physicalAddr) of the new directory.
    // * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    //   switched task.
    // * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    //   the next instruction.
    // * Jumps to the location in ECX (remember we put the new EIP in there).
    // if (!eip || !esp || !ebp) {
        // return;
    // }
    printk("switch on %d eip[%d], esp[%d], ebp[%d] dir [%p]\n", current_task->pid, eip, esp, ebp, current_directory->physical_address);
    perform_task_switch(eip, current_directory->physical_address, ebp, esp);
}

int getpid(void)
{
    return current_task->pid;
}

void init_processes(void)
{ 
    disable_interrupts();
    // Initialise the first task (kernel task)
    current_task = ready_queue = (t_process *)kmalloc(sizeof(t_process));
    current_task->pid = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;    
    enable_interrupts();
}