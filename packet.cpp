#include "packet.h"
#include "bele.h"
#include "exc.h"


void PacketRecord::parse(ReadDword dword_reader, FILE *f) {

    try {
        uint32_t full_seconds = reorder_u32(dword_reader(f));
        uint32_t sub_seconds = reorder_u32(dword_reader(f)); // to be decoded ms/ns
        uint32_t captured_length = reorder_u32(dword_reader(f));
        uint32_t original_length = reorder_u32(dword_reader(f));
    } catch (const TruncatedInput &e) {
        incomplete = true;
    }
}