/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:17:27 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/30 11:01:20 by ndubouil         ###   ########.fr       */
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
    uint32 esp;
    uint32 ebp;
    GET_ESP(esp);
    GET_EBP(ebp);
    char test[34] = "Je test la stack de mon kernel lol";

    clear_screen();
    init_gdt();
    init_idt();
    init_paging();

    /* Test page faulting */
    // uint32 *ptr = (uint32*)0xA0000000;
    // uint32 do_page_fault = *ptr;

    kfs();

    char *teststr = kmalloc(1000);
    strncpy(teststr, "yo je test le kmalloc", 999);
    printk("kmalloc 1000: %s, size: %d\n", teststr, kget_size(teststr));
    print_kheap_tree();
    kfree(teststr);

    char *teststr2 = kmalloc(10);
    strncpy(teststr, "test2", 9);
    printk("kmalloc 10: %s, size: %d\n", teststr2, kget_size(teststr2));
    print_kheap_tree();
    kfree(teststr2);

    init_pit(100);
    init_rtc();

    kputchar('\n', WHITE);
    kdump(esp, ebp - esp);
    kputchar('\n', WHITE);

    useless_shell();
    return;
}
