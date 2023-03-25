// parsed header
#pragma once
#include <cstdint>
#include <string>
#include<functional>
#include <cstdio>

using ReadDword = std::function <uint32_t (FILE* stream)>;

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
