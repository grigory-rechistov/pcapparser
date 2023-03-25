// Streams for input
#pragma once
#include <cstdio>
#include <vector>
#include <cstdint>

void read_input_buffer(FILE* stream, size_t count, std::vector<uint8_t> &out);
uint32_t read_input_u32(FILE* stream);
std::tuple<FILE *const, FILE *const> make_binary_streams();

