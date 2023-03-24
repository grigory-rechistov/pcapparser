// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
// TODO this parser is explicitly little endian. Big endian inputs are not handled
// correctly.
#include "parse.h"
#include "exc.h"
#include "header.h"


static constexpr uint32_t magic_sec_ms = 0xA1B2C3D4;
static constexpr uint32_t magic_sec_ns = 0xA1B23C4D;

ParsedHeader parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t magic = dword_reader(f);
    if (magic != magic_sec_ms && magic != magic_sec_ns) {
        throw BadMagic();
    }
    (void)magic;
    uint32_t major_minor = dword_reader(f);
    uint32_t reserved1 = dword_reader(f);
    uint32_t reserved2 = dword_reader(f);
    uint32_t snap_len = dword_reader(f);
    if (snap_len == 0) {
        throw ValueOutOfRange();
    }
    uint32_t fcs_f_link_type = dword_reader(f);

    ParsedHeader res{};
    res.is_time_in_ns = magic == magic_sec_ns;
    return res;
}
