#! /bin/bash

# C files
if [ -e sources.mk ]
then
    rm sources.mk
fi

# for file in `ls -1 **/*.c`
for file in `find . -name '*.c'`
do
	printf "SRCS+=%s\n" $file >> sources.mk
done

chmod 755 sources.mk

# header files
if [ -e sources.mk ]
then
    rm headers.mk
fi

for file in `find ./include -name '*.h'`
# for file in `ls -1 include/**/*.h`
do
	printf "HFILES+=%s\n" $file >> headers.mk
done

chmod 755 headers.mk
