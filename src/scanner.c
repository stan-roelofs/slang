#include "scanner.h"

#include "common.h"

void slang_scanner_init(slang_scanner *scanner, const char *source)
{
    scanner->token_start = source;
    scanner->token_current = source;
    scanner->line = 1;
}

static slang_token make_token(slang_scanner *scanner, slang_token_type type)
{
    slang_token token;
    token.type = type;
    token.start = scanner->token_start;
    token.length = (unsigned)(scanner->token_current - scanner->token_start);
    token.line = scanner->line;
    return token;
}

static char next_token(slang_scanner *scanner)
{
    return *scanner->token_current++;
}

static char peek_token(slang_scanner *scanner)
{
    return *scanner->token_current;
}

static bool is_at_end(slang_scanner *scanner)
{
    return peek_token(scanner) == '\0';
}

static void skip_whitespace(slang_scanner *scanner)
{
    while (true)
    {
        switch (peek_token(scanner))
        {
        case ' ':
        case '\r':
        case '\t':
            next_token(scanner);
            break;
        case '\n':
            scanner->line++;
            next_token(scanner);
            break;
        case '/':
            if (peek_token(scanner) == '/')
            {
                // A comment goes until the end of the line
                while (peek_token(scanner) != '\n' && !is_at_end(scanner))
                    next_token(scanner);
            }
            else
            {
                return;
            }
            break;
        default:
            return;
        }
    }
}

/// Peek the next token
static bool match(slang_scanner *scanner, char expected)
{
    if (is_at_end(scanner))
        return false;
    if (*scanner->token_current != expected)
        return false;

    scanner->token_current++;
    return true;
}

slang_token slang_scanner_scan_token(slang_scanner *scanner)
{
    scanner->token_start = scanner->token_current;

    skip_whitespace(scanner);

    if (is_at_end(scanner))
        return make_token(scanner, TOKEN_EOF);

    char c = next_token(scanner);
    switch (c)
    {
    // One character tokens
    case '(':
        return make_token(scanner, TOKEN_LEFT_PARENTHESIS);
    case ')':
        return make_token(scanner, TOKEN_RIGHT_PARENTHESIS);
    case '{':
        return make_token(scanner, TOKEN_LEFT_BRACE);
    case '}':
        return make_token(scanner, TOKEN_RIGHT_BRACE);
    case ',':
        return make_token(scanner, TOKEN_COMMA);
    case '.':
        return make_token(scanner, TOKEN_DOT);
    case '-':
        return make_token(scanner, TOKEN_MINUS);
    case '+':
        return make_token(scanner, TOKEN_PLUS);
    case ';':
        return make_token(scanner, TOKEN_SEMICOLON);
    case '/':
        return make_token(scanner, TOKEN_SLASH);
    case '*':
        return make_token(scanner, TOKEN_STAR);
    // One or two character tokens
    case '!':
        return make_token(scanner, match(scanner, '=') ? TOKEN_NEGATE_EQUAL : TOKEN_NEGATE);
    case '=':
        return make_token(scanner, match(scanner, '=' ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL));
    case '>':
        return make_token(scanner, match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
        return make_token(scanner, match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    }

    return make_token(scanner, TOKEN_ERROR); // TODO
}