#! /bin/bash

# C files
if [ -e sources.mk ]
then
    rm sources.mk
fi

for file in `find . -name '*.c'`
do
	printf "SRCS+=%s\n" $file >> sources.mk
done

chmod 755 sources.mk

# ASM files
if [ -e sources_asm.mk ]
then
    rm sources_asm.mk
fi

for file in `find ./gdt -name '*.asm'`
do
	printf "SRCS_ASM+=%s\n" $file >> sources_asm.mk
done

chmod 755 sources_asm.mk

# header files
if [ -e sources.mk ]
then
    rm headers.mk
fi

for file in `find ./include -name '*.h'`
do
	printf "HFILES+=%s\n" $file >> headers.mk
done

chmod 755 headers.mk
