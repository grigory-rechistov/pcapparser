// unit tests
#include "parse.h"
#include <cstdio>
#include <cstdlib>
#include <array>
#include "exc.h"

#define FAIL(reason) do {\
    printf("Expectation failed at %s:%d %s\n", __FILE__, __LINE__, (reason)); \
    std::exit(-1); \
} while (0)

#define EXPECT(cond, reason) do {\
    if (!(cond)) { \
        FAIL(reason); \
    } \
} while (0);


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

auto mock_read_bad_magic_header(FILE* stream) ->uint32_t {
    return 0xbaadc0de;
}

static void test_parse_header_on_bad_magic_should_throw() {
    bool bad_magic_detected = false;
    try {
        parse_header(mock_read_bad_magic_header, nullptr);
    } catch (const BadMagic) {
        bad_magic_detected = true;
    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    EXPECT(bad_magic_detected,"Should have reported bad magic in header");
}

static constexpr size_t header_size_in_dwords = 6;
// TODO convert to closure
static size_t mock_stream_pos = 0;
const std::array<uint32_t, header_size_in_dwords> mock_stream = {
    0xA1B2C3D4,
    0,
    0,
    0,
    1,
    0
};

auto mock_read_whole_header(FILE* stream) ->uint32_t {
    auto val = mock_stream.at(mock_stream_pos);
    mock_stream_pos += 1;
    return val;
}

static void test_parse_header_reads_whole_header() {
    mock_stream_pos = 0;
    parse_header(mock_read_whole_header, nullptr);
    EXPECT(mock_stream_pos == header_size_in_dwords,
        "parse_header() should read whole header");
}


static void test_parse_header_on_zero_snap_len_throws() {
    size_t stream_pos = 0;
    const std::array<uint32_t, header_size_in_dwords> mock_bad_stream = {
        0xA1B2C3D4,
        0,
        0,
        0,
        0, // zero SnapLen is violation
        0
    };

    auto mock_read_bad_magic_header = [&mock_bad_stream, &stream_pos] (FILE* stream) -> uint32_t {
        auto val = mock_bad_stream.at(stream_pos);
        stream_pos += 1;
        return val;
    };

    bool bad_snap_len_detected = false;
    try {
        parse_header(mock_read_bad_magic_header, nullptr);
    } catch (const ValueOutOfRange) {
        bad_snap_len_detected = true;
    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    EXPECT(bad_snap_len_detected,"Should have reported bad SnapLen in header");
}

int main() {
    test_parse_header_on_empty_should_throw();
    test_parse_header_on_bad_magic_should_throw();
    test_parse_header_reads_whole_header();
    test_parse_header_on_zero_snap_len_throws();
    return 0;
}
