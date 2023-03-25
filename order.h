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

    public:
    OrderBookSnapshot() = delete;
    OrderBookSnapshot(const std::vector<uint8_t> &payload);
    bool is_valid();
    std::string dump();
};
