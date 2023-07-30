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
CFLAGS ?= -g -Wall -Wextra -Wpedantic -pipe
 
# User controllable preprocessor flags. We set none by default.
CPPFLAGS ?= -std=c++20 -Wno-c++20-extensions -fno-threadsafe-statics
 
# User controllable nasm flags.
NASMFLAGS ?= -F dwarf -g
 
# User controllable linker flags. We set none by default.
LDFLAGS ?= 
 
# Internal C flags that should not be changed by the user.
override CFLAGS +=       \
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
	-Wno-return-type

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
override CPPFILES  := $(shell cd kernel && find . -type f -name '*.cpp')
override NASMFILES := $(shell cd kernel && find . -type f -name '*.asm')
override CFILES    := $(shell cd kernel && find . -type f -name '*.c')
override OBJ_CPP   := $(subst .cpp,.o,$(CPPFILES))
override OBJ_C     := $(subst .c,.o,$(CFILES))
override OBJ_ASM   := $(subst .asm,.o,$(NASMFILES))
override OBJ       := $(subst ./,build/kernel/,$(OBJ_CPP) $(OBJ_ASM) $(OBJ_C))

#test:
#	@echo $(OBJ)
 
# Default target.
.PHONY: all
all: $(KERNEL)

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	@mkdir -p $(@D) 
	@echo "   LD   $(subst ./,,$(subst kernel/,,$(OBJ))) ==> $@"
	@$(LD) $(subst ./,build/,$(OBJ)) $(LDFLAGS) -o $@
 
# Compilation rules for *.c files.
build/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "   CXX   $@"
	@g++ $(CPPFLAGS) $(CFLAGS) -c $< -o $@
 
build/%.o: %.c
	@mkdir -p $(@D)
	@echo "   CC    $@"
	@gcc $(CFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
build/%.o: %.asm
	@mkdir -p $(@D)
	@echo "   NASM  $@"
	@nasm $(NASMFLAGS) $< -o $@
 
# Remove object files and the final executable.
.PHONY: clean
clean:
	@rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS) limine initramfs iso_root/ bin/*
	@echo "Cleaned!"

initramfs:
	@tar -cf initramfs --format=ustar -C rootfs/ .

limine.h:
	@wget https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -q

iso:
	rm -rf limine/
	@make limine.h
	@make
	@make initramfs
	@git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1 --quiet
	
	@mkdir -p iso_root
 
	@cp bin/kernel.elf initramfs font limine.cfg limine/*.sys \
	  limine/*.bin iso_root/
 
	@./tools/iso.sh

run: iso
	@qemu-system-x86_64 -cdrom bin/image.iso -bios ./OVMF_CODE.fd -debugcon stdio -m 1G -smp 3 --enable-kvm

debug: iso
	@qemu-system-x86_64 -cdrom bin/image.iso -bios ./OVMF_CODE.fd -debugcon stdio -m 1G -d int -D log.txt -no-reboot -no-shutdown