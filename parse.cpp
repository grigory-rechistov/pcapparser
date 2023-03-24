// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
// TODO Code is assuming little endian. Big endian hosts are not handled
// correctly.
#include "parse.h"
#include "exc.h"
#include "header.h"
#include "bele.h"

static constexpr uint32_t magic_sec_ms = 0xA1B2C3D4;
static constexpr uint32_t magic_sec_ns = 0xA1B23C4D;

ParsedHeader parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t magic = reorder_u32(dword_reader(f));
    if (magic != magic_sec_ms && magic != magic_sec_ns) {
        throw BadMagic();
    }
    (void)magic;
    uint32_t major_minor = reorder_u32(dword_reader(f));
    uint32_t reserved1 = reorder_u32(dword_reader(f));
    uint32_t reserved2 = reorder_u32(dword_reader(f));
    uint32_t snap_len = reorder_u32(dword_reader(f));
    if (snap_len == 0) {
        throw ValueOutOfRange();
    }
    uint32_t fcs_f_link_type = reorder_u32(dword_reader(f));

    // TODO this should be in constructor for ParsedHeader
    ParsedHeader res{};
    res.is_time_in_ns = magic == magic_sec_ns;
    res.major_version = static_cast<uint16_t>(major_minor & 0xffffU);
    res.minor_version = static_cast<uint16_t>(major_minor >> 16);
    res.snap_len = snap_len;
    res.unparsed_link_type = fcs_f_link_type;
    return res;
}
