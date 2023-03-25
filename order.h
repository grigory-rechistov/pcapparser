// Sensing and parsing of OrderBookSnapshot etc.
#include <vector>
#include <string>

class OrderBookSnapshot {
    bool valid;

    public:
    OrderBookSnapshot() = delete;
    OrderBookSnapshot(const std::vector<uint8_t> &payload);
    bool is_valid();
};
