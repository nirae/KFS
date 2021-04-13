# KFS - 1

![Build](https://github.com/nirae/KFS-1/workflows/build/badge.svg)

>  Discover the world of "Kernel Programming" and learn how to create your very own Kernel from scratch.

32bit very simple kernel, just boot, print and catch keyboard input

![screen](images/screen.png)

# TODO

- A kernel you can boot via GRUB
- An ASM bootable base
- A basic kernel library, with basics functions and types
- Some basic code to print some stuff on the screen
- A basic "Hello world" kernel

**Bonus**

- [x] Add scroll and cursor support to your I/O interface.
- [x] Add colors support to your I/O interface.
- [x] Add helpers like printf / printk in order to print information / debug easily.
- [x] Handle keyboard entries and print them.
- [ ] Handle different screens, and keyboard shortcuts to switch easily between then.

# Compile

```
./generate_sources.sh
make
```

# QEMU run

```
make run
```
# Resources

introduction : https://www.linuxjournal.com/content/what-does-it-take-make-kernel-0

bad : https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C-2

http://www.jamesmolloy.co.uk/tutorial_html

http://www.osdever.net/bkerndev/Docs/intro.htm

https://wiki.osdev.org/Main_Page

