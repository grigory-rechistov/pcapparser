#pragma once
#include <cstdio>
#include <cstdint>

using ReadDword = auto (*)(FILE *stream) -> uint32_t;



void parse_header( ReadDword dword_reader, FILE *f);
