// File format after https://datatracker.ietf.org/doc/id/draft-gharris-opsawg-pcap-00.html
#include "parse.h"

void parse_header( ReadDword dword_reader, FILE *f) {
    uint32_t header = dword_reader(f);
    (void)header;
}