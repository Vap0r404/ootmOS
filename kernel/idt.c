#include "idt.h"
#include <stdint.h>

extern void isr0();

static idt_entry_t idt[256];
static idt_ptr_t   idtp;

static void idt_set_gate(uint8_t num, uintptr_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (uint16_t)(base & 0xFFFF);
    idt[num].base_high = (uint16_t)((base >> 16) & 0xFFFF);
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idtp.limit = (uint16_t)(sizeof(idt_entry_t) * 256 - 1);
    idtp.base = (uint32_t)(uintptr_t)&idt[0];

    for (int i = 0; i < 256; ++i) {
        idt[i].base_low = 0;
        idt[i].base_high = 0;
        idt[i].sel = 0x08; // kernel code segment, assuming a flat descriptor
        idt[i].always0 = 0;
        idt[i].flags = 0x8E; // present, ring 0, 32-bit interrupt gate
    }

    idt_set_gate(0, (uintptr_t)isr0, 0x08, 0x8E);

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idtp));
}

