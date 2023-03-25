// Sensing and parsing of OrderBookSnapshot etc.
#include "order.h"
#include <cstring>
#include <cassert>

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

    memcpy(&this->msg17, p + 24, sizeof(this->msg17));

    assert(this->msg17.MDEntryType == 'J');

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

    snprintf(buf, sizeof(buf) -1, "Full headers\n"
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
                       "  OrderBookSnapshot\n"
                       "    SecurityID %#x\n"
                       "    LastMsgSeqNumProcessed %#x\n"
                       "    RptSeq %#x\n"
                       "    ExchangeTradingSessionID %#x\n"
                       "    NoMDEntries.block_length %#x\n"
                       "    NoMDEntries.number_in_group %#x\n"
                       "    MDEntryID %#lx\n"
                       "    TransactTime %#l\n"
                       "    MDEntryPx %#lx\n"
                       "    MDEntrySize %#lx\n"
                       "    TradeId %#lx\n"
                       "    MDFlags %#lx\n"
                       "    MDEntryType %#x\n"
                       , MsgSeqNum, (unsigned)MsgSize, (unsigned)MsgFlags,
                         SendingTime,
                         (unsigned)BlockLength,
                         (unsigned)TemplateId, (unsigned)SchemaId, 
                         (unsigned)Version,

                        msg17.SecurityID,
                        msg17.LastMsgSeqNumProcessed,
                        msg17.RptSeq,
                        msg17.ExchangeTradingSessionID,
                        (unsigned)msg17.NoMDEntries.block_length,
                        (unsigned)msg17.NoMDEntries.number_in_group,
                        
                        msg17.MDEntryID,
                        msg17.TransactTime,
                        msg17.MDEntryPx,
                        msg17.MDEntrySize,
                        msg17.TradeId,
                        msg17.MDFlags,

                        (unsigned)msg17.MDEntryType
    );
    return std::string(buf);
}
