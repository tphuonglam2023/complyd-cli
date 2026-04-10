#ifndef GRC_SCANNER_HPP
#define GRC_SCANNER_HPP

#include <cstddef>

// Configuration item structure
struct config_item_t {
    char *key;
    char *value;
};

// Configuration structure
struct config_t {
    config_item_t *items;
    size_t count;
    size_t capacity;
};

// Scanner core functions
config_t* scanner_load_config(const char *filepath);
void scanner_free_config(config_t *config);
char* config_to_string(const config_t *config);

#endif // GRC_SCANNER_HPP
