// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
#include "parse.h"
#include "exc.h"


static constexpr uint32_t magic_sec_ms = 0xA1B2C3D4; 

void parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t header = dword_reader(f);
    if (header != magic_sec_ms) {
        throw BadMagic();
    }

    (void)header;
}