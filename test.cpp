// unit tests
#include "parse.h"
#include <cstdio>
#include <cstdlib>
#include <exception>

#define FAIL(reason) do {\
    printf("Expectation failed at %s:%d %s\n", __FILE__, __LINE__, (reason)); \
    std::exit(-1); \
} while (0)

#define EXPECT(cond, reason) do {\
    if (!(cond)) { \
        FAIL(reason); \
    } \
} while (0);

class TruncatedInput: public std::exception {};

auto mock_read_truncated_header(FILE* stream) ->uint32_t {
    throw TruncatedInput();
}

static void test_parse_header_on_empty_should_throw() {
    bool truncation_detected = false;
    try {
        parse_header(mock_read_truncated_header, nullptr);
    } catch (const TruncatedInput) {
        truncation_detected = true;
    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    EXPECT(truncation_detected,"Should not return normally on empty header");
}


int main() {
    test_parse_header_on_empty_should_throw();
    return 0;
}