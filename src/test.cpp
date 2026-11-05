#include "asterix.hpp"
#include <cassert>
#include <cstdio>
int main() {
    Block b;
    assert(decode_block(parse_hex("30 00 06 80 01 02"), b));
    assert(b.cat == 48 && b.frns.size() == 1 && b.frns[0] == 1);
    assert(decode_block(parse_hex("30 00 06 81 40 02"), b));
    assert(b.frns.size() == 2 && b.frns[1] == 9);
    assert(!decode_block(parse_hex("30 00 09 80"), b));
    std::puts("all tests passed");
}