// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
// TODO Code is assuming little endian. Big endian hosts are not handled
// correctly.
#include "parse.h"
#include "exc.h"
#include "header.h"

static constexpr uint32_t magic_sec_ms = 0xA1B2C3D4;
static constexpr uint32_t magic_sec_ns = 0xA1B23C4D;

void ParsedHeader::parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t magic = dword_reader(f);
    if (magic != magic_sec_ms && magic != magic_sec_ns) {
        throw BadMagic();
    }
    (void)magic;
    uint32_t major_minor = dword_reader(f);
    uint32_t reserved1 = dword_reader(f);
    uint32_t reserved2 = dword_reader(f);
    uint32_t snap_len_raw = dword_reader(f);
    if (snap_len_raw == 0) {
        throw ValueOutOfRange();
    }
    uint32_t fcs_f_link_type = dword_reader(f);

    is_time_in_ns = magic == magic_sec_ns;
    major_version = static_cast<uint16_t>(major_minor & 0xffffU);
    minor_version = static_cast<uint16_t>(major_minor >> 16);
    snap_len = snap_len_raw;
    unparsed_link_type = fcs_f_link_type;
}


std::string ParsedHeader::dump() {
    using s = std::string;
    using std::to_string;
    // Assume C++20 is not available, this is inefficient, of course.
    return s("Header: ") 
        + s(this->is_time_in_ns ? "time in sec:ns" : "sec:ms")
        + s(" version: ") + to_string(major_version) + s(".") + to_string(minor_version)
        + s(" snap_len: ") + to_string(this->snap_len);
}