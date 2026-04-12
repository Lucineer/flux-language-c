#include <stdio.h>
#include <string.h>
#include <math.h>
#include "language.h"

static int tests_run = 0, tests_passed = 0;

#define ASSERT(cond, msg) do { \
    tests_run++; \
    if (cond) { tests_passed++; } \
    else { printf("  FAIL: %s\n", msg); } \
} while(0)

int main(void) {
    LanguageEngine e;

    /* 1. Init */
    lang_init(&e);
    ASSERT(e.tokens.count == 0, "init clears tokens");
    ASSERT(e.pattern_count == 0, "init clears patterns");

    /* 2. Basic tokenize */
    lang_tokenize(&e, "hello world");
    ASSERT(e.tokens.count == 3, "tokenize hello world (word space word)");
    ASSERT(e.tokens.tokens[0].type == TOK_WORD, "first token is WORD");
    ASSERT(strcmp(e.tokens.tokens[0].text, "hello") == 0, "first token text");

    /* 3. Tokenize with numbers and punct */
    lang_tokenize(&e, "foo 42, bar!");
    ASSERT(e.tokens.count == 7, "mixed tokens count");
    ASSERT(e.tokens.tokens[1].type == TOK_SPACE, "space token");
    ASSERT(e.tokens.tokens[2].type == TOK_NUMBER, "number token");
    ASSERT(e.tokens.tokens[3].type == TOK_PUNCT, "punct token");

    /* 4. Strict tokenize (skip spaces) */
    lang_tokenize_strict(&e, "hello   world  foo");
    ASSERT(e.tokens.count == 3, "strict skips spaces");
    for (int i = 0; i < e.tokens.count; i++)
        ASSERT(e.tokens.tokens[i].type != TOK_SPACE, "strict: no space tokens");

    /* 5. Count words */
    lang_tokenize(&e, "the quick brown fox");
    ASSERT(lang_count_words(&e) == 4, "count words = 4");

    /* 6. Contains */
    ASSERT(lang_contains(&e, "quick") == 1, "contains quick");
    ASSERT(lang_contains(&e, "slow") == 0, "does not contain slow");

    /* 7. Add pattern */
    int pi = lang_add_pattern(&e, "test");
    ASSERT(pi == 0, "pattern index 0");
    ASSERT(e.pattern_count == 1, "pattern count 1");
    lang_add_pattern(&e, "hello");
    ASSERT(e.pattern_count == 2, "pattern count 2");
    ASSERT(lang_add_pattern(&e, NULL) == -1, "null pattern rejected");

    /* 8. Match substring */
    lang_tokenize(&e, "testing the waters");
    ASSERT(lang_match(&e, "test") == 1, "substring match test in testing");
    ASSERT(lang_match(&e, "fire") == 0, "no substring match");

    /* 9. Exact match */
    lang_tokenize(&e, "hello hello world");
    ASSERT(lang_match_exact(&e, "hello") == 1, "exact match hello");
    ASSERT(lang_match_exact(&e, "hel") == 0, "no partial exact match");

    /* 10. Word frequency */
    lang_tokenize(&e, "the cat sat on the mat the");
    ASSERT(lang_word_frequency(&e, "the") == 3, "frequency of 'the' = 3");
    ASSERT(lang_word_frequency(&e, "cat") == 1, "frequency of 'cat' = 1");
    ASSERT(lang_word_frequency(&e, "dog") == 0, "frequency of 'dog' = 0");

    /* 11. Most common */
    char buf[64];
    int freq = lang_most_common(&e, buf, sizeof(buf));
    ASSERT(freq == 3, "most common freq = 3");
    ASSERT(strcmp(buf, "the") == 0, "most common word = the");

    /* 12. Similarity */
    ASSERT(lang_similarity("abc", "abc") == 100, "identical similarity 100");
    ASSERT(lang_similarity("abc", "xyz") == 0, "no similarity 0");
    ASSERT(lang_similarity("abc", "abd") > 50, "partial similarity > 50");
    ASSERT(lang_similarity(NULL, "abc") == 0, "null a = 0");
    ASSERT(lang_similarity("", "") == 100, "empty = 100");

    /* 13. Is number */
    ASSERT(lang_is_number("42") == 1, "42 is number");
    ASSERT(lang_is_number("3.14") == 1, "3.14 is number");
    ASSERT(lang_is_number("abc") == 0, "abc not number");
    ASSERT(lang_is_number("") == 0, "empty not number");
    ASSERT(lang_is_number("3.14.15") == 0, "double dot not number");

    /* 14. Extract number */
    lang_tokenize(&e, "I have 3 cats and 15 dogs");
    float n0 = lang_extract_number(&e, 0);
    float n1 = lang_extract_number(&e, 1);
    ASSERT(fabsf(n0 - 3.0f) < 0.001f, "first number = 3");
    ASSERT(fabsf(n1 - 15.0f) < 0.001f, "second number = 15");
    ASSERT(lang_extract_number(&e, 5) == 0.0f, "out of bounds = 0");

    /* 15. Empty/null tokenize */
    lang_init(&e);
    ASSERT(lang_tokenize(&e, "") == 0, "empty string = 0 tokens");
    ASSERT(lang_tokenize(&e, NULL) == 0, "null = 0 tokens");

    /* 16. Most common empty */
    int mc = lang_most_common(&e, buf, sizeof(buf));
    ASSERT(mc == -1, "most common empty = -1");

    printf("\n%d / %d tests passed\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
