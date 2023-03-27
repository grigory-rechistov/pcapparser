// Pretty print formatting of more complex types
#pragma once
#include <string>
#include <cstdint>
#include <cstring>

static inline const std::string Int64NulltoStr(uint64_t v) {
    constexpr uint64_t null_repr = 0x8000000000000000ULL;
    if (v == null_repr) {
        return std::string("NULL (0x8000000000000000)");
    } else {
        int64_t signed_v = (int64_t)v;
        std::string sign(signed_v < 0? "-": "");
        int64_t magnitude = std::abs(signed_v); 
        char buf[20];
        snprintf(buf, sizeof(buf)-1, "%#lx", magnitude);
        return sign + std::string(buf);
    }
}

static inline const std::string MDEntryTypetoStr(uint8_t v) {
    switch (v) {
        case '0': return std::string("Bid");
        case '1': return std::string("Ask");
        case 'J': return std::string("Empty Book");
        default: return std::string("invalid");
    };
}
