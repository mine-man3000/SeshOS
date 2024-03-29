# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := bin/kernel.elf
 
# Convenience macro to reliably declare overridable command variables.
define DEFAULT_VAR =
	ifeq ($(origin $1),default)
		override $(1) := $(2)
	endif
	ifeq ($(origin $1),undefined)
		override $(1) := $(2)
	endif
endef
 
# It is highly recommended to use a custom built cross toolchain to build a kernel.
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
$(eval $(call DEFAULT_VAR,CC,cc))
 
# Same thing for "ld" (the linker).
$(eval $(call DEFAULT_VAR,LD,ld))
 
# User controllable CFLAGS.
CFLAGS ?= -g -Wall -Wextra -pipe
 
# User controllable preprocessor flags. We set none by default.
#CPPFLAGS ?= -Wno-c++20-extensions -fno-threadsafe-statics
 
# User controllable nasm flags.
NASMFLAGS ?= -F dwarf -g
 
# User controllable linker flags. We set none by default.
LDFLAGS ?= 
 
# Internal C flags that should not be changed by the user.
override CFLAGS +=       \
	-std=gnu2x           \
	-I.                  \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-stack-check     \
	-fno-pie             \
	-fno-pic             \
	-m64                 \
	-march=x86-64        \
	-mabi=sysv           \
	-mno-80387			 \
	-mno-mmx             \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone        \
	-mcmodel=kernel      \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-address \
	-Wno-int-to-pointer-cast \
	-Wno-pointer-arith \
	-Wno-write-strings \
	-Wno-cast-function-type \
	-Wno-return-type \
	-Iinclude/ \
	-include include/forceInclude.h

# Internal linker flags that should not be changed by the user.
override LDFLAGS +=         \
	-nostdlib               \
	-static                 \
	-z max-page-size=0x1000 \
	-T linker.ld
 
# Internal nasm flags that should not be changed by the user.
override NASMFLAGS += \
	-f elf64

# Use find to glob all *.c, *.S, and *.asm files in the directory and extract the object names.
#override CPPFILES  := $(shell cd kernel && find . -type f -name '*.cpp')
override NASMFILES := $(shell cd kernel && find . -type f -name '*.asm')
override CFILES    := $(shell cd kernel && find . -type f -name '*.c')
#override OBJ_CPP   := $(subst .cpp,.o,$(CPPFILES))
override OBJ_C     := $(subst .c,.o,$(CFILES))
override OBJ_ASM   := $(subst .asm,.o,$(NASMFILES))
override OBJ       := $(subst ./,build/kernel/,$(OBJ_ASM) $(OBJ_C)) build/font.f16.bin.o

#test:
#	@echo $(OBJ)
 
# Default target.
.PHONY: all
all: $(KERNEL) 

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	@mkdir -p $(@D) 
	@echo "   LD    $(subst build/,,$(subst ./,,$(subst kernel/,,$(OBJ)))) ==> $@"
	@$(LD) $(subst ./,build/,$(OBJ)) $(LDFLAGS) -o $@
 
# Compilation rules for *.c files.
# build/%.o: %.cpp
#	@mkdir -p $(@D)
#	@echo "   CXX   $@"
#	@g++ $(CPPFLAGS) $(CFLAGS) -c $< -o $@
 
build/%.o: %.c
	@mkdir -p $(@D)
	@echo "   CC    $@"
	@gcc $(CFLAGS) -std=gnu2x -c $< -o $@

# Compilation rules for *.asm (nasm) files.
build/%.o: %.asm
	@mkdir -p $(@D)
	@echo "   NASM  $@"
	@nasm $(NASMFLAGS) $< -o $@

build/font.f16.bin.o: vgafont.f16
	@$(info $s   PERL  vgafont.f16 ==> font.f16.c)
	@perl tools/navcc.perl vgafont.f16 > build/font.f16.c
	@$(info $s   CC    font.f16.c ==> font.bin.f16.o)
	@$(CC) -c -Wno-gnu-binary-literal -std=gnu2x build/font.f16.c -o build/font.f16.bin.o

# Remove object files and the final executable.
.PHONY: clean
clean:
	@rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS) limine initramfs iso_root/ bin/*
	@echo "Cleaned!"

initramfs:
	@tar -cf initramfs --format=ustar -C rootfs/ .

limine.h:
	@wget https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -q

iso: $(kernel)
	@make limine.h
	@make
	@make initramfs
	@rm -rf limine/
	@git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1 --quiet
	
	@mkdir -p iso_root
 
	@cp bin/kernel.elf initramfs limine.cfg limine/*.sys \
	  limine/*.bin iso_root/
 
	@./tools/iso.sh

test.disk:
	qemu-img create test.disk 1G

run: iso test.disk
	@qemu-system-x86_64 -hda test.disk -cdrom bin/image.iso -bios ./OVMF_CODE.fd -debugcon stdio -m 1G -smp 3 --enable-kvm -M q35

debug: iso test.disk
	@qemu-system-x86_64 -hda test.disk -cdrom bin/image.iso -bios ./OVMF_CODE.fd -debugcon stdio -m 1G -d int -D log.txt -no-reboot -no-shutdown  -M q35