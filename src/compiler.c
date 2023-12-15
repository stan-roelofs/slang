#include "compiler.h"

#include "memory.h"
#include "opcode.h"
#include "scanner.h"
#include "util.h"

typedef struct
{
    slang_scanner scanner;
    slang_chunk *chunk;

    char **error_message; // Buffer that will be filled with the error message
    slang_token current;
    slang_token previous;
    bool had_error;
    bool suppress_errors;
} slang_parser;

static slang_chunk *current_chunk(slang_parser *parser)
{
    return parser->chunk;
}

static void report_error(slang_parser *parser, slang_token *token, const char *message)
{
    if (parser->suppress_errors)
        return;

    parser->suppress_errors = true;
    parser->had_error = true;

    slang_append_error(parser->error_message, "[line %d] error: %s \n", token->line, message);
}

static void advance(slang_parser *parser)
{
    parser->previous = parser->current;

    while (true)
    {
        parser->current = slang_scanner_scan_token(&parser->scanner);
        if (parser->current.type != TOKEN_ERROR)
            return;

        report_error(parser, &parser->current, parser->current.start);
    }
}

static void consume(slang_parser *parser, slang_token_type type, const char *message)
{
    if (parser->current.type == type)
    {
        advance(parser);
        return;
    }

    report_error(parser, &parser->current, message);
}

static void emit_byte(slang_parser *parser, uint8_t byte)
{
    slang_chunk_write(current_chunk(parser), byte, parser->previous.line);
}

static void emit_byte2(slang_parser *parser, uint8_t byte1, uint8_t byte2)
{
    emit_byte(parser, byte1);
    emit_byte(parser, byte2);
}

typedef void (*slang_parse_fn)(slang_parser *parser);

static void prefix(slang_parser *parser)
{
    slang_parse_fn parse_fn = parse_table[parser->previous.type];
    if (!parse_fn)
    {
        report_error(parser, &parser->previous, "Expected expression");
        return;
    }

    parse_fn(parser);
}

static slang_parse_fn parse_table[] =
    {
        [TOKEN_MINUS] = prefix,
};

static void parse_expression(slang_parser *parser)
{
}

bool slang_compile(slang_chunk *chunk, const char *source, char **error_message)
{
    slang_parser parser;
    parser.chunk = chunk;
    parser.error_message = error_message;
    parser.had_error = false;
    parser.suppress_errors = false;
    slang_scanner_init(&parser.scanner, source);

    advance(&parser);
    parse_expression(&parser);

    emit_byte(&parser, OP_RETURN); // TODO temporary

    return parser.had_error == false;
}