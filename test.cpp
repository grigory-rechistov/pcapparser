// unit tests
#include "parse.h"
#include <cstdio>
#include <cstdlib>

#define FAIL(reason) do {\
    printf("Expectation failed at %s:%d %s\n", __FILE__, __LINE__, (reason)); \
    std::exit(-1); \
} while (0)

class TruncatedInput {};

static void test_parse_header_on_empty() {
    try {
        parse_header({});
    } catch (const TruncatedInput) {

    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    FAIL("Should not return normally on empty header");
}

int main() {
    test_parse_header_on_empty();
    return 0;
}