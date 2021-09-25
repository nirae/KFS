; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    boot.asm                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/03/16 15:17:17 by ndubouil          #+#    #+#              ;
;    Updated: 2021/03/16 15:17:23 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits 32

section .multiboot
        dd 0x1BADB002               ; Magic number for bootloader
        dd 0x0                      ; Flags
        dd - (0x1BADB002 + 0x0)     ; Checksum

section .initial_stack, nobits
align 4
stack_bottom:
    ; 1mb of uninitialized data(1024*1024=104856)
    resb 104856
stack_top:

section .text
global start
extern main                         ; Main

start:
    cli                             ; Clear interrupts
    sti
    mov esp, stack_top            ; Stack pointer
    call main                       ; Main call
    jmp .1
.1:
    hlt
    ; hlt                           ; Halt CPU
