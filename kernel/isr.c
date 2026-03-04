#include "isr.h"
#include "idt.h"
#include <stdint.h>

// Forward declaration of logging from kmain.c
void kprint(const char* s, uint8_t color);
void kprint_hex32(uint32_t value, uint8_t color);
uint8_t vga_color(uint8_t fg, uint8_t bg);

void fault_handler(regs_t* r) {
    // Simple red-on-blue exception panel.
    uint8_t bg = 0x1;
    uint8_t fg = 0xC; // light red

    kprint("=== EXCEPTION ===", vga_color(fg, bg));
    kprint("int_no:", vga_color(fg, bg));
    kprint_hex32(r->int_no, vga_color(fg, bg));
    kprint("err_code:", vga_color(fg, bg));
    kprint_hex32(r->err_code, vga_color(fg, bg));

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

