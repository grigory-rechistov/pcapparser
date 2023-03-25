// entrypoint for the main converter
// It takes stdin and gives stdout
// Compile with RAW_MODE defined to make it output binary instead of hexadecimal
#define RAW_MODE 1

#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <cassert>
#include "parse.h"
#include "exc.h"
#include "packet.h"

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

int main() {
    FILE *const in = fdopen(dup(fileno(stdin)), "rb");
    FILE *const out = fdopen(dup(fileno(stdout)), "wb");
    
    ParsedHeader ph{};
    ph.parse_header(read_input_u32, in);

    while (true) {
        PacketRecord pr(ph);
        pr.parse_header(read_input_u32, in);
        pr.read_raw_data(read_input_buffer, in);
        if (pr.is_incomplete()) {
            break;
        }
#ifdef RAW_MODE
        auto rd = pr.raw_data();
        fwrite(rd.data(), rd.size(), 1, out);
#else
        printf("%s\n", pr.dump().c_str());
#endif
    }
    fclose(out);
    return 0;
}
