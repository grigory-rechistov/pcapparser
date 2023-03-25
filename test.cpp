// unit tests
#include "parse.h"
#include <cstdio>
#include <cstdlib>
#include <array>
#include <iostream>
#include "exc.h"
#include "header.h"
#include "bele.h"
#include "packet.h"
#include "timestamp.h"

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



static void test_parse_header_reads_whole_header() {
    size_t stream_pos = 0;
    const std::array<uint32_t, header_size_in_dwords> mock_stream = {
        reorder_u32(0xA1B2C3D4),
        0,
        0,
        0,
        1,
        0
    };

    auto mock_read_whole_header = [&mock_stream, &stream_pos] (FILE* stream) -> uint32_t {
        auto val = mock_stream.at(stream_pos);
        stream_pos += 1;
        return val;
    };

    parse_header(mock_read_whole_header, nullptr);
    EXPECT(stream_pos == header_size_in_dwords,
        "parse_header() should read whole header");
}

static void test_parse_header_on_zero_snap_len_throws() {
    size_t stream_pos = 0;
    const std::array<uint32_t, header_size_in_dwords> mock_bad_stream = {
        reorder_u32(0xA1B2C3D4),
        0,
        0,
        0,
        0, // zero SnapLen is violation
        0
    };

    auto mock_read_bad_snap_len = [&mock_bad_stream, &stream_pos] (FILE* stream) -> uint32_t {
        auto val = mock_bad_stream.at(stream_pos);
        stream_pos += 1;
        return val;
    };

    bool bad_snap_len_detected = false;
    try {
        parse_header(mock_read_bad_snap_len, nullptr);
    } catch (const ValueOutOfRange) {
        bad_snap_len_detected = true;
    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    EXPECT(bad_snap_len_detected,"Should have reported bad SnapLen in header");
}

static void test_header_values_land_in_correct_places() {
    size_t stream_pos = 0;
    const std::array<uint32_t, header_size_in_dwords> realistic_stream = {
        reorder_u32(0xA1B23C4D),
        reorder_u32(0x00040002),
        0,
        0,
        reorder_u32(1000),
        0
    };

    auto mock_read_whole_header = [&realistic_stream, &stream_pos] (FILE* stream) -> uint32_t {
        auto val = realistic_stream.at(stream_pos);
        stream_pos += 1;
        return val;
    };

    auto ph = parse_header(mock_read_whole_header, nullptr);
    EXPECT(ph.is_time_in_ns, "is_time_in_ns");
    EXPECT(ph.major_version == 2, "major_version");
    EXPECT(ph.minor_version == 4, "minor_version");
    EXPECT(ph.snap_len == 1000, "snap_len");

}

// static void test_reorder_u32() {
//     auto res = reorder_u32(0x11223344U);
//     EXPECT(res == 0x44332211U, "Octets are swapped");
// }

static void test_parsed_header_is_properly_dumped() {
    ParsedHeader h{};
    h.is_time_in_ns = true;
    h.major_version = 6;
    h.minor_version = 5;
    h.snap_len = 1234;
    auto dump_text = h.dump();
    const std::string ref_dump = "Header: time in sec:ns version: 6.5 snap_len: 1234";
    if (dump_text != ref_dump) {
        std::cout << "Expected: " << ref_dump << "\n"; 
        std::cout << "Got     : " << dump_text << "\n";
        FAIL("Dumped string difference");
    }
}

static void test_parse_packet_record_on_empty_header_returns_incomplete() {
    ParsedHeader ph {};
    PacketRecord pr(ph);
    try {
        pr.parse_header(mock_read_truncated_header, nullptr);
    } catch (const TruncatedInput) {
        FAIL("Should turn TruncatedInput to incomplete");
    } catch (...) {
        FAIL("Unexpected exception thrown");
    }
    EXPECT(pr.is_incomplete(),"Should be marked as incomplete");
}

static void test_parse_packet_record_handles_microseconds() {
    constexpr uint32_t ms = 5678;
    constexpr long ms_to_ns_factor = 1000;
    size_t stream_pos = 0;
    const std::array<uint32_t, 4> packet_header_stream = {
        reorder_u32(5555),
        reorder_u32(ms),
        0,
        0,
    };

    auto mock_read_whole_header = [&packet_header_stream, &stream_pos] (FILE* stream) -> uint32_t {
        auto val = packet_header_stream.at(stream_pos);
        stream_pos += 1;
        return val;
    };


    ParsedHeader h = {.is_time_in_ns = false};
    
    PacketRecord pr(h);
    pr.parse_header(mock_read_whole_header, nullptr);
    EXPECT(!pr.is_incomplete(),"Should be marked as complete");
    auto ts = pr.timestamp();
    EXPECT(ts.s == 5555, "Seconds are preserved"); 
    EXPECT(ts.ns == ms* ms_to_ns_factor,"microseconds are converted to nanoseconds");

}

static void test_packet_get_raw_data_on_truncated_data() {
    ParsedHeader ph {};
    PacketRecord pr(ph);
    constexpr size_t requested = 10;

    auto mock_read_insufficient_bytes = [&requested] (FILE* stream, size_t count, std::vector<uint8_t> &out) {
        
        out.resize(requested - 1);
        // out.at(0) = 0x11;
    };

    size_t header_stream_pos = 0;
    const std::array<uint32_t, 4> packet_header_stream = {
        0,
        0,
        reorder_u32(requested),
        0,
    };

    auto mock_read_whole_header = [&packet_header_stream, &header_stream_pos] (FILE* stream) -> uint32_t {
        auto val = packet_header_stream.at(header_stream_pos);
        header_stream_pos += 1;
        return val;
    };

    pr.parse_header(mock_read_whole_header, nullptr); // inject captured_length
    pr.read_raw_data(mock_read_insufficient_bytes, nullptr);
    auto res = pr.raw_data();
    EXPECT(pr.is_incomplete(), "Should be marked as incomplete");
    EXPECT(res.size() < requested, "Returned fewer octets than asked");
}

static void test_packet_get_raw_data_on_full_data() {
    ParsedHeader ph {};
    PacketRecord pr(ph);
    constexpr size_t requested = 10;

    auto mock_read_all_bytes = [&requested] (FILE* stream, size_t count, std::vector<uint8_t> &out) {
        EXPECT(requested == count, "unexpected read count");
        out.resize(requested);
        out.assign({1,2,3,4,5,6,7,8,9,10});
    };

    size_t header_stream_pos = 0;
    const std::array<uint32_t, 4> packet_header_stream = {
        0,
        0,
        reorder_u32(requested),
        0,
    };

    auto mock_read_whole_header = [&packet_header_stream, &header_stream_pos] (FILE* stream) -> uint32_t {
        auto val = packet_header_stream.at(header_stream_pos);
        header_stream_pos += 1;
        return val;
    };

    pr.parse_header(mock_read_whole_header, nullptr); // inject captured_length
    pr.read_raw_data(mock_read_all_bytes, nullptr);
    auto res = pr.raw_data();
    EXPECT(!pr.is_incomplete(), "Should be marked as complete");
    EXPECT(res.size() == requested, "Returned exactly how much asked");    
    EXPECT(res == std::vector<uint8_t>({1,2,3,4,5,6,7,8,9,10}), "Contents as expected");
}

int main() {
    test_parse_header_on_empty_should_throw();
    test_parse_header_on_bad_magic_should_throw();
    test_parse_header_reads_whole_header();
    test_parse_header_on_zero_snap_len_throws();
    test_header_values_land_in_correct_places();
    // test_reorder_u32();
    test_parsed_header_is_properly_dumped();
    test_parse_packet_record_on_empty_header_returns_incomplete();
    test_parse_packet_record_handles_microseconds();
    test_packet_get_raw_data_on_truncated_data();
    test_packet_get_raw_data_on_full_data();
    return 0;
}
