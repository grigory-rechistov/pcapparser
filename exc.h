// Parsing exceptions
#pragma once
#include <exception>

class TruncatedInput: public std::exception {};
class BadMagic: public std::exception {};
class ValueOutOfRange: public std::exception {};
class NotSupportedInput: public std::exception {};