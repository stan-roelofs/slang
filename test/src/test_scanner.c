#include "test.h"

#include "scanner.h"

START_TEST_SUITE(scanner)

ADD_TEST(init)
{
    slang_scanner scanner;
    const char *source = "test";
    slang_scanner_init(&scanner, source);

    EXPECT(scanner.token_start == source);
    EXPECT(scanner.token_current == source);
    EXPECT(scanner.line == 1);
}

slang_scanner scanner;

ADD_TEST(skips_whitespace)
{
    slang_scanner_init(&scanner, "   )     \n\r\r\r\r\t\t\t\t\t   (");
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_RIGHT_PARENTHESIS);
    token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_LEFT_PARENTHESIS);
}

ADD_TEST(newlines_increase_line_count)
{
    slang_scanner_init(&scanner, ")\n\n\n\n\n(");
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_RIGHT_PARENTHESIS);
    EXPECT(token.line == 1);
    token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_LEFT_PARENTHESIS);
    EXPECT(token.line == 6);
}

ADD_TEST(comments)
{
    slang_scanner_init(&scanner, ") // This is a comment\n // also a comment \n (");

    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_RIGHT_PARENTHESIS);
    token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_LEFT_PARENTHESIS);
}

ADD_TEST(eof)
{
    slang_scanner_init(&scanner, "");
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_EOF);
}

#define TEST_TOKEN(token_type, token_string)                         \
    slang_scanner_init(&scanner, token_string);                      \
    slang_token token = slang_scanner_scan_token(&scanner);          \
    EXPECT(token.type == token_type);                                \
    EXPECT(STRING_EQUAL_N(token.start, token_string, token.length)); \
    EXPECT(token.length == strlen(token_string));                    \
    EXPECT(token.line == 1);

ADD_TEST(left_parenthesis)
{
    TEST_TOKEN(TOKEN_LEFT_PARENTHESIS, "(");
}

ADD_TEST(right_parenthesis)
{
    TEST_TOKEN(TOKEN_RIGHT_PARENTHESIS, ")");
}

ADD_TEST(left_brace)
{
    TEST_TOKEN(TOKEN_LEFT_BRACE, "{");
}

ADD_TEST(right_brace)
{
    TEST_TOKEN(TOKEN_RIGHT_BRACE, "}");
}

ADD_TEST(comma)
{
    TEST_TOKEN(TOKEN_COMMA, ",");
}

ADD_TEST(dot)
{
    TEST_TOKEN(TOKEN_DOT, ".");
}

ADD_TEST(minus)
{
    TEST_TOKEN(TOKEN_MINUS, "-");
}

ADD_TEST(plus)
{
    TEST_TOKEN(TOKEN_PLUS, "+");
}

ADD_TEST(semicolon)
{
    TEST_TOKEN(TOKEN_SEMICOLON, ";");
}

ADD_TEST(slash)
{
    TEST_TOKEN(TOKEN_SLASH, "/");
}

ADD_TEST(star)
{
    TEST_TOKEN(TOKEN_STAR, "*");
}

ADD_TEST(negate)
{
    TEST_TOKEN(TOKEN_NEGATE, "!");
}

ADD_TEST(negate_equal)
{
    TEST_TOKEN(TOKEN_NEGATE_EQUAL, "!=");
}

ADD_TEST(equal)
{
    TEST_TOKEN(TOKEN_EQUAL, "=");
}

ADD_TEST(equal_equal)
{
    TEST_TOKEN(TOKEN_EQUAL_EQUAL, "==");
}

ADD_TEST(greater)
{
    TEST_TOKEN(TOKEN_GREATER, ">");
}

ADD_TEST(greater_equal)
{
    TEST_TOKEN(TOKEN_GREATER_EQUAL, ">=");
}

ADD_TEST(less)
{
    TEST_TOKEN(TOKEN_LESS, "<");
}

ADD_TEST(less_equal)
{
    TEST_TOKEN(TOKEN_LESS_EQUAL, "<=");
}

ADD_TEST(string_literal)
{
    const char *string_literal = "\"This is a string literal\"";
    slang_scanner_init(&scanner, string_literal);
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_STRING);
    EXPECT(token.length == strlen(string_literal));
    EXPECT(token.line == 1);
    EXPECT(STRING_EQUAL_N(token.start, string_literal, token.length));
}

ADD_TEST(multiline_string_literal)
{
    const char *string_literal = "\"This is a string literal\nwith multiple lines\"";
    slang_scanner_init(&scanner, string_literal);
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_STRING);
    EXPECT(token.length == strlen(string_literal));
    EXPECT(token.line == 2);
    EXPECT(STRING_EQUAL_N(token.start, string_literal, token.length));
}

ADD_TEST(unterminated_string_literal)
{
    const char *string_literal = "\"This is an unterminated string literal";
    slang_scanner_init(&scanner, string_literal);
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_ERROR);
    EXPECT(token.line == 1);
}

ADD_TEST(number)
{
    const char *number_literal = "1234";
    slang_scanner_init(&scanner, number_literal);
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_NUMBER);
    EXPECT(token.length == strlen(number_literal));
    EXPECT(token.line == 1);
    EXPECT(STRING_EQUAL_N(token.start, number_literal, token.length));
}

ADD_TEST(number_with_decimal)
{
    const char *number_literal = "1234.5678";
    slang_scanner_init(&scanner, number_literal);
    slang_token token = slang_scanner_scan_token(&scanner);
    EXPECT(token.type == TOKEN_NUMBER);
    EXPECT(token.length == strlen(number_literal));
    EXPECT(token.line == 1);
    EXPECT(STRING_EQUAL_N(token.start, number_literal, token.length));
}

END_TEST_SUITE()