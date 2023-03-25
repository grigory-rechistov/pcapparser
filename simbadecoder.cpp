// SIMBA packets partial decoder
#include <cstdint>
#include "parsedheader.h"
#include "exc.h"
#include "packet.h"
#include "order.h"
#include "io.h"

static bool decode_packet(ParsedHeader &ph, FILE *const in, FILE *const out) {
        PacketRecord pr(ph);
        pr.parse_header(read_input_u32, in);
        pr.read_raw_data(read_input_buffer, in);
        if (pr.is_incomplete()) {
            return true;
        }

        auto payload = pr.payload();

        auto orderbooksnapshot = OrderBookSnapshot(payload);
        if (orderbooksnapshot.is_valid()) {
            printf("%s\n", orderbooksnapshot.dump().c_str());
        }
        /* TODO Add similar sensing for OrderExecution and OrderUpdate here */

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
