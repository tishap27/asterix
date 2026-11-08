#include "asterix.hpp"
#include <cassert>
#include <cstdio>
int main() {
    Block b;
    assert(decode_block(parse_hex("30 00 06 80 01 02"), b));
    assert(b.cat == 48 && b.frns.size() == 1 && b.frns[0] == 1);
    assert(b.has010 && b.sac == 1 && b.sic == 2 && b.complete);

    assert(decode_block(parse_hex("30 00 08 81 40 01 02 AA"), b));
    assert(b.frns.size() == 2 && b.frns[1] == 9);
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
    std::puts("all tests passed");
}