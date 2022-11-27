## BUILDING

install gcc, nasm, qemu-system-x86_64, qemu-uefi, xorriso, and make

for the debian/ubuntu/distros with APT, run ``sudo apt install build-essential qemu-system-x86 xorriso``

then run ``make run`` and it will build and run the OS.

## TODO

What I want to do: 
- [X] Booting 
- [X] Basic GUI 
- [X] Mouse Detection 
- [X] Keyboard Driver
- [X] Terminal
- [ ] Port *Something*
- [X] Make boot partition FAT32
- [X] Get initrd working

What I need to do:
- [X] Load an IDT so that exceptions and interrupts can be handled.
- [X] Memory Management 
- [ ] Scheduler 
- [X] Start up the other CPU cores
- [ ] VFS
- [ ] Elf loader
- [ ] Userland