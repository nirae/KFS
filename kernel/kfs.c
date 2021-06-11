/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:17:27 by ndubouil          #+#    #+#             */
/*   Updated: 2021/06/11 19:10:16 by ndubouil         ###   ########.fr       */
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

    // uint32 *ptr = (uint32*)0xA0000000;
    // uint32 do_page_fault = *ptr;

    // uint32 a = kmalloc(8);
    // printk("a: %p=%x\n", a, a);
    // void *a = kmalloc(8);
    // uint32 b = kmalloc(8);
    // printk("b: %p=%x\n", b, b);
    // kfree(a);
    // kfree(b);
    // uint32 c = kmalloc(8);
    // printk("c: %p=%x\n", c, c);

    // void *d = kmalloc(PAGE_SIZE * 2);
    // printk("d: %p=%x\n", d, d);

    print_kheap_tree();
    // printk("get smallest hole : %p\n", find_best_hole(kheap, 8, 0));
    char *teststr = kmalloc(1000);
    strncpy(teststr, "yo je test le kmalloc", 999);
    printk("kmalloc: %s\n", teststr);
    print_kheap_tree();
    kfree(teststr);
    printk("apres le free\n");
    print_kheap_tree();
    char *teststr2 = kmalloc(1000);
    strncpy(teststr, "yo je test le kmalloc 2", 999);
    printk("kmalloc: %s\n", teststr2);
    print_kheap_tree();

    init_pit(100);
    init_rtc();
    // kfs();

    kputchar('\n', WHITE);
    kdump(esp, ebp - esp);
    kputchar('\n', WHITE);

    useless_shell();
    return;
}
