# Interface control document: asterix-decode

## Purpose
Defines what the decoder accepts, what it prints, and how it reports errors.

## Input
- **Command line:** `asterix "<hex bytes>"`
- **File:** `asterix -f <path>`
- Format: hexadecimal bytes separated by whitespace. One or more ASTERIX data blocks back to back. Each block starts with the category byte and a 2 byte big-endian length that covers the whole block.
- Only Category 048 fields listed below are decoded.

## Output (stdout)
For each block:
```
--- block N ---
CATccc len=L FRNs: <field numbers present>
<one line per decoded field>
(stopped at a field not implemented yet)   <- only if decoding stopped early
```
Then a summary line: `N block(s) decoded, M bad`.

## Decoded fields
| Item | Size | Meaning and units |
|------|------|-------------------|
| I048/010 | 2 bytes | SAC and SIC |
| I048/140 | 3 bytes | Time of day, 1/128 s since midnight UTC |
| I048/020 | 1+ bytes | Detection type (top 3 bits of the first byte); extension bytes skipped |
| I048/040 | 4 bytes | Range 1/256 NM; azimuth 360/65536 degrees |
| I048/070 | 2 bytes | Mode 3/A code, 12 bits, printed in octal |
| I048/090 | 2 bytes | Flight level, 14 bit signed, 1/4 FL per bit |
| I048/130 | variable | Skipped, not decoded |
| I048/220 | 3 bytes | 24 bit aircraft address |
| I048/240 | 6 bytes | Callsign, 8 characters of 6 bits |

## Exit codes
| Code | Meaning |
|------|---------|
| 0 | All blocks decoded |
| 1 | Bad usage, unreadable file, invalid hex, or at least one bad block |

## Error behaviour
| Condition | Result |
|-----------|--------|
| Length field does not match the data | Counted as bad; decoding of the stream stops |
| Block cut off inside a field | Counted as bad |
| Field not implemented yet | Block marked incomplete, earlier fields still printed |
| Invalid hex text | `invalid hex in input`, exit code 1 |
| File cannot be opened | `cannot open file`, exit code 1 |

## Known limitations
- Hex values larger than one byte (for example `1FF`) are not rejected and are silently truncated
- Field layouts follow the Eurocontrol CAT048 specification; verify against the edition you work with
- Sample data in this repo is hand-made, not from a real radar