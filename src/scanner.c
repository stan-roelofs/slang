#include "scanner.h"

void slang_scanner_init(slang_scanner *scanner, const char *source)
{
    scanner->token_start = source;
    scanner->token_current = source;
    scanner->line = 1;
}

slang_token slang_scanner_scan_token(slang_scanner *scanner)
{
    slang_token token;
    token.type = TOKEN_ERROR;
    token.start = scanner->token_current;
    token.length = 1;
    token.line = scanner->line;
    return token; // TODO!!
}