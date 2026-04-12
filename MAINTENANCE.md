# MAINTENANCE.md

## Overview

`flux-language-c` is intentionally minimal: single header, single source, no external dependencies.

## Architecture

- **language.h** — Public API and type definitions
- **language.c** — Implementation
- **test_language.c** — Self-contained test suite (16 tests)

All buffers are fixed-size and stack-allocated. `LanguageEngine` is ~120KB on the stack — be mindful of stack limits on embedded targets. Reduce `TOKENS_MAX`/`WORDS_MAX` if needed.

## Adding Features

1. Add declaration to `language.h`
2. Implement in `language.c`
3. Add test case to `test_language.c`
4. Run `make test`

## Common Tasks

### Increase token capacity
Edit `TOKENS_MAX` in `language.h`. Also increases `LanguageEngine` size.

### Add a new token type
Add to `TokenType` enum in `language.h`, update `classify()` in `language.c`.

### Port to C89
Remove `<stdint.h>`, replace `uint16_t` with `unsigned short`, remove `_Generic` if added.

## Testing

```sh
make test        # build + run
make clean       # remove binary
```

Tests use simple printf assertions — no framework needed.

## Known Limitations

- No Unicode support (ASCII only)
- No regex — `lang_match` is plain substring
- `lang_similarity` is character overlap, not Levenshtein
- Decimals with multiple dots aren't parsed as numbers
- Tokens beyond `TOKENS_MAX` are silently dropped
