//
#pragma once

#include <functional>
#include <cstdint>
#include <cstdio>

using ReadDword = std::function <uint32_t (FILE* stream)>;

class PacketRecord {
    bool incomplete;

    public:
    PacketRecord(): incomplete(false) {};
    bool is_incomplete() {return incomplete;}
    void parse(ReadDword dword_reader, FILE *f);
};
