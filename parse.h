#pragma once
#include <cstdio>
#include <cstdint>
#include<functional>
#include "header.h" 

using ReadDword = std::function <uint32_t (FILE* stream)>;

static constexpr size_t header_size_in_dwords = 6;

ParsedHeader parse_header( ReadDword dword_reader, FILE *f);
