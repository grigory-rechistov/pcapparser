#pragma once
#include <cstdint>
#include <string>
#include <cstdio>
#include "io.h"

static constexpr size_t header_size_in_dwords = 6;
struct ParsedHeader {
    bool is_time_in_ns;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t snap_len;
    uint32_t unparsed_link_type; // TODO extract FCS if needed

    std::string dump();
    void parse_header(ReadDword dword_reader, FILE *f);
};

