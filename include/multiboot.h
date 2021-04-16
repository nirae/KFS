/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiboot.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 15:10:15 by ndubouil          #+#    #+#             */
/*   Updated: 2021/04/15 17:01:03 by ndubouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIBOOT_H
# define MULTIBOOT_H

#include "kfs.h"

/* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html */
typedef struct  __attribute__((packed)) multiboot {
   uint32 magic;
   uint32 flags;
   uint32 checksum;
   uint32 header_addr;
   uint32 load_addr;
   uint32 load_end_addr;
   uint32 bss_end_addr;
   uint32 entry_addr;
   uint32 mode_type;
   uint32 width;
   uint32 height;
   uint32 depth;
}               t_multiboot;

#endif
