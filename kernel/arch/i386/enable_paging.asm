; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    enable_paging.asm                                  :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/04/15 12:04:38 by ndubouil          #+#    #+#              ;
;    Updated: 2021/04/15 12:04:38 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

global enable_paging

enable_paging:
    mov eax, [esp+4]    ; 1st parameter : pointer to the page directory
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000  ; set the PG bit
    mov cr0, eax
    ret
