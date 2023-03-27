// Sensing and parsing of OrderBookSnapshot etc.
#include "order.h"
#include <cstring>
#include <cstddef>
#include <cassert>
#include "tostr.h"

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

    // TODO Pretty print remaining types, such as Decimal5NULL, MDEntryType
    // TODO for signed primitives, add formatters that represent them with "+/-" 
    snprintf(buf, sizeof(buf)-1,
                       "OrderBookSnapshot\n"
                       "  SecurityID %#x\n"
                       "  LastMsgSeqNumProcessed %#x\n"
                       "  RptSeq %#x\n"
                       "  ExchangeTradingSessionID %#x\n"
                       "  NoMDEntries.block_length %#x\n"
                       "  NoMDEntries.number_in_group %#x\n"
                       "  MDEntryID %s\n"
                       "  TransactTime %ld\n"
                       "  MDEntryPx %#lx\n"
                       "  MDEntrySize %s\n"
                       "  TradeId %s\n"
                       "  MDFlags %#lx\n"
                       "  MDEntryType %s\n",
                        msg17.SecurityID,
                        msg17.LastMsgSeqNumProcessed,
                        msg17.RptSeq,
                        msg17.ExchangeTradingSessionID,
                        (unsigned)msg17.NoMDEntries.block_length,
                        (unsigned)msg17.NoMDEntries.number_in_group,
                        
                        Int64NulltoStr(msg17.MDEntryID).c_str(),
                        msg17.TransactTime,
                        msg17.MDEntryPx,
                        Int64NulltoStr(msg17.MDEntrySize).c_str(),
                        Int64NulltoStr(msg17.TradeId).c_str(),
                        msg17.MDFlags,

                        MDEntryTypetoStr(msg17.MDEntryType).c_str()
    );
    return std::string(buf);
}
