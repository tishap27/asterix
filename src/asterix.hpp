#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Block { uint8_t cat; uint16_t len; std::vector<int> frns; };

std::vector<uint8_t> parse_hex(const std::string& s);
bool decode_block(const std::vector<uint8_t>& b, Block& out);