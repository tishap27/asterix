#include "asterix.hpp"
#include <cstdio>
int main(int argc, char** argv) {
    if (argc < 2) { std::puts("usage: asterix \"30 00 06 80 01 02\""); return 1; }
    Block b;
    if (!decode_block(parse_hex(argv[1]), b)) { std::puts("bad block"); return 1; }
    std::printf("CAT%03u len=%u FRNs:", b.cat, b.len);
    for (int f : b.frns) std::printf(" %d", f);
    std::puts("");
}