// Endiannes conversions
#pragma once

#include <cstdint>

static inline uint32_t reorder_u32(uint32_t v) {
    uint32_t b1 = v & 0xffU;
    uint32_t b2 = (v >> 8) & 0xffU;
    uint32_t b3 = (v >> 16) & 0xffU;
    uint32_t b4 = (v >> 24) & 0xffU;
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}