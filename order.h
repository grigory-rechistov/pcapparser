// Sensing and parsing of OrderBookSnapshot etc.
#include <vector>
#include <string>
#include <cstdint>

class OrderBookSnapshot {
    bool valid;
    uint32_t MsgSeqNum;
    uint16_t MsgSize;
    uint16_t MsgFlags;
    uint64_t SendingTime;

    uint16_t BlockLength;
    uint16_t TemplateId;
    uint16_t SchemaId;
    uint16_t Version;

#pragma pack(push,1)
    struct {
        uint32_t SecurityID;
        uint32_t LastMsgSeqNumProcessed;
        uint32_t RptSeq;
        uint32_t ExchangeTradingSessionID;
        struct groupSize {
            uint16_t block_length;
            uint8_t number_in_group;
        } NoMDEntries;
        uint64_t MDEntryID;
        uint64_t TransactTime;
        uint64_t MDEntryPx;
        uint64_t MDEntrySize;
        uint64_t TradeId;
        uint64_t MDFlags;
        uint8_t MDEntryType;
    } msg17;
#pragma pack(pop)

    public:
    OrderBookSnapshot() = delete;
    OrderBookSnapshot(const std::vector<uint8_t> &payload);
    bool is_valid();
    std::string dump();
};
