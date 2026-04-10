#include "parsers/file_parsers.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

// Parse Markdown file - extract content and convert to plain text
parse_result_t* parse_md_file(const char *filename) {
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
        result->error_message = strdup("Failed to read MD file");
        return result;
    }

    char *processed = static_cast<char*>(malloc(file_length + 1));
    if (!processed) {
        free(file_content);
        result->success = 0;
        result->error_message = strdup("Memory allocation failed");
        return result;
    }

    size_t out_pos = 0;
    size_t in_pos = 0;
    int in_code_block = 0;
    int at_line_start = 1;

    while (in_pos < file_length) {
        char c = file_content[in_pos];

        // Handle code blocks
        if (at_line_start && in_pos + 2 < file_length &&
            file_content[in_pos] == '`' &&
            file_content[in_pos + 1] == '`' &&
            file_content[in_pos + 2] == '`') {
            in_code_block = !in_code_block;
            in_pos += 3;
            while (in_pos < file_length && file_content[in_pos] != '\n') {
                in_pos++;
            }
            if (in_pos < file_length) {
                in_pos++;
            }
            at_line_start = 1;
            continue;
        }

        // If in code block, copy as-is
        if (in_code_block) {
            processed[out_pos++] = c;
            at_line_start = (c == '\n');
            in_pos++;
            continue;
        }

        // Remove markdown headers
        if (at_line_start && c == '#') {
            while (in_pos < file_length &&
                   (file_content[in_pos] == '#' || file_content[in_pos] == ' ')) {
                in_pos++;
            }
            at_line_start = 0;
            continue;
        }

        // Remove bold/italic markers
        if (c == '*') {
            in_pos++;
            if (in_pos < file_length && file_content[in_pos] == '*') {
                in_pos++;
            }
            continue;
        }

        // Handle underscore
        if (c == '_') {
            int prev_alnum = (in_pos > 0 && isalnum(static_cast<unsigned char>(file_content[in_pos - 1])));
            int next_alnum = (in_pos + 1 < file_length && isalnum(static_cast<unsigned char>(file_content[in_pos + 1])));

            if (prev_alnum && next_alnum) {
                processed[out_pos++] = c;
                at_line_start = 0;
                in_pos++;
                continue;
            } else {
                in_pos++;
                if (in_pos < file_length && file_content[in_pos] == '_') {
                    in_pos++;
                }
                continue;
            }
        }

        // Remove inline code markers
        if (c == '`') {
            in_pos++;
            continue;
        }

        // Copy regular character
        processed[out_pos++] = c;
        at_line_start = (c == '\n');
        in_pos++;
    }

    processed[out_pos] = '\0';

    result->content = processed;
    result->content_length = out_pos;
    result->success = 1;
    result->error_message = nullptr;

    free(file_content);
    return result;
}
