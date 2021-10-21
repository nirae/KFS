// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   scheduler.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2021/10/21 15:26:01 by ndubouil          #+#    #+#             */
// /*   Updated: 2021/10/21 16:16:23 by ndubouil         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "kmem.h"
// #include "process.h"
// #include "interrupts.h"
// #include "libk.h"

// // The start of the task linked list.
// t_task      *schedule_queue;
// t_task      *future_task;
// t_task      *current_task;

// extern t_mempage_directory *current_directory;

// t_process *get_current_task_process(void)
// {
//     return current_task->process;
// }

// t_task *create_task(t_process *process)
// {
//     t_task *task;

//     task = (t_task *)kmalloc_a(sizeof(t_task));
//     memset(task, 0, sizeof(t_task));
//     task->process = process;
//     return task;
// }

// void add_process_to_scheduler(t_process *process)
// {
//     t_task *task;
//     t_task *tmp;

//     task = create_task(process);
//     tmp = schedule_queue;
//     while (tmp->next) {
//         tmp = tmp->next;
//     }
//     tmp->next = task;
// }

// void remove_process_to_scheduler(t_process *process)
// {
//     t_task *tmp;
//     t_task *to_remove;
    
//     tmp = (t_process*)schedule_queue;
//     while (tmp->next) {
//         // If this is the task we want to remove
//         if (tmp->next->pid == process->pid) {
//             to_remove = tmp->next;
//             if (tmp->next->next) {
//                 tmp->next = tmp->next->next;
//             }
//             // kfree(to_remove);
//             break;
//         }
//         tmp = tmp->next;
//     }
// }

// void init_scheduler_list(t_process *first_process)
// {
//     current_task = create_task(first_process);
//     schedule_queue = current_task;
// }

// void print_task_list(void)
// {
//     t_task *tmp_task = schedule_queue;
//     printk("%d ", tmp_task->process->pid);
//     tmp_task = tmp_task->next;
//     while (tmp_task) {
//         printk("-> %d ", tmp_task->process->pid);
//         tmp_task = tmp_task->next;
//     }
//     printk("\n");
// }

// void switch_task(void)
// {
//     disable_interrupts();
//     // If we haven't initialised tasking yet, just return.
//     if (!current_task)
//         return;

//     // Read esp, ebp now for saving later on.
//     uint32 esp, ebp, eip;
//     asm volatile("mov %%esp, %0" : "=r"(esp));
//     asm volatile("mov %%ebp, %0" : "=r"(ebp));

//     // Read the instruction pointer. We do some cunning logic here:
//     // One of two things could have happened when this function exits - 
//     //   (a) We called the function and it returned the EIP as requested.
//     //   (b) We have just switched tasks, and because the saved EIP is essentially
//     //       the instruction after get_eip(), it will seem as if get_eip has just
//     //       returned.
//     // In the second case we need to return immediately. To detect it we put a dummy
//     // value in EAX further down at the end of this function. As C returns values in EAX,
//     // it will look like the return value is this dummy value! (0x12345).

//     if (current_task->next)
//         future_task = current_task->next;
//     else
//         future_task = schedule_queue;

//     eip = get_eip();

//     if (future_task->process->pid == current_task->process->pid) {
//         enable_interrupts();
//         return;
//     }

//     // Have we just switched tasks?
//     // if (eip == 0x12345)
//         // return;

//     // No, we didn't switch tasks. Let's save some register values and switch.
//     current_task->process->eip = eip;
//     current_task->process->esp = esp;
//     current_task->process->ebp = ebp;
//     // Get the next task to run.
//     current_task = current_task->next;
//     // If we fell off the end of the linked list start again at the beginning.
//     if (!current_task) current_task = schedule_queue;

//     if (current_task->process->status == 0)
//         return;

//     // Make sure the memory manager knows we've changed page directory.
//     current_directory = current_task->process->page_directory;
//     // Here we:
//     // * Stop interrupts so we don't get interrupted.
//     // * Temporarily puts the new EIP location in ECX.
//     // * Loads the stack and base pointers from the new task struct.
//     // * Changes page directory to the physical address (physicalAddr) of the new directory.
//     // * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
//     //   switched task.
//     // * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
//     //   the next instruction.
//     // * Jumps to the location in ECX (remember we put the new EIP in there).
//     // printk("switch on %d eip[%p], esp[%p], ebp[%p] dir [%p]\n", current_task->process->pid, eip, esp, ebp, current_directory->physical_address);
//     perform_task_switch(current_task->process->eip, current_directory->physical_address, current_task->process->ebp, current_task->process->esp);
//     enable_interrupts();
// }
