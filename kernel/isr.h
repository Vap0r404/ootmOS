#pragma once

#include <stdint.h>

typedef struct regs {
    uint32_t int_no;
    uint32_t err_code;
} regs_t;

void fault_handler(regs_t* r);

