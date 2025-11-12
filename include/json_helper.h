#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#define JSMN_HEADER
#include "jsmn.h"

#include <stdbool.h>

// Helper function to compare JSON token with string
bool json_token_equals(const char* json, jsmntok_t* tok, const char* s);

// Get string value from JSON token
int json_get_string(const char* json, jsmntok_t* tok, char* output, size_t max_len);

// Find a key in JSON object and return its value token
jsmntok_t* json_find_token(const char* json, jsmntok_t* tokens, int num_tokens, const char* key);

// Parse JSON string
int json_parse(const char* json, jsmntok_t* tokens, size_t max_tokens);

#endif // JSON_HELPER_H
