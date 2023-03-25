// entrypoint for the main converter
// it takes stdin and gives stdout
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include "parse.h"
#include "exc.h"

auto read_input(FILE* stream) -> uint32_t {
    uint32_t res = 0;
    auto actual = fread(&res, sizeof(res), 1, stream);
    if (actual != 1) {
        throw TruncatedInput();
    }
    return res;
}

int main() {
    FILE *const in = fdopen(dup(fileno(stdin)), "rb");
    FILE *const out = fdopen(dup(fileno(stdout)), "wb");
    
    ParsedHeader ph = parse_header(read_input, in);


    fclose(out);
    return 0;
}
