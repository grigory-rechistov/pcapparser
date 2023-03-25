// Streams for input
#pragma once
#include <cstdio>
#include <vector>
#include <cstdint>
#include <functional>

using ReadDword = std::function <uint32_t (FILE* stream)>;
using FillBuffer = std::function <void (FILE* stream, size_t count, std::vector<uint8_t> &out)>;

void read_input_buffer(FILE* stream, size_t count, std::vector<uint8_t> &out);
uint32_t read_input_u32(FILE* stream);
std::tuple<FILE *const, FILE *const> make_binary_streams();

