#include "parsers/file_parsers.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

// Simple PDF text extraction
static int is_text_char(unsigned char c) {
    return (c >= 32 && c < 127) || c == '\n' || c == '\r' || c == '\t';
}

// Extract text from PDF content stream
static char* extract_text_from_stream(const char *stream, size_t stream_len) {
    size_t text_size = stream_len + 1;
    char *text = static_cast<char*>(malloc(text_size));
    if (!text) {
        return nullptr;
    }

    size_t text_pos = 0;
    int in_text_block = 0;
    int in_parentheses = 0;

    for (size_t i = 0; i < stream_len - 1; i++) {
        // Look for text showing operators: BT (Begin Text), ET (End Text)
        if (stream[i] == 'B' && stream[i+1] == 'T' &&
            (i == 0 || isspace(static_cast<unsigned char>(stream[i-1]))) &&
            (i+2 >= stream_len || isspace(static_cast<unsigned char>(stream[i+2])))) {
            in_text_block = 1;
            i += 1;
            continue;
        }

        if (stream[i] == 'E' && stream[i+1] == 'T' &&
            (i == 0 || isspace(static_cast<unsigned char>(stream[i-1]))) &&
            (i+2 >= stream_len || isspace(static_cast<unsigned char>(stream[i+2])))) {
            in_text_block = 0;
            i += 1;
            continue;
        }

        if (!in_text_block) {
            continue;
        }

        // Look for text in parentheses: (text)
        if (stream[i] == '(' && (i == 0 || stream[i-1] != '\\')) {
            in_parentheses = 1;
            continue;
        }

        if (stream[i] == ')' && (i == 0 || stream[i-1] != '\\')) {
            in_parentheses = 0;
            if (text_pos > 0 && text[text_pos-1] != '\n') {
                text[text_pos++] = ' ';
            }
            continue;
        }

        if (in_parentheses) {
            if (stream[i] == '\\' && i+1 < stream_len) {
                i++;
                switch (stream[i]) {
                    case 'n': text[text_pos++] = '\n'; break;
                    case 'r': text[text_pos++] = '\r'; break;
                    case 't': text[text_pos++] = '\t'; break;
                    case '(': text[text_pos++] = '('; break;
                    case ')': text[text_pos++] = ')'; break;
                    case '\\': text[text_pos++] = '\\'; break;
                    default: text[text_pos++] = stream[i]; break;
                }
            } else {
                text[text_pos++] = stream[i];
            }
        }

        // Reallocate if needed
        if (text_pos >= text_size - 1) {
            text_size *= 2;
            char *new_text = static_cast<char*>(realloc(text, text_size));
            if (!new_text) {
                free(text);
                return nullptr;
            }
            text = new_text;
        }
    }

    text[text_pos] = '\0';
    return text;
}

// Parse PDF file and extract text content
parse_result_t* parse_pdf_file(const char *filename) {
    if (!filename) {
        return nullptr;
    }

    parse_result_t *result = static_cast<parse_result_t*>(calloc(1, sizeof(parse_result_t)));
    if (!result) {
        return nullptr;
    }

    size_t file_length = 0;
    char *file_content = read_file_contents(filename, &file_length);

    if (!file_content) {
        result->success = 0;
        result->error_message = strdup("Failed to read PDF file");
        return result;
    }

    // Check PDF header
    if (file_length < 5 || memcmp(file_content, "%PDF-", 5) != 0) {
        free(file_content);
        result->success = 0;
        result->error_message = strdup("Invalid PDF file format");
        return result;
    }

    char *extracted_text = extract_text_from_stream(file_content, file_length);

    free(file_content);

    if (!extracted_text) {
        result->success = 0;
        result->error_message = strdup("Failed to extract text from PDF");
        return result;
    }

    result->content = extracted_text;
    result->content_length = strlen(extracted_text);
    result->success = 1;
    result->error_message = nullptr;

    return result;
}
