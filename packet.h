//
#pragma once

#include <functional>
#include <cstdint>
#include <cstdio>
#include "timestamp.h"
#include "header.h"

using ReadDword = std::function <uint32_t (FILE* stream)>;

class PacketRecord {
    bool incomplete;
    timestamp_t ts;
    unsigned sub_sec_factor;

    public:
    PacketRecord(const ParsedHeader &ph);
    bool is_incomplete() {return incomplete;}
    void parse(ReadDword dword_reader, FILE *f);
    timestamp_t timestamp();
};
