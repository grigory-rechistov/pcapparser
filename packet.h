//
#pragma once

#include <functional>
#include <cstdint>
#include <cstdio>
#include "timestamp.h"

using ReadDword = std::function <uint32_t (FILE* stream)>;

class PacketRecord {
    bool incomplete;
    timestamp_t ts;

    public:
    PacketRecord(): incomplete(false), ts{} {};
    bool is_incomplete() {return incomplete;}
    void parse(ReadDword dword_reader, FILE *f);
    timestamp_t timestamp();
};
