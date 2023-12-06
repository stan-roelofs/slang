#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

static slang_token make_error_token(slang_scanner *scanner, const char *message)
{
    slang_token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (unsigned)strlen(message);
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

static char peek_next_token(slang_scanner *scanner)
{
    if (is_at_end(scanner))
        return '\0';
    return scanner->token_current[1];
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
            if (peek_next_token(scanner) == '/')
            {
                while (peek_token(scanner) != '\n' && !is_at_end(scanner))
                    next_token(scanner);
            }
            else
                return; // break from loop
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

static bool is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static bool is_keyword_or_identifier(char c, bool start_of_token)
{
    if (start_of_token)
        return is_letter(c) || c == '_';

    return is_letter(c) || is_digit(c) || c == '_';
}

static slang_token make_string(slang_scanner *scanner)
{
    while (peek_token(scanner) != '"' && !is_at_end(scanner))
    {
        if (peek_token(scanner) == '\n')
            scanner->line++;
        next_token(scanner);
    }

    if (is_at_end(scanner))
    {
        char message[64];
        snprintf(message, sizeof(message), "Unterminated string starting at %d", scanner->line);
        return make_error_token(scanner, message);
    }

    // The closing "
    next_token(scanner);
    return make_token(scanner, TOKEN_STRING);
}

static slang_token make_number(slang_scanner *scanner)
{
    while (is_digit(peek_token(scanner)))
        next_token(scanner);

    if (peek_token(scanner) == '.' && is_digit(peek_next_token(scanner)))
    {
        // Skip the "."
        next_token(scanner);

        while (is_digit(peek_token(scanner)))
            next_token(scanner);
    }

    return make_token(scanner, TOKEN_NUMBER);
}

static slang_token finish_keyword_else_identifier(slang_scanner *scanner, unsigned start, unsigned length, const char *rest, slang_token_type type)
{
    if (scanner->token_current - scanner->token_start == start + length &&
        memcmp(scanner->token_start + start, rest, length) == 0)
    {
        return make_token(scanner, type);
    }

    return make_token(scanner, TOKEN_IDENTIFIER);
}

static slang_token make_keyword_or_identifier(slang_scanner *scanner)
{
    char next_char = peek_token(scanner);
    while (is_keyword_or_identifier(next_char, scanner->token_current == scanner->token_start))
    {
        next_token(scanner);
        next_char = peek_token(scanner);
    }

    // We could properly implement a trie for this, but since our keywords are always the same
    // we can just build it by hand using a switch statement.
    switch (scanner->token_start[0])
    {
    case 'e':
        return finish_keyword_else_identifier(scanner, 1, 3, "lse", TOKEN_ELSE);
    case 'f':
        if (scanner->token_current - scanner->token_start > 1)
        {
            switch (scanner->token_start[1])
            {
            case 'o':
                return finish_keyword_else_identifier(scanner, 2, 1, "r", TOKEN_FOR);
            case 'u':
                return finish_keyword_else_identifier(scanner, 2, 6, "nction", TOKEN_FUNCTION);
            }
        }
        return finish_keyword_else_identifier(scanner, 1, 1, "n", TOKEN_FUNCTION);
    case 'i':
        return finish_keyword_else_identifier(scanner, 1, 1, "f", TOKEN_IF);
    case 'r':
        return finish_keyword_else_identifier(scanner, 1, 5, "eturn", TOKEN_RETURN);
    case 'v':
        return finish_keyword_else_identifier(scanner, 1, 2, "ar", TOKEN_VARIABLE);
    case 'w':
        return finish_keyword_else_identifier(scanner, 1, 4, "hile", TOKEN_WHILE);
    default:
        break;
    }

    return make_token(scanner, TOKEN_IDENTIFIER);
}

slang_token slang_scanner_scan_token(slang_scanner *scanner)
{
    scanner->token_start = scanner->token_current;

    skip_whitespace(scanner);

    if (is_at_end(scanner))
        return make_token(scanner, TOKEN_EOF);

    if (is_keyword_or_identifier(peek_token(scanner), true))
        return make_keyword_or_identifier(scanner);

    if (is_digit(peek_token(scanner)))
        return make_number(scanner);

    char c = next_token(scanner);
    switch (c)
    {
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
    case '!':
        return make_token(scanner, match(scanner, '=') ? TOKEN_NEGATE_EQUAL : TOKEN_NEGATE);
    case '=':
        return make_token(scanner, match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '>':
        return make_token(scanner, match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
        return make_token(scanner, match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '|':
        return make_token(scanner, match(scanner, '|') ? TOKEN_OR_OR : TOKEN_OR);
    case '&':
        return make_token(scanner, match(scanner, '&') ? TOKEN_AND_AND : TOKEN_AND);
    // Literals
    case '"':
        return make_string(scanner);
    default:
        break;
    }

    return make_error_token(scanner, "Unexpected character");
}