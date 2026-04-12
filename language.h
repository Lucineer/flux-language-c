#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <stdint.h>
#include <stddef.h>

#define TOKENS_MAX 128
#define TOKEN_MAX_LEN 64
#define PATTERNS_MAX 32
#define WORDS_MAX 256

typedef enum { TOK_WORD=0, TOK_NUMBER, TOK_PUNCT, TOK_SPACE, TOK_UNKNOWN } TokenType;

typedef struct { TokenType type; char text[TOKEN_MAX_LEN]; } Token;
typedef struct { Token tokens[TOKENS_MAX]; uint16_t count; } TokenList;

typedef struct { char pattern[TOKEN_MAX_LEN]; int match_count; } Pattern;

typedef struct {
    TokenList tokens;
    Pattern patterns[PATTERNS_MAX];
    uint16_t pattern_count;
    char words[WORDS_MAX][TOKEN_MAX_LEN];
    uint16_t word_count;
} LanguageEngine;

void lang_init(LanguageEngine *e);
int lang_tokenize(LanguageEngine *e, const char *text);
int lang_tokenize_strict(LanguageEngine *e, const char *text);
int lang_count_words(const LanguageEngine *e);
int lang_contains(const LanguageEngine *e, const char *word);
int lang_add_pattern(LanguageEngine *e, const char *pattern);
int lang_match(const LanguageEngine *e, const char *pattern);
int lang_match_exact(const LanguageEngine *e, const char *word);
int lang_word_frequency(const LanguageEngine *e, const char *word);
int lang_most_common(LanguageEngine *e, char *out, int outsz);
int lang_similarity(const char *a, const char *b);
int lang_is_number(const char *text);
float lang_extract_number(const LanguageEngine *e, int index);

#endif
