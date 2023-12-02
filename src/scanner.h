#ifndef SLANG_SCANNER_H
#define SLANG_SCANNER_H

typedef struct
{
    const char *token_start;   // start of the current token
    const char *token_current; // current character
    unsigned line;             // current line for error reporting / debugging
} slang_scanner;

typedef enum
{
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,

    TOKEN_NEGATE,
    TOKEN_EQUAL,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_NEGATE_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS_EQUAL,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_VARIABLE,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_SWITCH,

    // Other
    TOKEN_ERROR,
    TOKEN_EOF
} slang_token_type;

typedef struct
{
    slang_token_type type;
    const char *start;
    unsigned length;
    unsigned line;
} slang_token;

/**
 * Initializes a scanner with the given source code.
 * \param scanner The scanner to initialize
 * \param source The source code to scan, this must be a null-terminated string.
 *               The scanner does not make a copy of the source code, so the source code must outlive the scanner.
 */
void slang_scanner_init(slang_scanner *scanner, const char *source);

/**
 * Scans the next token from the source code.
 * \param scanner The scanner to scan the next token from *
 * \return The next token from the source code. On error the token type will be TOKEN_ERROR an error message is written to the token's start.
 */
slang_token slang_scanner_scan_token(slang_scanner *scanner);

#endif