# SeshOS
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
- [Issues](#known-issues)
- [Requirements](#requirements)
- [Features](#features)
- [Getting Started](#getting-started)
   - [Dependencies](#dependencies)
   - [Building](#compiling)
- [To Do](#to-do)
- [Frequently Asked Questions](#faq)
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

 > **Warning**:  By continuing, you acknowledge that you have read and understood the contents of [LICENSE.txt](LICENSE) and this disclaimer, and consent to their terms.

The contents in this GitHub repo have the potential to cause permanent harm to your computer, and you should be aware of this potential outcome before proceeding. The author and/or it's maintainers/contributors of this repo cannot be held accountable for any damage, loss of data, or other harm caused by using the contents of this repo. It is your responsibility to use the contents of this repo at your own risk, and to ensure that you fully understand what you are doing before proceeding.


--------------------------------------------------------------------------------------------------------------------------------------------------------

### Known Issues
-  
- 
-
-
 


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
To compile and run the OS, follow these steps:

1. Open the terminal and `cd` to the project directory.
2. Type `make run` to build the OS.

--------------------------------------------------------------------------------------------------------------------------------------------------------

### To Do

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

### FAQ
<details>
  <summary>Frequently Asked Questions</summary>


> **Q: What is SeshOS?**

SeshOS is an extremely lightweight and optimized operating system for UEFI systems.

> **Q: What are the requirements for installing SeshOS?**

To install SeshOS, you must have UEFI firmware and a 64-bit CPU. Basic Input Output System (BIOS) is not supported.

> **Q: Does SeshOS have any dependencies on Linux?**

No, SeshOS has no dependency on Linux.

> **Q: How can I compile and run SeshOS?**

You can compile and run SeshOS by executing the command `make run`. More information is provided under [here](#getting-started)

> **Q: How can I report security vulnerabilities?**

Please refer to [our security policy](SECURITY.md) for information on reporting security issues.

> **Q: Can I contribute to SeshOS?**

Yes, we welcome any feedback or contributions from our community. Please report any issues or make a pull request on our GitHub repository.

> **Q: Is there any documentation available for SeshOS?**

Yes, documentation is available on our GitHub repository, and we welcome contributions to improve it.

> **Q: Does SeshOS have a graphical user interface (GUI)?**

Yes, SeshOS has a basic GUI.

> **Q: What license is SeshOS released under?**

SeshOS is released under the GNU General Public License v3.0. Please see [LICENSE.txt](LICENSE) for more information.

> **Q: What if I encounter issues with SeshOS?**

Please report any issues or bugs in the Issues tab of our GitHub repository. We appreciate your support in improving SeshOS.
</details>



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
