#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Block {
    uint8_t cat = 0;
    uint16_t len = 0;
    std::vector<int> frns;
    bool complete = true;   // decode == true, if cant decode yet== false
    bool has010 = false;
    uint8_t sac = 0, sic = 0;
    bool has140 = false;
    double tod = 0;          // seconds since midnight UTC
};

std::vector<uint8_t> parse_hex(const std::string& s);
bool decode_block(const std::vector<uint8_t>& b, Block& out);