#include "packet.h"
#include "exc.h"


PacketRecord::PacketRecord(const ParsedHeader &ph): incomplete(false), ts{}, 
    data{}, packet_length{} {
    if (ph.is_time_in_ns) {
        sub_sec_factor = 1;
    } else {
        sub_sec_factor = 1000;
    }
};

void PacketRecord::parse_header(ReadDword dword_reader, FILE *f) {

    try {
        uint32_t full_seconds = dword_reader(f);
        uint32_t sub_seconds = dword_reader(f); // to be decoded ms/ns
        uint32_t captured_length = dword_reader(f);
        uint32_t original_length = dword_reader(f);
        ts.s = full_seconds;
        ts.ns = sub_seconds * sub_sec_factor;
        packet_length = captured_length; // TODO validate against snap_len and original_length
        data.resize(captured_length);
    } catch (const TruncatedInput &e) {
        incomplete = true;
    }
}


timestamp_t PacketRecord::timestamp() {
    return ts;
}

void PacketRecord::read_raw_data(FillBuffer byte_filler, FILE *f) {
    if (incomplete) {
        return;
    }
    byte_filler(f, packet_length, data);
    if (data.size() < packet_length) {
        incomplete = true;
    }
}

const std::vector<uint8_t> PacketRecord::raw_data() {
    return data;
}

const std::string PacketRecord::dump() {
    if (data.size() == 0) {
        return std::string("");
    }
    std::string res{};
    for (const auto &c: data) {
        char hexpair[] = "   ";
        auto n1 = c >> 4;
        auto n2 = c & 0xfU;
        hexpair[0] = n1 < 10? n1 + '0': n1 + 'a' - 10;
        hexpair[1] = n2 < 10? n2 + '0': n2 + 'a' - 10;
        res.append(hexpair);
    }
    res.pop_back(); // the nasty trailing end space
    return res;

}