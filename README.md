# flux-language-c

A pure C11 library for text tokenization, pattern matching, and basic NLP primitives.

**Zero dependencies. No malloc. Stack-allocated only.**

## Features

- **Tokenization** — splits text into words, numbers, punctuation, and whitespace
- **Pattern matching** — substring and exact token matching
- **Word frequency** — count occurrences and find most common words
- **String similarity** — character overlap ratio (0–100)
- **Number extraction** — detect and extract numeric values from token streams

## Build & Test

```sh
make test
```

## Usage

```c
#include "language.h"

LanguageEngine e;
lang_init(&e);

lang_tokenize(&e, "the quick brown fox jumped over the lazy dog");
printf("Words: %d\n", lang_count_words(&e));       // 9
printf("Has 'fox': %d\n", lang_contains(&e, "fox")); // 1
printf("'the' appears %d times\n", lang_word_frequency(&e, "the")); // 2
printf("Similarity: %d%%\n", lang_similarity("hello", "hallo"));   // 80
```

## API

| Function | Description |
|---|---|
| `lang_init` | Initialize engine |
| `lang_tokenize` | Tokenize (with spaces) |
| `lang_tokenize_strict` | Tokenize (skip spaces) |
| `lang_count_words` | Count word tokens |
| `lang_contains` | Check if word exists |
| `lang_add_pattern` | Register a pattern |
| `lang_match` | Substring match across tokens |
| `lang_match_exact` | Exact token match |
| `lang_word_frequency` | Count word occurrences |
| `lang_most_common` | Get most frequent word |
| `lang_similarity` | 0–100 character overlap |
| `lang_is_number` | Check if string is numeric |
| `lang_extract_number` | Extract nth numeric token |

## Limits

All fixed-size, stack-allocated:

| Constant | Value |
|---|---|
| TOKENS_MAX | 128 |
| TOKEN_MAX_LEN | 64 |
| PATTERNS_MAX | 32 |
| WORDS_MAX | 256 |

## License

Public domain / CC0.
