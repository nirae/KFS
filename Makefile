# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndubouil <ndubouil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/16 15:17:17 by ndubouil          #+#    #+#              #
#    Updated: 2021/04/15 14:28:06 by ndubouil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include sources.mk
include sources_asm.mk
include headers.mk

CCYAN=\033[36m
CEND=\033[0m

CFLAGS = -m32 -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector -Wall -Wextra -g3
CC = /usr/bin/gcc
LD = /usr/bin/ld
NASM = /usr/bin/nasm

# Directories

H			=	include
ISO_DIR		=	iso

# Files

GRUB_CFG	=	grub/grub.cfg

BOOT		=	boot.asm
BOOT_OBJ	=	boot.o

LINKER		=	linker.ld

OBJS		=	$(patsubst %.c,%.o,$(SRCS))
OBJS		+=	$(patsubst %.asm,%.o,$(SRCS_ASM))

# Binary name

NAME		=	kfs
ISO			=	$(NAME).iso

.PHONY: all boot kernel linker iso clean run

all: boot $(OBJS) linker iso
	@true

boot: boot.asm
	@$(NASM) -f elf32 $(BOOT) -o $(BOOT_OBJ)

%.o: %.c $(HFILES)
	@echo "$(CCYAN)Creating $@ ...$(CEND)"
	@$(CC) $(CFLAGS) -I$(H) -c $< -o $@

%.o: %.asm
	@echo "$(CCYAN)Creating $@ ...$(CEND)"
	@$(NASM) -f elf32 -g -F dwarf $< -o $@

linker: $(LINKER) $(BOOT_OBJ) $(OBJS)
	@echo "$(CCYAN)Linking ...$(CEND)"
	@$(LD) -m elf_i386 -T $(LINKER) -o $(NAME) $(BOOT_OBJ) $(OBJS)
	@echo "$(CCYAN)$(NAME) binary is ready !$(CEND)"

iso:
	@echo "$(CCYAN)Creating the $(ISO_DIR) directory$(CEND)"
	@mkdir -pv $(ISO_DIR)/boot/grub
	@cp $(NAME) $(ISO_DIR)/boot
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@echo "$(CCYAN)Creating the $(ISO) file$(CEND)"
	@grub-file --is-x86-multiboot $(ISO_DIR)/boot/$(NAME)
	@grub-mkrescue -o $(ISO) $(ISO_DIR)

clean:
	@echo "$(CCYAN)Cleaning Objs ...$(CEND)"
	@rm -rf $(OBJS) $(BOOT_OBJ) $(ISO_DIR)

fclean: clean
	@echo "$(CCYAN)Cleaning kfs ...$(CEND)"
	@rm -rf $(NAME) $(ISO)

re: fclean all

run:
	qemu-system-i386 -s -cdrom $(ISO)
