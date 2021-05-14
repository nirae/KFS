# KFS

Current 42 project : **KFS-2**

![Build](https://github.com/nirae/KFS/workflows/build/badge.svg)

>  Discover the world of "Kernel Programming" and learn how to create your very own Kernel from scratch.

32bit very simple kernel

Functionalities:

- Boot (yes!)
- VGA printing
- GDT + stack dump function
- IDT - ISR + IRQ
- Keyboard management
- PIT
- RTC time

![screen](images/screen-kfs-2.png)

## KFS-1

### TODO

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

## KFS-2

### TODO

- Create a Global Descriptor Table
- The GDT must contain:
    - Kernel Code
    - Kernel Data
    - Kernel stack
    - User code
    - User data
    - User stack
- Declare the GDT to the BIOS
- The GDT must be set at address 0x00000800
- Code a tool to print the kernel stack, in a human-friendly way


**Bonus**

- [x] Code a minimalistic shell with a few commands, for debugging pur-poses.
- For example :
    - [x] the print-kernel-stack-thing
    - [x] reboot
    - [x] halt
    - [x] more

![screen](images/screen-kfs-2-2.png)

## Compile

```
./generate_sources.sh
make
```

## QEMU run

```
make run
```

## Resources

The Bible: https://wiki.osdev.org/Main_Page

James Molloy: http://www.jamesmolloy.co.uk/tutorial_html

With the osdev complement: https://wiki.osdev.org/James_Molloy's_Tutorial_Known_Bugs

Brandon Friesen - osdever: http://www.osdever.net/bkerndev/Docs/intro.htm

introduction : https://www.linuxjournal.com/content/what-does-it-take-make-kernel-0

bad : https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C-2

https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html
