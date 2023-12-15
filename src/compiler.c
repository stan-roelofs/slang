#include "compiler.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static void emit_byte(slang_parser *parser, uint8_t byte)
{
    slang_chunk_write(current_chunk(parser), byte, parser->previous.line);
}

static void emit_byte2(slang_parser *parser, uint8_t byte1, uint8_t byte2)
{
    emit_byte(parser, byte1);
    emit_byte(parser, byte2);
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

static void expect_token(slang_parser *parser, slang_token_type type, const char *message)
{
    if (parser->current.type != type)
        report_error(parser, &parser->current, message);
}

typedef void (*slang_parse_fn)(slang_parser *parser);

static void expression(slang_parser *parser);

static void number(slang_parser *parser)
{
    // Our token is not null-terminated, so we need to copy it into a null-terminated buffer
    char *buffer = slang_allocate(parser->current.length + 1);
    memcpy(buffer, parser->current.start, parser->current.length);
    buffer[parser->current.length] = '\0';
    double value = strtod(buffer, NULL);
    slang_free(buffer);

    size_t index = slang_chunk_write_constant(current_chunk(parser), value);
    // OP_CONSTANT only has 1 byte for the index, so we can only have 256 constants for now...
    if (index > 0xFF)
    {
        report_error(parser, &parser->previous, "Too many constants in one chunk");
        return;
    }

    emit_byte2(parser, OP_CONSTANT, (uint8_t)index);
}

static void unary(slang_parser *parser)
{
    slang_token_type token_type = parser->current.type;

    // Compiles the operand
    expression(parser);

    // Emit the operator instruction
    switch (token_type)
    {
    case TOKEN_MINUS:
        emit_byte(parser, OP_NEGATE);
        break;
    default:
        assert(false); // We should not have reached this point
        return;
    }
}

static void grouping(slang_parser *parser)
{
    expression(parser);
    advance(parser);
    expect_token(parser, TOKEN_RIGHT_PARENTHESIS, "Expected ')' after expression");
}

static slang_parse_fn get_prefix_parser(slang_token_type type)
{
    static slang_parse_fn prefix_parsers[] = {
        [TOKEN_LEFT_PARENTHESIS] = grouping,
        [TOKEN_RIGHT_PARENTHESIS] = NULL,
        [TOKEN_LEFT_BRACE] = NULL,
        [TOKEN_RIGHT_BRACE] = NULL,
        [TOKEN_COMMA] = NULL,
        [TOKEN_DOT] = NULL,
        [TOKEN_MINUS] = unary,
        [TOKEN_PLUS] = NULL,
        [TOKEN_SEMICOLON] = NULL,
        [TOKEN_SLASH] = NULL,
        [TOKEN_STAR] = NULL,
        [TOKEN_OR] = NULL,
        [TOKEN_OR_OR] = NULL,
        [TOKEN_AND] = NULL,
        [TOKEN_AND_AND] = NULL,
        [TOKEN_NEGATE] = NULL,
        [TOKEN_EQUAL] = NULL,
        [TOKEN_GREATER] = NULL,
        [TOKEN_LESS] = NULL,
        [TOKEN_NEGATE_EQUAL] = NULL,
        [TOKEN_EQUAL_EQUAL] = NULL,
        [TOKEN_GREATER_EQUAL] = NULL,
        [TOKEN_LESS_EQUAL] = NULL,
        [TOKEN_IDENTIFIER] = NULL,
        [TOKEN_STRING] = NULL,
        [TOKEN_NUMBER] = number,
        [TOKEN_IF] = NULL,
        [TOKEN_ELSE] = NULL,
        [TOKEN_FOR] = NULL,
        [TOKEN_WHILE] = NULL,
        [TOKEN_VARIABLE] = NULL,
        [TOKEN_FUNCTION] = NULL,
        [TOKEN_RETURN] = NULL,
        [TOKEN_ERROR] = NULL,
        [TOKEN_EOF] = NULL};

    return prefix_parsers[type];
}

static void expression(slang_parser *parser)
{
    advance(parser);
    slang_parse_fn prefix_parser = get_prefix_parser(parser->current.type);
    if (prefix_parser == NULL)
    {
        char *error = slang_allocate(50 + parser->current.length);
        sprintf(error, "Unexpected token '%.*s', expected expression", parser->current.length, parser->current.start);
        report_error(parser, &parser->previous, error);
        slang_free(error);
        return;
    }

    prefix_parser(parser);
}

bool slang_compile(slang_chunk *chunk, const char *source, char **error_message)
{
    slang_parser parser;
    parser.chunk = chunk;
    parser.error_message = error_message;
    parser.had_error = false;
    parser.suppress_errors = false;
    slang_scanner_init(&parser.scanner, source);

    expression(&parser);

    // emit_byte(&parser, OP_RETURN); // TODO temporary

    return parser.had_error == false;
}