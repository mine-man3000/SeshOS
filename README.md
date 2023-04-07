## SeshOS
An extremely lightweight and optimized OS for UEFI systems.

```
 ____            _      ___  ____  
/ ___|  ___  ___| |__  / _ \/ ___| 
\___ \ / _ \/ __| '_ \| | | \___ \ 
 ___) |  __/\__ \ | | | |_| |___) |
|____/ \___||___/_| |_|\___/|____/ 
                                   
```

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Table of Contents

- [Current Status](#current-status)
- [**Disclaimer**](#disclaimer)
- [Requirements](#requirements)
- [Building](#building)
  - [Dependencies](#dependencies)
  - [Building](#building)
- [Todo](#todo)
  - [Plans for future](#i-plan-to-work-on)
  - [Need to do](#i-need-to-do)
- [License](#license)
- [Security](#security)
- [Credits](#credits)

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Current Status

| **Feature**        | **Status**           | **Notes**                                                                                     |
|--------------------|----------------------|-----------------------------------------------------------------------------------------------|
| 1               | Working              |                   |
| 2          | Working              |                                      |
| 3    | Working              |                                    |
| 4           | Working              |                                             | 
| 5    | Working              |                                                |
| 6  | Working              |   |
                                                                          


--------------------------------------------------------------------------------------------------------------------------------------------------------

### Disclaimer 

 > **Warning**:  By continuing, you acknowledge that you have read and understood the contents of [LICENSE.md](LICENSE.md) and the [disclaimer](#%EF%B8%8F-disclaimer-%EF%B8%8F), and consent to their terms.

 The instructions outlined in this GitHub repo have the potential to cause permanent harm to your computer, and you should be aware of this potential outcome before proceeding. I cannot be held accountable for any damage caused from following or disregarding these instructions. I make zero assurances concerning the dependability or efficacy of the materials referenced in this repository.

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Requirements
- 
- 
- 



--------------------------------------------------------------------------------------------------------------------------------------------------------

### Building
>**Note**: For Debian, Ubuntu, or any other distro with the APT package manager, make sure to run `sudo apt install build-essential qemu-system-x86 xorriso`

#### Dependencies 
- `gcc`
- `qemu-system-x86_64`
- `nasm`
- `qemu-uefi`
- `xorriso`
- `make`

#### Compiling 
- Execute `make run` to compile and run the OS.

--------------------------------------------------------------------------------------------------------------------------------------------------------

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


--------------------------------------------------------------------------------------------------------------------------------------------------------

### License
SeshOS is licensed under _.

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Security
>**Warning**: Do not submit security vunerablillity reports to Issues.

At SeshOS, we take security seriously. **Please report all security exploits to securityexploits@seshos.com**. 

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Credits

- mineman for being cool
- meghan06 for being cool
- donald trump
- sleepy joe
- hi
- people 





