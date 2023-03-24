#pragma once
#include <cstdio>
#include <cstdint>
#include<functional>
#include "header.h" 

using ReadDword = std::function <uint32_t (FILE* stream)>;



ParsedHeader parse_header( ReadDword dword_reader, FILE *f);
