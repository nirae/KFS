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
global mboot
extern code
extern bss
extern end

align 4
mboot:
    MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
    MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
    MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
    ; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
    ; pass us a symbol table.
    MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
    MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)
    dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                    ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct
    
    dd  mboot                     ; Location of this descriptor
    dd  code                      ; Start of kernel '.text' (code) section.
    dd  bss                       ; End of kernel '.data' section.
    dd  end                       ; End of kernel.
    dd  start                     ; Kernel entry point (initial EIP).

; section .multiboot
;         dd 0x1BADB002               ; Magic number for bootloader
;         dd 0x0                      ; Flags
;         dd - (0x1BADB002 + 0x0)     ; Checksum

; section .text
global start
extern main                         ; Main

start:
    cli                         ; Clear interrupts
    sti
    xor ebp, ebp
    mov esp, stack_space        ; Stack pointer
    push ebx
    call main                   ; Main call
    jmp .1
.1:
    hlt
    ; hlt                         ; Halt CPU

section .bss
    ; resb 8192                         ; stack size : 8KB
    resb 4096                           ; stack size : 4KB
stack_space:
