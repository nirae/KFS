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

section .text
global start
extern main                         ; Main

start:
        cli                         ; Clear interrupts
        mov esp, stack_space        ; Stack pointer
        call main                   ; Main call
        hlt                         ; Halt CPU

section .bss
; resb 8192                         ; stack size : 8KB
resb 4096                           ; stack size : 4KB
stack_space:
