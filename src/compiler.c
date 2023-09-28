#include "compiler.h"

#include "memory.h"
#include "scanner.h"
#include "util.h"

slang_chunk *slang_compile(const char *source, char **error_message)
{
    (void)error_message;
    slang_scanner scanner;
    slang_scanner_init(&scanner, source);

    slang_chunk *chunk = slang_allocate(sizeof(slang_chunk));
    slang_chunk_init(chunk);

    while (true)
    {
        slang_token token = slang_scanner_scan_token(&scanner);

        slang_trace("Scanned token: %.*s\n", token.length, token.start);

        switch (token.type)
        {
        case TOKEN_ERROR:
        {
            slang_set_error(error_message, "Compiling failed: %.*s\n", token.length, token.start); // TODO: use a proper error message
            slang_chunk_free(chunk);
            return NULL;
        }
        break;

        case TOKEN_EOF:
        {
            return chunk;
        }
        }
    }

    return chunk;
}