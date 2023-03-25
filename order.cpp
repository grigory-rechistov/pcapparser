// Sensing and parsing of OrderBookSnapshot etc.
#include "order.h"
#include <cstring>
#include <cstddef>
#include <cassert>

static constexpr uint8_t OrderBookSnapshotMarker = 'J';
static constexpr size_t OrderBookSnapshotRecognizedLength = 92;
static constexpr ptrdiff_t OrderBookSnapshotOffset = 24;

OrderBookSnapshot::OrderBookSnapshot(const std::vector<uint8_t> &payload): 
    valid(false) {
    if (payload.size() != OrderBookSnapshotRecognizedLength) {
        return;
    }
    if (payload.at(OrderBookSnapshotRecognizedLength-1) != OrderBookSnapshotMarker) {
        return;
    }
    const auto p = payload.data();
    memcpy(&this->msg17, p + OrderBookSnapshotOffset, sizeof(this->msg17));
    assert(this->msg17.MDEntryType ==  OrderBookSnapshotMarker);

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

    snprintf(buf, sizeof(buf) -1,
                       "OrderBookSnapshot\n"
                       "  SecurityID %#x\n"
                       "  LastMsgSeqNumProcessed %#x\n"
                       "  RptSeq %#x\n"
                       "  ExchangeTradingSessionID %#x\n"
                       "  NoMDEntries.block_length %#x\n"
                       "  NoMDEntries.number_in_group %#x\n"
                       "  MDEntryID %#lx\n"
                       "  TransactTime %ld\n"
                       "  MDEntryPx %#lx\n"
                       "  MDEntrySize %#lx\n"
                       "  TradeId %#lx\n"
                       "  MDFlags %#lx\n"
                       "  MDEntryType %#x\n",
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
