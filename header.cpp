// 
#include "header.h"
// #include <format> // Assume C++20 is not available

using s = std::string;
using std::to_string;

std::string ParsedHeader::dump() {
    // Inefficient, of course.
    return s("Header: ") 
        + s(this->is_time_in_ns ? "time in sec:ns" : "sec:ms")
        + s(" version: ") + to_string(major_version) + s(".") + to_string(minor_version)
        + s(" snap_len: ") + to_string(this->snap_len);
}