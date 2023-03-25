// Sensing and parsing of OrderBookSnapshot etc.
#include "order.h"

OrderBookSnapshot::OrderBookSnapshot(const std::vector<uint8_t> &payload): 
    valid(false) {
    if (payload.size() != 92) {
        return;
    }
    if (payload.at(91) != 'J') {
        return;
    }

    const auto p = payload.data();

    // Borderline undefined behavior with dereferencing ensues
    // Market data packet header
    MsgSeqNum = *reinterpret_cast<const uint32_t*>(p);
    MsgSize = *reinterpret_cast<const uint16_t*>(p + 4);
    MsgFlags = *reinterpret_cast<const uint16_t*>(p + 6);
    SendingTime = *reinterpret_cast<const uint64_t*>(p + 8);

    // SBE header
    BlockLength = *reinterpret_cast<const uint16_t*>(p + 16);
    TemplateId = *reinterpret_cast<const uint16_t*>(p + 18);
    SchemaId = *reinterpret_cast<const uint16_t*>(p + 20);
    Version = *reinterpret_cast<const uint16_t*>(p + 22);

    valid = true;
}

bool OrderBookSnapshot::is_valid() {
    return valid;
}

std::string OrderBookSnapshot::dump() {
    if (!is_valid()) {
        return "invalid"; // throw exception instead?
    }

    // wastefully preallocate buffer 
    char buf[1000] = {0};

    snprintf(buf, sizeof(buf) -1, "OrderBookSnapshot:\n"
                       "  Market data packet header\n"
                       "    MsgSeqNum %#x\n"
                       "    MsgSize %#x\n"
                       "    MsgFlags %#x\n"
                       "    SendingTime %ld\n"
                       "  SBE Header\n"
                       "    BlockLength %#x\n"
                       "    TemplateId %#x\n"
                       "    SchemaId %#x\n"
                       "    Version %#x\n"
                       , MsgSeqNum, (unsigned)MsgSize, (unsigned)MsgFlags,
                         SendingTime,
                         (unsigned)BlockLength,
                         (unsigned)TemplateId, (unsigned)SchemaId, 
                         (unsigned)Version
    );
    return std::string(buf);
}
