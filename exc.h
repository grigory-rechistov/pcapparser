// Parsing exceptions
#pragma once
#include <exception>

class TruncatedInput: public std::exception {};
class BadMagic: public std::exception {};