# asterix-decode design

## Data flow
```mermaid
flowchart LR
    In[Hex text: CLI argument or capture file] --> Main[main.cpp]
    Main --> Hex[parse_hex]
    Hex --> Stream[decode_stream]
    Stream --> Block[decode_block]
    Block --> Out[Block struct]
    Out --> Print[print_block]
    Print --> Stdout[Console output]
```

## Decoding sequence
```mermaid
sequenceDiagram
    participant U as User
    participant M as main
    participant S as decode_stream
    participant B as decode_block
    U->>M: asterix -f capture.txt
    M->>M: read file, parse_hex to bytes
    M->>S: bytes
    loop each data block
        S->>S: read length, slice block
        S->>B: block bytes
        B->>B: read FSPEC, then decode each field in order
        B-->>S: Block (or failure)
    end
    S-->>M: blocks and bad count
    M-->>U: decoded fields, summary, exit code
```

## Key types
```mermaid
classDiagram
    class Block {
        +uint8_t cat
        +uint16_t len
        +vector~int~ frns
        +bool complete
        +decoded fields (I010, I140, I020, I040, I070, I090, I220, I240)
    }
    class Decoder {
        +parse_hex(text) vector~uint8_t~
        +decode_block(bytes, Block) bool
        +decode_stream(bytes, bad) vector~Block~
    }
    Decoder --> Block : produces
```

## Design decisions
- **Library plus thin CLI.** Decoding lives in `asterix.cpp`; `main.cpp` only handles input and printing, so the decoder can be reused and tested without the CLI.
- **Fields decoded in FRN order.** A field's position depends on the lengths of the fields before it, so decoding stops at the first field not implemented yet and marks the block `complete = false` instead of guessing.
- **Bad data is counted, not fatal.** A truncated or inconsistent block increments a bad counter and the program reports it through its exit code.
- **Stops at a broken length field.** Without a valid length the next block's start is unknown, so the decoder doesn't try to resynchronise.
- **Tests build and run in CI** on every push.