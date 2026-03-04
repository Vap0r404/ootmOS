TARGET    := ootmOS
ARCH      := x86_64

ISO_DIR   := iso
BUILD_DIR := build

KERNEL_ELF := $(BUILD_DIR)/$(TARGET).elf
KERNEL_BIN := $(BUILD_DIR)/$(TARGET).bin

CC      := x86_64-linux-gnu-gcc
LD      := x86_64-linux-gnu-ld
AS      := x86_64-linux-gnu-as

CFLAGS  := -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -m64 -Wall -Wextra -std=gnu11
LDFLAGS := -nostdlib -z max-page-size=0x1000

SRCS_C := $(wildcard kernel/*.c)
SRCS_S := $(wildcard boot/*.s)
OBJS   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS_C)) \
          $(patsubst %.s,$(BUILD_DIR)/%.o,$(SRCS_S))

.PHONY: all clean run iso

all: $(KERNEL_BIN)

$(BUILD_DIR)/boot/%.o: boot/%.s
	mkdir -p $(dir $@)
	$(AS) -m64 -o $@ $<

$(BUILD_DIR)/kernel/%.o: kernel/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJS)

$(KERNEL_BIN): $(KERNEL_ELF)
	x86_64-linuxssss-gnu-objcopy -O elf32-i386 $< $@

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

