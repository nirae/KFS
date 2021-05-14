; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    idt_flush.asm                                      :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/04/15 12:04:38 by ndubouil          #+#    #+#              ;
;    Updated: 2021/04/15 12:04:38 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

global idt_flush

idt_flush:
    mov eax, [esp+4]    ; 1st parameter : pointer to the IDT
    lidt [eax]
    ret
