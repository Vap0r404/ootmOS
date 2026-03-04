TARGET    := ootmOS
ARCH      := i386

ISO_DIR   := iso
BUILD_DIR := build

KERNEL_ELF := $(BUILD_DIR)/$(TARGET).elf
KERNEL_BIN := $(BUILD_DIR)/$(TARGET).bin

CC      := gcc
LD      := ld
AS      := gcc

CFLAGS  := -ffreestanding -fno-stack-protector -fno-pic -m32 -Wall -Wextra -std=gnu11
LDFLAGS := -nostdlib -m elf_i386 -z max-page-size=0x1000

SRCS_C := $(wildcard kernel/*.c)
SRCS_S := $(wildcard boot/*.s)
OBJS   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS_C)) \
          $(patsubst %.s,$(BUILD_DIR)/%.o,$(SRCS_S))

.PHONY: all clean run iso

all: $(KERNEL_BIN)

$(BUILD_DIR)/boot/%.o: boot/%.s
	mkdir -p $(dir $@)
	$(AS) -m32 -c $< -o $@

$(BUILD_DIR)/kernel/%.o: kernel/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJS)

$(KERNEL_BIN): $(KERNEL_ELF)
	objcopy -O elf32-i386 $< $@

iso: $(KERNEL_BIN)
	rm -rf $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/$(TARGET).bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso $(ISO_DIR)

run: iso
	qemu-system-x86_64 -cdrom $(TARGET).iso

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(TARGET).iso

