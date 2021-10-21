; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    tlb_flush.asm                                      :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/10/21 14:04:38 by ndubouil          #+#    #+#              ;
;    Updated: 2021/10/21 14:04:38 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

global tlb_flush

; Flush the Translation Lookaside Buffer (TLB)
; wiki.osdev.org/TLB
; The TLB is not transparently informed of changes made to paging structures
; Therefore the TLB has to be flushed upon such a change
; On x86 systems, this can be done by writing to the page directory base register (CR3)
tlb_flush:
    mov eax, cr3
    mov cr3, eax
    xor eax, eax
    ret
