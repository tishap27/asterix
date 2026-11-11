#include "asterix.hpp"
#include <cstdio>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>

static void print_block(const Block& b) {
    std::printf("CAT%03u len=%u FRNs:", b.cat, b.len);
    for (int f : b.frns) std::printf(" %d", f);
    std::puts("");
    if (b.has010) std::printf("I010: SAC=%u SIC=%u\n", b.sac, b.sic);
    if (b.has140) {
        int s = static_cast<int>(b.tod);
        std::printf("I140: %02d:%02d:%02d UTC\n", s / 3600, (s % 3600) / 60, s % 60);
    }
    if (b.has020) std::printf("I020: type=%u\n", b.typ);
    if (b.has040) std::printf("I040: range=%.2f NM azimuth=%.2f deg\n", b.rho_nm, b.theta_deg);
    if (b.has070) std::printf("I070: squawk=%04o\n", b.mode3a);
    if (b.has090) std::printf("I090: FL%03.0f\n", b.fl);
    if (b.has220) std::printf("I220: address=%06X\n", b.icao);
    if (b.has240) std::printf("I240: callsign=%s\n", b.callsign.c_str());
    if (!b.complete) std::puts("(stopped at a field not implemented yet)");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::puts("usage: asterix \"30 00 06 80 01 02\"");
        std::puts("       asterix -f samples\\capture.txt");
        return 1;
    }
    std::string text;
    if (std::string(argv[1]) == "-f") {
        if (argc < 3) { std::puts("missing file name"); return 1; }
        std::ifstream in(argv[2]);
        if (!in) { std::puts("cannot open file"); return 1; }
        std::stringstream ss;
        ss << in.rdbuf();
        text = ss.str();
    } else {
        text = argv[1];
    }

    try {
        size_t bad = 0;
        auto blocks = decode_stream(parse_hex(text), &bad);
        for (size_t n = 0; n < blocks.size(); ++n) {
            std::printf("--- block %zu ---\n", n + 1);
            print_block(blocks[n]);
        }
        std::printf("%zu block(s) decoded, %zu bad\n", blocks.size(), bad);
        return bad ? 1 : 0;
    } catch (const std::exception&) {
        std::puts("invalid hex in input");
        return 1;
    }
}