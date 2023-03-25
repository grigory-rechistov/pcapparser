#include "io.h"
#include <unistd.h>
#include <tuple>
#include <cassert>
#include "exc.h"

uint32_t read_input_u32(FILE* stream) {
    uint32_t res = 0;
    auto actual = fread(&res, sizeof(res), 1, stream);
    if (actual != 1) {
        throw TruncatedInput();
    }
    return res;
}

void read_input_buffer(FILE* stream, size_t count, std::vector<uint8_t> &out) {
    assert(out.size() >= count);
    auto actual = fread(out.data(), 1, count, stream);
    out.resize(actual);
}

std::tuple<FILE *const, FILE *const> make_binary_streams() {
    FILE *const in = fdopen(dup(fileno(stdin)), "rb");
    FILE *const out = fdopen(dup(fileno(stdout)), "wb");
    return std::tuple{in, out};
}
