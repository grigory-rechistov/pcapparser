#pragma once
#include <cstdio>
#include <cstdint>
#include<functional> 

using ReadDword = std::function <uint32_t (FILE* stream)>; //auto (*)(FILE *stream) -> uint32_t;



void parse_header( ReadDword dword_reader, FILE *f);
