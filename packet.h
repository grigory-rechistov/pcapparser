// Individual packets after the file header 
#pragma once
#include <cstdint>
#include <cstdio>
#include "timestamp.h"
#include "parsedheader.h"
#include "io.h"

class PacketRecord {
    bool incomplete;
    timestamp_t ts;
    unsigned sub_sec_factor;
    std::vector<uint8_t> data;
    size_t packet_length;

    public:
    PacketRecord(const ParsedHeader &ph);
    bool is_incomplete() {return incomplete;}
    void parse_header(ReadDword dword_reader, FILE *f);
    timestamp_t timestamp();
    void read_raw_data(FillBuffer byte_filler, FILE *f);
    const std::vector<uint8_t> raw_data();
    const std::string dump();
    const std::vector<uint8_t> payload();
};
