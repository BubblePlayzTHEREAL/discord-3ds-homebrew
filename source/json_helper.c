#define JSMN_STATIC
#include "json_helper.h"
#include <string.h>
#include <stdio.h>

bool json_token_equals(const char* json, jsmntok_t* tok, const char* s) {
    if (tok->type == JSMN_STRING && 
        (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return true;
    }
    return false;
}

int json_get_string(const char* json, jsmntok_t* tok, char* output, size_t max_len) {
    if (tok == NULL || tok->type != JSMN_STRING) {
        return -1;
    }
    
    size_t len = tok->end - tok->start;
    if (len >= max_len) {
        len = max_len - 1;
    }
    
    strncpy(output, json + tok->start, len);
    output[len] = '\0';
    return 0;
}

jsmntok_t* json_find_token(const char* json, jsmntok_t* tokens, int num_tokens, const char* key) {
    for (int i = 0; i < num_tokens - 1; i++) {
        if (json_token_equals(json, &tokens[i], key)) {
            return &tokens[i + 1];
        }
    }
    return NULL;
}

int json_parse(const char* json, jsmntok_t* tokens, size_t max_tokens) {
    jsmn_parser parser;
    jsmn_init(&parser);
    return jsmn_parse(&parser, json, strlen(json), tokens, max_tokens);
}
