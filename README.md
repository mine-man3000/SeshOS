## SeshOS
A lightweight and optimized OS for UEFI systems.

---------------------------------------------------------------------------------------------------------------------------------------------------------

### Table of Contents

- [Building](#building)
  - [Dependencies](#dependencies)
  - [Building](#building)
- [TODO](#todo)
  - [Plans for future](#i-plan-to-work-on)
  - [Need to do](#i-need-to-do)
- [License](#license)
- [Security](#security)
- [Credits](#credits)

---------------------------------------------------------------------------------------------------------------------------------------------------------

### Building
>**Note**: For the Debian, Ubuntu, or any other distro with the APT package manager, make sure to run `sudo apt install build-essential qemu-system-x86 xorriso`

#### Dependencies 
- gcc 
- qemu-system-x86_64
- nasm
- qemu-uefi
- xorriso
- make

#### Compiling 
- Run `make run` to compile and run the OS.

---------------------------------------------------------------------------------------------------------------------------------------------------------

### Todo

#### I plan to work on: 
- [X] Booting 
- [X] Basic GUI 
- [X] Mouse Detection 
- [X] Keyboard Driver
- [X] Terminal
- [ ] Port *Something*
- [X] Make boot partition FAT32
- [X] Get initrd working

#### I need to do:
- [X] Load an IDT so that exceptions and interrupts can be handled.
- [X] Memory Management 
- [ ] Scheduler 
- [X] Start up the other CPU cores
- [ ] VFS
- [ ] Elf loader
- [ ] Userland


---------------------------------------------------------------------------------------------------------------------------------------------------------

### License
SeshOS is licensed under _.

---------------------------------------------------------------------------------------------------------------------------------------------------------

### Security
>**Warning**: Do not submit security vunerablillity reports to Issues.

At SeshOS, we take security seriously. **Please report all security exploits to securityexploits@seshos.com**. 

---------------------------------------------------------------------------------------------------------------------------------------------------------

### Credits

- mineman for being cool
- meghan06 for being cool
- donald trump
- sleepy joe
- hi
- people 





