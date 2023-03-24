// parsed header
#pragma once
#include <cstdint>

struct ParsedHeader {
    bool is_time_in_ns;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t snap_len;
    uint32_t link_type;
    bool fcs_valid;
    uint8_t fcs;
};
