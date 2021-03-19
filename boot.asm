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

section .multiboot               ;according to multiboot spec
        dd 0x1BADB002            ;set magic number for bootloader
        dd 0x0                   ;set flags
        dd - (0x1BADB002 + 0x0)  ;set checksum

section .text
global start
extern main                      ; the C

start:
        cli                      ;block interrupts
        mov esp, stack_space     ;set stack pointer
        call main
        hlt                      ;halt the CPU

section .bss
resb 8192                        ; stack size : 8KB
; resb 4096                        ; stack size : 8KB
stack_space:
