#include "language.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void lang_init(LanguageEngine *e) {
    memset(e, 0, sizeof(*e));
}

static TokenType classify(char c) {
    if (isdigit((unsigned char)c)) return TOK_NUMBER;
    if (isalpha((unsigned char)c) || c == '_') return TOK_WORD;
    if (isspace((unsigned char)c)) return TOK_SPACE;
    return TOK_PUNCT;
}

static int do_tokenize(LanguageEngine *e, const char *text, int skip_spaces) {
    e->tokens.count = 0;
    e->word_count = 0;
    if (!text) return 0;

    int i = 0, len = (int)strlen(text);
    while (i < len && e->tokens.count < TOKENS_MAX) {
        TokenType ct = classify(text[i]);

        if (skip_spaces && ct == TOK_SPACE) {
            i++;
            continue;
        }

        int j = 0;
        while (i + j < len && j < TOKEN_MAX_LEN - 1 &&
               classify(text[i + j]) == ct) {
            j++;
        }

        Token *t = &e->tokens.tokens[e->tokens.count];
        t->type = ct;
        memcpy(t->text, text + i, j);
        t->text[j] = '\0';
        e->tokens.count++;
        i += j;

        if (ct == TOK_WORD || ct == TOK_NUMBER) {
            if (e->word_count < WORDS_MAX)
                memcpy(e->words[e->word_count++], t->text, j + 1);
        }
    }
    return e->tokens.count;
}

int lang_tokenize(LanguageEngine *e, const char *text) {
    return do_tokenize(e, text, 0);
}

int lang_tokenize_strict(LanguageEngine *e, const char *text) {
    return do_tokenize(e, text, 1);
}

int lang_count_words(const LanguageEngine *e) {
    int c = 0;
    for (int i = 0; i < e->tokens.count; i++)
        if (e->tokens.tokens[i].type == TOK_WORD) c++;
    return c;
}

int lang_contains(const LanguageEngine *e, const char *word) {
    if (!word) return 0;
    for (int i = 0; i < e->word_count; i++)
        if (strcmp(e->words[i], word) == 0) return 1;
    return 0;
}

int lang_add_pattern(LanguageEngine *e, const char *pattern) {
    if (!pattern || e->pattern_count >= PATTERNS_MAX) return -1;
    strncpy(e->patterns[e->pattern_count].pattern, pattern, TOKEN_MAX_LEN - 1);
    e->patterns[e->pattern_count].pattern[TOKEN_MAX_LEN - 1] = '\0';
    e->patterns[e->pattern_count].match_count = 0;
    e->pattern_count++;
    return (int)(e->pattern_count - 1);
}

int lang_match(const LanguageEngine *e, const char *pattern) {
    if (!pattern) return 0;
    for (int i = 0; i < e->tokens.count; i++)
        if (strstr(e->tokens.tokens[i].text, pattern)) return 1;
    return 0;
}

int lang_match_exact(const LanguageEngine *e, const char *word) {
    if (!word) return 0;
    for (int i = 0; i < e->tokens.count; i++)
        if (strcmp(e->tokens.tokens[i].text, word) == 0) return 1;
    return 0;
}

int lang_word_frequency(const LanguageEngine *e, const char *word) {
    if (!word) return 0;
    int c = 0;
    for (int i = 0; i < e->word_count; i++)
        if (strcmp(e->words[i], word) == 0) c++;
    return c;
}

int lang_most_common(LanguageEngine *e, char *out, int outsz) {
    if (e->word_count == 0 || !out || outsz <= 0) return -1;

    int best_count = 0;
    const char *best = e->words[0];
    for (int i = 0; i < e->word_count; i++) {
        int c = 0;
        for (int j = 0; j < e->word_count; j++)
            if (strcmp(e->words[i], e->words[j]) == 0) c++;
        if (c > best_count) { best_count = c; best = e->words[i]; }
    }
    strncpy(out, best, outsz - 1);
    out[outsz - 1] = '\0';
    return best_count;
}

int lang_similarity(const char *a, const char *b) {
    if (!a || !b) return 0;
    int la = (int)strlen(a), lb = (int)strlen(b);
    if (la == 0 && lb == 0) return 100;
    int maxl = la > lb ? la : lb;

    int common = 0;
    int used[256];
    memset(used, 0, sizeof(int) * 256);
    for (int i = 0; i < lb; i++)
        used[(unsigned char)b[i]]++;

    for (int i = 0; i < la; i++) {
        unsigned char ch = (unsigned char)a[i];
        if (used[ch] > 0) { common++; used[ch]--; }
    }
    return (int)((long long)common * 100 / maxl);
}

int lang_is_number(const char *text) {
    if (!text || !*text) return 0;
    int dot = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] == '.') { if (dot++) return 0; continue; }
        if (!isdigit((unsigned char)text[i])) return 0;
    }
    return 1;
}

float lang_extract_number(const LanguageEngine *e, int index) {
    int seen = 0;
    for (int i = 0; i < e->tokens.count; i++) {
        if (e->tokens.tokens[i].type == TOK_NUMBER) {
            if (seen == index) return strtof(e->tokens.tokens[i].text, NULL);
            seen++;
        }
    }
    return 0.0f;
}
