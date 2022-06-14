## BUILDING

install gcc, nasm, qemu-system-x86_64, qemu-uefi, and make

then run ``make run`` and it will build and run the OS.

## TODO

What I want to do: 
- [X] Booting 
- [ ] Basic GUI 
- [ ] Mouse Detection 
- [X] Keyboard Driver
- [X] Terminal
- [ ] Port *Something*
- [X] Make boot partition FAT32
- [X] Get initrd working

What I need to do:
- [X] Load an IDT so that exceptions and interrupts can be handled.
- [ ] Memory Management 
- [ ] Begin parsing ACPI tables, the most important one is the MADT since it contains information about the APIC.
- [ ] Start up the other CPUs. stivale2 provides a facility to make this less painful.
- [ ] Set up an interrupt controller such as the APIC.
- [ ] Configure the PIT.
- [ ] Implement a scheduler to schedule threads in order make multitasking possible.
- [ ] Design a virtual file system (VFS) and implement it. The traditional UNIX VFS works  when porting software
- [ ] Decide how to abstract devices. UNIX likes usually go for a /dev virtual filesystem 
- [ ] Get a userland going by loading executables from your VFS and running them in ring 3. Set up a way to run sys calls.
- [ ] Write a PCI driver.
- [ ] Add support for a storage medium, the easiest and most common ones are AHCI and NVMe# SeshOS-UEFI-Better
