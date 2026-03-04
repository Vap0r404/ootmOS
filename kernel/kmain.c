#include <stdint.h>

static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

static uint8_t vga_color(uint8_t fg, uint8_t bg) {
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

void kmain(void) {
    // Colors: 0x0 = black, 0x1 = blue, 0x5 = magenta, 0xF = white
    uint8_t bg = 0x1;  // deep blue
    uint8_t fg = 0xF;  // bright white
    uint8_t accent = 0x5; // magenta accent

    vga_clear(vga_color(fg, bg));

    const char* title = "ootmOS";
    const char* subtitle = "one of the many, but this one's yours.";
    const char* hint = "legacy BIOS boot | GRUB | Multiboot2";

    vga_write_at(title, 10, 35, vga_color(accent, bg));
    vga_write_at(subtitle, 12, 18, vga_color(fg, bg));
    vga_write_at(hint, 14, 22, vga_color(fg, bg));

    // Hang the CPU.
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

