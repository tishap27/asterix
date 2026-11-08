#include "asterix.hpp"
#include <cstdio>
int main(int argc, char** argv) {
    if (argc < 2) { std::puts("usage: asterix \"30 00 06 80 01 02\""); return 1; }
    Block b;
    if (!decode_block(parse_hex(argv[1]), b)) { std::puts("bad block"); return 1; }
    std::printf("CAT%03u len=%u FRNs:", b.cat, b.len);
    for (int f : b.frns) std::printf(" %d", f);
    std::puts("");
    if (b.has010) std::printf("I010: SAC=%u SIC=%u\n", b.sac, b.sic);
    if (b.has140) {
        int s = static_cast<int>(b.tod);
        std::printf("I140: %02d:%02d:%02d UTC\n", s / 3600, (s % 3600) / 60, s % 60);
    }
    if (b.has020) std::printf("I020: type=%u\n", b.typ);
    if (!b.complete) std::puts("(stopped at a field not implemented yet)");
}