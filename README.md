# asterix-decode
C++17 ASTERIX decoder, CAT048 first.

![CI](https://github.com/tishap27/asterix/actions/workflows/ci.yml/badge.svg)


## Project layout
- `src/asterix.cpp`, `src/asterix.hpp`: decoder library
- `src/main.cpp`: command line tool
- `tests/test.cpp`: tests
- `samples/capture.txt`: example capture



Example output:
```
--- block 1 ---
CAT048 len=30 FRNs: 1 2 3 4 5 6 7 8 9
I010: SAC=1 SIC=2
I140: 00:02:08 UTC
I020: type=2
I040: range=1.00 NM azimuth=90.00 deg
I070: squawk=1200
I090: FL350
I220: address=C0FFEE
I240: callsign=ACA123
1 block(s) decoded, 0 bad
```
## Tests
```
g++ -std=c++17 -Wall -Wextra -Isrc -o test_run tests/test.cpp src/asterix.cpp
test_run
```
Tests also run on every push through GitHub Actions.

## Limitations
- Category 048 only, and only the fields listed above
- Input is hex text, not raw network or file captures
- The sample data is hand-made, not from a real radar

## Roadmap
- I048/090 flight level
- More I048 fields (track number, velocity)
- Reading raw binary captures

## Supported fields
| Item | Description |
|------|-------------|
| I048/010 | Data source identifier (SAC/SIC) |
| I048/140 | Time of day |
| I048/020 | Target report descriptor (detection type) |
| I048/040 | Measured position (range and azimuth) |
| I048/070 | Mode 3/A code (squawk) |
| I048/090 | Flight level |
| I048/130 | Plot characteristics (skipped, not decoded) |
| I048/220 | Aircraft address |
| I048/240 | Aircraft identification (callsign) |
