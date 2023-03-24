// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
#include "parse.h"
#include "exc.h"


static constexpr uint32_t magic_sec_ms = 0xA1B2C3D4;
static constexpr uint32_t magic_sec_ns = 0xA1B23C4D;

void parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t magic = dword_reader(f);
    if (magic != magic_sec_ms && magic != magic_sec_ns) {
        throw BadMagic();
    }

    (void)magic;
}