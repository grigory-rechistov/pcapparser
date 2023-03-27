// Pretty printing of primitive types
#pragma once
#include <string>
#include <cstdint>

static inline const std::string Int64NulltoStr(uint64_t v) {
    constexpr uint64_t null_repr = 0x8000000000000000ULL;
    if (v == null_repr) {
        return std::string("NULL (0x8000000000000000)");
    } else {
        int64_t signed_v = (int64_t)v;
        return std::string("lol");
    }
}
