#include "asterix.hpp"
#include <sstream>

std::vector<uint8_t> parse_hex(const std::string& s) {
    std::vector<uint8_t> out; std::istringstream in(s); std::string t;
    while (in >> t) out.push_back(static_cast<uint8_t>(std::stoul(t, nullptr, 16)));
    return out;
}

bool decode_block(const std::vector<uint8_t>& b, Block& out) {
    if (b.size() < 4) return false;
    out.cat = b[0];
    out.len = (b[1] << 8) | b[2];
    if (out.len != b.size()) return false;
    out.frns.clear();
    size_t i = 3; int base = 0; bool fx = true;
    while (fx) {
        if (i >= b.size()) return false;
        uint8_t o = b[i++];
        for (int bit = 7; bit >= 1; --bit)
            if (o & (1 << bit)) out.frns.push_back(base + (8 - bit));
        fx = o & 1; base += 7;
    }
    return true;
}