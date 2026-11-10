#include "asterix.hpp"
#include <sstream>

std::vector<uint8_t> parse_hex(const std::string& s) {
    std::vector<uint8_t> out; std::istringstream in(s); std::string t;
    while (in >> t) out.push_back(static_cast<uint8_t>(std::stoul(t, nullptr, 16)));
    return out;
}

bool decode_block(const std::vector<uint8_t>& b, Block& out) {
    out = Block{};
    if (b.size() < 4) return false;
    out.cat = b[0];
    out.len = (b[1] << 8) | b[2];
    if (out.len != b.size()) return false;

    // FSPEC: which fields are present
    size_t i = 3; int base = 0; bool fx = true;
    while (fx) {
        if (i >= b.size()) return false;
        uint8_t o = b[i++];
        for (int bit = 7; bit >= 1; --bit)
            if (o & (1 << bit)) out.frns.push_back(base + (8 - bit));
        fx = o & 1; base += 7;
    }

    // Fields, in FRN order
    for (int f : out.frns) {
        switch (f) {
        case 1:  // I048/010 Data Source Identifier
            if (i + 2 > b.size()) return false;
            out.has010 = true;
            out.sac = b[i];
            out.sic = b[i + 1];
            i += 2;
            break;
        
        case 2: {  // I048/140 Time of Day, units of 1/128 s
            if (i + 3 > b.size()) return false;
            uint32_t raw = (b[i] << 16) | (b[i + 1] << 8) | b[i + 2];
            out.has140 = true;
            out.tod = raw / 128.0;
            i += 3;
            break;
        }
        case 3: {  // I048/020 Target Report Descriptor, variable length
            if (i >= b.size()) return false;
            out.has020 = true;
            out.typ = b[i] >> 5;
            bool more = true;
            while (more) {
                if (i >= b.size()) return false;
                more = b[i++] & 1;   // lowest bit set = another byte follows
            }
            break;
        }
        case 4: {  // I048/040 Measured Position, polar
            if (i + 4 > b.size()) return false;
            uint16_t rho = (b[i] << 8) | b[i + 1];
            uint16_t theta = (b[i + 2] << 8) | b[i + 3];
            out.has040 = true;
            out.rho_nm = rho / 256.0;              // 1/256 NM per bit
            out.theta_deg = theta * 360.0 / 65536; // 360/2^16 deg per bit
            i += 4;
            break;
        }
        case 5: {  // I048/070 Mode-3/A code
            if (i + 2 > b.size()) return false;
            out.has070 = true;
            out.mode3a = ((b[i] << 8) | b[i + 1]) & 0x0FFF;  // top 4 bits are flags
            i += 2;
            break;
        }

        case 6: {  // I048/090 Flight Level, 1/4 FL per bit, two's complement
            if (i + 2 > b.size()) return false;
            int raw = ((b[i] << 8) | b[i + 1]) & 0x3FFF;  // top 2 bits are validity flags
            if (raw & 0x2000) raw -= 0x4000;               // sign extend 14 bits
            out.has090 = true;
            out.fl = raw / 4.0;
            i += 2;
            break;
        }
        default:  //can't know its length stops here
            out.complete = false;
            return true;
        }
    }
    return true;
}

std::vector<Block> decode_stream(const std::vector<uint8_t>& d, size_t* bad) {
    std::vector<Block> out;
    size_t nbad = 0, o = 0;
    while (o < d.size()) {
        if (d.size() - o < 3) { ++nbad; break; }
        size_t len = (d[o + 1] << 8) | d[o + 2];
        if (len < 4 || o + len > d.size()) { ++nbad; break; }  // broken length, can't resync
        std::vector<uint8_t> chunk(d.begin() + o, d.begin() + o + len);
        Block b;
        if (decode_block(chunk, b)) out.push_back(b); else ++nbad;
        o += len;
    }
    if (bad) *bad = nbad;
    return out;
}