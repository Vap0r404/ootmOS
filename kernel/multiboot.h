#pragma once

#include <stdint.h>

// Multiboot1 loader magic value passed in EAX
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002u

// Multiboot1 info flags
#define MULTIBOOT_INFO_MEM      0x00000001u

// Minimal subset of the Multiboot information structure.
// Layout must match the spec; we only use the first few fields for now.
typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    // The real structure continues, but we don't need the rest yet.
} multiboot_info_t;

