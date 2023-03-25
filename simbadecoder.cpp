// SIMBA packets partial decoder
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <cassert>
#include "parsedheader.h"
#include "exc.h"
#include "packet.h"
#include "order.h"

// TODO extract and unify helper functions
auto read_input_u32(FILE* stream) -> uint32_t {
    uint32_t res = 0;
    auto actual = fread(&res, sizeof(res), 1, stream);
    if (actual != 1) {
        throw TruncatedInput();
    }
    return res;
}

auto read_input_buffer(FILE* stream, size_t count, std::vector<uint8_t> &out) {
    assert(out.size() >= count);
    auto actual = fread(out.data(), 1, count, stream);
    out.resize(actual);
}

static auto make_binary_streams() {
    FILE *const in = fdopen(dup(fileno(stdin)), "rb");
    FILE *const out = stdout;
    return std::tuple{in, out};
}

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
            printf("OrderBookSnapshot found in: %s\n", pr.dump().c_str());
        }

        // fwrite(rd.data() rd.size(), 1, out);
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
