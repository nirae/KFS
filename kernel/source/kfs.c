/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:17:27 by ndubouil          #+#    #+#             */
/*   Updated: 2021/10/21 20:24:52 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kfs.h"
#include "useless_shell.h"
#include "gdt.h"
#include "idt.h"
#include "kput.h"
#include "multiboot.h"
#include "debug.h"
#include "pit.h"
#include "rtc.h"
#include "kmem.h"
#include "heap.h"
#include "panic.h"
#include "keyboard.h"
#include "syscall.h"
#include "process.h"

uint32 first_esp = 0;

void    print_current_time(void)
{
    t_rtc_time rtc_time = get_rtc_time();
    printk(
        "%02d/%02d/%04d %02d:%02d:%02d", \
        rtc_time.day, \
        rtc_time.month, \
        rtc_time.year, \
        rtc_time.hour, \
        rtc_time.minute, \
        rtc_time.second \
    );
}

void    kfs(void)
{
    kputchar('\n', WHITE);
    kputstr("           :::     ::::::::        :::    ::: ::::::::: ::::::::: \n", CYAN);
    kputstr("         :+:     :+:    :+:       :+:   :+:  :+:       :+:    :+: \n", CYAN);
    kputstr("       +:+ +:+        +:+        +:+  +:+   +:+       +:+         \n", CYAN);
    kputstr("     +#+  +:+      +#+          +#++:++    :#::+::#  +#++:++#++   \n", CYAN);
    kputstr("   +#+#+#+#+#+  +#+            +#+  +#+   +#+              +#+    \n", CYAN);
    kputstr("        #+#   #+#             #+#   #+#  #+#       #+#    #+#     \n", CYAN);
    kputstr("       ###  ##########       ###    ### ###        ########       \n", CYAN);
    kputchar('\n', WHITE);
    kputstr("By: ndubouil\n", CYAN);
    kputchar('\n', WHITE);
    kputstr("The power of this kernel is huge, type something to feel it ! ", CYAN);
    kputchar(2, CYAN);
    kputstr("\n\n", WHITE);
    print_current_time();
    kputchar('\n', WHITE);
}

void    main(void)
{
    GET_ESP(first_esp);
    clear_screen();
    init_gdt();
    init_idt();
    init_paging();
    kfs();
    init_pit(50);
    init_rtc();
    init_keyboard();
    // init_syscalls();
    init_processes();

    // while (1) {};
    int p = fork();
    printk("fork %d -> pid %d -> status %d\n", p, getpid(), getstatus());
    if (p == 0) {
        printk("dans le fils p[%d] pid %d\n", p, getpid());
        // while(1) {};
        useless_shell();
        exit();
    }

    while(1) {};
    return;
}
