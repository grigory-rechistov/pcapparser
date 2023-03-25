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

    // Borderline undefined behavior ensues
    MsgSeqNum = *reinterpret_cast<const uint32_t*>(p);
    MsgSize = *reinterpret_cast<const uint16_t*>(p + 4);
    MsgFlags = *reinterpret_cast<const uint16_t*>(p + 6);
    SendingTime = *reinterpret_cast<const uint64_t*>(p + 8);

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
                       "  Marker data packet header\n"
                       "    MsgSeqNum %#x\n"
                       "    MsgSize %#x\n"
                       "    MsgFlags %#x\n"
                       "    SendingTime %ld\n"
                       "  SBE Header\n"
                       , MsgSeqNum, (unsigned)MsgSize, (unsigned)MsgFlags,
                         SendingTime);
    return std::string(buf);
}
