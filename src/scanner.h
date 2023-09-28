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

void slang_scanner_init(slang_scanner *scanner, const char *source);
slang_token slang_scanner_scan_token(slang_scanner *scanner);

#endif