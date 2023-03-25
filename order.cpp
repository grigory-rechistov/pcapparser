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
    
    valid = true;
}

bool OrderBookSnapshot::is_valid() {
    return valid;
}
