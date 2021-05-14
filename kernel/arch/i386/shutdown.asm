; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    shutdown.asm                                      :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/04/15 12:04:38 by ndubouil          #+#    #+#              ;
;    Updated: 2021/04/15 12:04:38 by ndubouil         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; BIOS shutdown

global shutdown

shutdown:
  mov ax, 0x1000
  mov ax, ss
  mov sp, 0xf000
  mov ax, 0x5307
  mov bx, 0x0001
  mov cx, 0x0003
  int 0x15
 
  ret
