#include <stdint.h>
#include "multiboot.h"
#include "idt.h"

static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

uint8_t vga_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_clear(uint8_t color) {
    uint16_t entry = vga_entry(' ', color);
    for (int y = 0; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            VGA_BUFFER[y * VGA_WIDTH + x] = entry;
        }
    }
}

static void vga_write_at(const char* s, int row, int col, uint8_t color) {
    int i = 0;
    while (s[i] != '\0') {
        int x = col + i;
        if (x >= VGA_WIDTH) {
            break;
        }
        VGA_BUFFER[row * VGA_WIDTH + x] = vga_entry(s[i], color);
        i++;
    }
}

static const int LOG_ROW_START = 1;
static const int LOG_ROW_END   = 9;  // keep logs in rows 1-9 (banner starts lower)

static int log_row = LOG_ROW_START;

void kprint(const char* s, uint8_t color) {
    if (log_row > LOG_ROW_END) {
        log_row = LOG_ROW_END;
    }
    vga_write_at(s, log_row, 2, color);
    log_row++;
}

void kprint_hex32(uint32_t value, uint8_t color) {
    char buf[11];
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 8; ++i) {
        uint8_t nibble = (value >> ((7 - i) * 4)) & 0xF;
        buf[2 + i] = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
    }
    buf[10] = '\0';
    kprint(buf, color);
}

void kmain(uint32_t magic, uint32_t mb_info_addr) {
    // Colors: 0x0 = black, 0x1 = blue, 0x5 = magenta, 0xF = white
    uint8_t bg = 0x1;  // deep blue
    uint8_t fg = 0xF;  // bright white
    uint8_t accent = 0x5; // magenta accent

    vga_clear(vga_color(fg, bg));
    log_row = LOG_ROW_START;

    kprint("booting ootmOS...", vga_color(fg, bg));
    kprint("handing off from GRUB, entering kernel space.", vga_color(fg, bg));

    kprint("multiboot magic:", vga_color(fg, bg));
    kprint_hex32(magic, vga_color(accent, bg));

    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        multiboot_info_t* mbi = (multiboot_info_t*)(uintptr_t)mb_info_addr;

        kprint("multiboot info @", vga_color(fg, bg));
        kprint_hex32(mb_info_addr, vga_color(accent, bg));

        if (mbi->flags & MULTIBOOT_INFO_MEM) {
            kprint("mem_lower (KB):", vga_color(fg, bg));
            kprint_hex32(mbi->mem_lower, vga_color(accent, bg));

            kprint("mem_upper (KB):", vga_color(fg, bg));
            kprint_hex32(mbi->mem_upper, vga_color(accent, bg));
        } else {
            kprint("multiboot: no basic mem info flag.", vga_color(fg, bg));
        }
    } else {
        kprint("multiboot magic mismatch!", vga_color(fg, bg));
    }

    kprint("this one is yours. make it weird.", vga_color(accent, bg));

    idt_init();

    const char* title = "ootmOS";
    const char* subtitle = "one of the many, but this one's yours.";
    const char* hint = "legacy BIOS boot | GRUB | Multiboot1";

    vga_write_at(title, 11, 35, vga_color(accent, bg));
    vga_write_at(subtitle, 13, 18, vga_color(fg, bg));
    vga_write_at(hint, 15, 22, vga_color(fg, bg));

    // Hang the CPU.
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

