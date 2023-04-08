## SeshOS
An extremely lightweight and optimized OS for UEFI systems.

[![License](https://img.shields.io/badge/license-GPL-blue)](https://www.gnu.org/licenses/gpl-3.0.en.html) [![Status](https://user-images.githubusercontent.com/77316348/230705808-40c7ba6b-9b4f-41fb-8c40-8e7db3b97ad0.png)](https://github.com/mine-man3000/SeshOS)

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
- [Features](#features)
- [Getting Started](#getting-started)
   - [Dependencies](#dependencies)
   - [Building](#compiling)
- [ToDo](#todo)
- [Contributing](#contributing)
- [Security](#security)
- [License](#license)
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

 > **Warning**:  By continuing, you acknowledge that you have read and understood the contents of [LICENSE.txt](LICENSE) and the [disclaimer](#disclaimer), and consent to their terms.

The contents in this GitHub repo have the potential to cause permanent harm to your computer, and you should be aware of this potential outcome before proceeding. I cannot be held accountable for any damage, nuclear war, market crashes, or planet collision caused from these instructions. I make zero (0) assurances concerning the dependability or efficacy of the materials referenced in this GitHub repository.

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Requirements
To be able to install SeshOS on your computer, you must have:
- UEFI firmware
   - **BIOS is unsupported**
- 64 bit CPU 


--------------------------------------------------------------------------------------------------------------------------------------------------------

### Features
- No dependency on Linux.
- Lightweight
- no telemetry :sunglasses: 

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Getting Started

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

- [X] Booting 
- [X] Basic GUI 
- [X] Mouse Detection 
- [X] Keyboard Driver
- [X] Terminal
- [ ] Port *Something*
- [X] Make boot partition FAT32
- [X] Get initrd working
- [X] Load an IDT so that exceptions and interrupts can be handled.
- [X] Memory Management 
- [ ] Scheduler 
- [X] Start up the other CPU cores
- [ ] VFS
- [ ] Elf loader
- [ ] Userland


--------------------------------------------------------------------------------------------------------------------------------------------------------

### Contributing
>**Warning**: Do not submit security vulnerability reports to the Issues tab.

Want to contribute? We love hearing from our community and welcome any feedback or contributions. 

If you come across any issues with SeshOS, please let us know by reporting it in the Issues tab in this repo. Thank you for your support in improving SeshOS.


--------------------------------------------------------------------------------------------------------------------------------------------------------

### Security
>**Warning**: Do not submit security vulnerability reports to the Issues tab.

Please refer to [our security policy](SECURITY.md) for reporting security issues.

--------------------------------------------------------------------------------------------------------------------------------------------------------

### License
Copyright (c) mine-man3000. All rights reserved.

SeshOS is licensed under the [GNU General Public License v3.0](LICENSE)

--------------------------------------------------------------------------------------------------------------------------------------------------------

### Credits

- [mineman](https://github.com/mine-man3000) for writing this
- [meghan06](https://github.com/meghan06) for writing documentation
