// Entrypoint for the main converter
// It takes stdin and gives stdout
// Compile with RAW_MODE defined to make it output binary instead of hexadecimal
#define RAW_MODE 1
#define UDP_HEADER_OFFSET 0 // set to 42 to exclude Ethernet/IPv4/UDP preamble

#include <cstdio>
#include <cstdint>
#include "parsedheader.h"
#include "exc.h"
#include "packet.h"
#include "io.h"

static bool decode_packet(ParsedHeader &ph, FILE *const in, FILE *const out) {
        PacketRecord pr(ph);
        pr.parse_header(read_input_u32, in);
        pr.read_raw_data(read_input_buffer, in);
        if (pr.is_incomplete()) {
            return true;
        }
#ifdef RAW_MODE
        auto rd = pr.raw_data();
        fwrite(rd.data()+UDP_HEADER_OFFSET, rd.size()-UDP_HEADER_OFFSET, 1, out);
#else
        printf("%s\n", pr.dump().c_str());
#endif
    return false;
}

int main() {
    auto [in, out] = make_binary_streams();
    
    ParsedHeader ph{};
    ph.parse_header(read_input_u32, in);

    bool last = false;
    do {
        last = decode_packet(ph, in, out);
    } while (!last);
    fclose(out);
    return 0;
}
