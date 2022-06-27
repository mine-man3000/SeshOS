OSNAME = CustomOS

LDS = linker.ld
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -fno-exceptions -fno-stack-protector
ASMFLAGS = 
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := kernel
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)  
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)  
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

kernel: $(OBJS) link

prep:
	mkdir -vp bin
	mkdir -vp lib

$(OBJDIR)/idt/interrupts.o: $(SRCDIR)/idt/interrupts.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@
	 
link:
	@ echo !==== LINKING
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

run: clean prep iso
	qemu-system-x86_64 -smp 2 -m 1G image.iso -bios OVMF_CODE.fd -debugcon stdio

clean:
	rm -rvf lib bin image.iso initramfs
	make -C limine clean

initrd:
	tar -cvf initramfs --format=ustar -C rootfs/ .
	
iso: 
	make
	make -C limine
	mkdir -p iso_root

	make initrd

	cp -v bin/kernel.elf limine.cfg limine/limine.sys \
	      limine/limine-cd.bin limine/limine-cd-efi.bin \
		  initramfs iso_root/ 
 
	xorriso -as mkisofs -b limine-cd.bin \
	        -no-emul-boot -boot-load-size 4 -boot-info-table \
	        --efi-boot limine-cd-efi.bin \
	        -efi-boot-part --efi-boot-image --protective-msdos-label \
	        iso_root -o image.iso

	mv iso_root bin

	./limine/limine-deploy image.iso