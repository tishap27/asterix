# asterix-decode
C++17 ASTERIX decoder, CAT048 first.



## Project layout
- `src/asterix.cpp`, `src/asterix.hpp`: decoder library
- `src/main.cpp`: command line tool
- `tests/test.cpp`: tests
- `samples/capture.txt`: example capture

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
