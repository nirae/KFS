; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    interrupts.asm                                     :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/04/15 12:04:38 by ndubouil          #+#    #+#              ;
;    Updated: 2021/04/15 12:04:38 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; from interrupts/irq.c
extern irq_handler

%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47 

irq_common_stub:
    pusha               ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds          ; Lower 16-bits of eax = ds.
    push eax            ; save the data segment descriptor
    mov ax, 0x10        ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler
    pop ebx             ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa                ; Pops edi,esi,ebp...
    add esp, 8          ; Cleans up the pushed error code and pushed ISR number
    sti
    iret                ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
