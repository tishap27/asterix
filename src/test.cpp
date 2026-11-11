#include "asterix.hpp"
#include <cassert>
#include <cstdio>
int main() {
    Block b;
    assert(decode_block(parse_hex("30 00 06 80 01 02"), b));
    assert(b.cat == 48 && b.frns.size() == 1 && b.frns[0] == 1);
    assert(b.has010 && b.sac == 1 && b.sic == 2 && b.complete);

    assert(decode_block(parse_hex("30 00 08 81 20 01 02 AA"), b));
    assert(b.frns.size() == 2 && b.frns[1] == 10);
    assert(b.sac == 1 && b.sic == 2 && !b.complete);

    assert(!decode_block(parse_hex("30 00 09 80"), b));
    assert(!decode_block(parse_hex("30 00 05 80 01"), b));  // heretruncated I010

   //  I010 + I140: time 0x004000 = 16384 / 128 = 128 s
    assert(decode_block(parse_hex("30 00 09 C0 01 02 00 40 00"), b));
    assert(b.has140 && b.tod == 128.0 && b.complete);

    
    // I010 + I140 + I020 (0x40 -> type 2, single SSR)
    assert(decode_block(parse_hex("30 00 0A E0 01 02 00 40 00 40"), b));
    assert(b.has020 && b.typ == 2 && b.complete);


    // I020 with one extension byte
    assert(decode_block(parse_hex("30 00 0B E0 01 02 00 40 00 41 00"), b));
    assert(b.typ == 2 && b.complete);

    // I010 + I140 + I020 + I040: rho 0x0100 = 1.0 NM, theta 0x4000 = 90 deg
    assert(decode_block(parse_hex("30 00 0E F0 01 02 00 40 00 40 01 00 40 00"), b));
    assert(b.has040 && b.rho_nm == 1.0 && b.theta_deg == 90.0 && b.complete);

    // all five fields, squawk 1200 (octal) = 0x280
    assert(decode_block(parse_hex("30 00 10 F8 01 02 00 40 00 40 01 00 40 00 02 80"), b));
    assert(b.has070 && b.mode3a == 01200 && b.complete);


        // two blocks back to back
    size_t bad = 0;
    auto v = decode_stream(parse_hex("30 00 06 80 01 02 30 00 09 C0 01 02 00 40 00"), &bad);
    assert(v.size() == 2 && bad == 0);
    assert(v[0].sac == 1 && v[1].tod == 128.0);
    // truncated second block is counted as bad
    v = decode_stream(parse_hex("30 00 06 80 01 02 30 00 09 C0"), &bad);
    assert(v.size() == 1 && bad == 1);

    
    // all six fields, FL350 = 350 * 4 = 1400 = 0x0578
    assert(decode_block(parse_hex("30 00 12 FC 01 02 00 40 00 40 01 00 40 00 02 80 05 78"), b));
    assert(b.has090 && b.fl == 350.0 && b.complete);
    // negative flight level: -10 FL = 0x3FD8 in 14 bit two's complement
    assert(decode_block(parse_hex("30 00 12 FC 01 02 00 40 00 40 01 00 40 00 02 80 3F D8"), b));
    assert(b.fl == -10.0);
    
    // FRN 1-7 with I130 (primary 0x80 = one subfield, 1 byte) skipped
    assert(decode_block(parse_hex("30 00 14 FE 01 02 00 40 00 40 01 00 40 00 02 80 05 78 80 10"), b));
    assert(b.fl == 350.0 && b.complete);

    // FRN 1-9: address C0FFEE, callsign "ACA123"
    assert(decode_block(parse_hex("30 00 1E FF C0 01 02 00 40 00 40 01 00 40 00 02 80 05 78 80 10 C0 FF EE 04 30 71 CB 38 20"), b));
    assert(b.has220 && b.icao == 0xC0FFEE);
    assert(b.has240 && b.callsign == "ACA123" && b.complete);

    std::puts("all tests passed");
}