#include "discord_api.h"
#include "json_helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#define DISCORD_API_BASE "https://discord.com/api/v10"
#define MAX_RESPONSE_SIZE (1024 * 512) // 512KB max response

// Structure to hold HTTP response
typedef struct {
    char* data;
    size_t size;
} HTTPResponse;

// Callback for curl to write response data
static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    HTTPResponse* resp = (HTTPResponse*)userp;
    
    char* ptr = realloc(resp->data, resp->size + realsize + 1);
    if (ptr == NULL) {
        printf("Failed to allocate memory for response\n");
        return 0;
    }
    
    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = 0;
    
    return realsize;
}

// Make HTTP GET request to Discord API
static char* discord_api_get(DiscordClient* client, const char* endpoint) {
    CURL* curl;
    CURLcode res;
    HTTPResponse response = {0};
    
    response.data = malloc(1);
    response.size = 0;
    
    curl = curl_easy_init();
    if (!curl) {
        free(response.data);
        return NULL;
    }
    
    char url[512];
    snprintf(url, sizeof(url), "%s%s", DISCORD_API_BASE, endpoint);
    
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: %s", client->token);
    
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "User-Agent: Discord3DS/1.0");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Discord3DS/1.0");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    
    res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(response.data);
        return NULL;
    }
    
    return response.data;
}

// Make HTTP POST request to Discord API
static char* discord_api_post(DiscordClient* client, const char* endpoint, const char* json_data) {
    CURL* curl;
    CURLcode res;
    HTTPResponse response = {0};
    
    response.data = malloc(1);
    response.size = 0;
    
    curl = curl_easy_init();
    if (!curl) {
        free(response.data);
        return NULL;
    }
    
    char url[512];
    snprintf(url, sizeof(url), "%s%s", DISCORD_API_BASE, endpoint);
    
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: %s", client->token);
    
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "User-Agent: Discord3DS/1.0");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    
    res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(response.data);
        return NULL;
    }
    
    return response.data;
}

void discord_init(DiscordClient* client, const char* token) {
    memset(client, 0, sizeof(DiscordClient));
    strncpy(client->token, token, sizeof(client->token) - 1);
    client->connected = false;
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

bool discord_connect(DiscordClient* client) {
    // Verify token by fetching user info
    char* response = discord_api_get(client, "/users/@me");
    if (!response) {
        printf("Failed to connect to Discord API\n");
        return false;
    }
    
    // Parse response to verify authentication
    jsmntok_t tokens[128];
    int r = json_parse(response, tokens, 128);
    
    if (r < 0) {
        printf("Failed to parse user info JSON\n");
        free(response);
        return false;
    }
    
    // Check if we got a valid user object (should have "id" field)
    jsmntok_t* id_token = json_find_token(response, tokens, r, "id");
    if (!id_token) {
        printf("Invalid token or authentication failed\n");
        free(response);
        return false;
    }
    
    free(response);
    
    client->connected = true;
    
    // Fetch initial server list
    discord_fetch_servers(client);
    
    // Set first server as current and get its first channel
    if (client->server_count > 0) {
        strcpy(client->current_server_id, client->servers[0].id);
        
        // Fetch channels for first server to set current_channel_id
        char endpoint[256];
        snprintf(endpoint, sizeof(endpoint), "/guilds/%s/channels", client->servers[0].id);
        char* channels_response = discord_api_get(client, endpoint);
        
        if (channels_response) {
            jsmntok_t channel_tokens[256];
            int ch_count = json_parse(channels_response, channel_tokens, 256);
            
            if (ch_count > 0 && channel_tokens[0].type == JSMN_ARRAY && channel_tokens[0].size > 0) {
                // Find first text channel
                for (int i = 1; i < ch_count; i++) {
                    jsmntok_t* type_token = json_find_token(channels_response, &channel_tokens[i], ch_count - i, "type");
                    if (type_token) {
                        char type_str[16];
                        json_get_string(channels_response, type_token, type_str, sizeof(type_str));
                        // Type 0 is text channel
                        if (strcmp(type_str, "0") == 0) {
                            jsmntok_t* ch_id_token = json_find_token(channels_response, &channel_tokens[i], ch_count - i, "id");
                            if (ch_id_token) {
                                json_get_string(channels_response, ch_id_token, client->current_channel_id, sizeof(client->current_channel_id));
                                break;
                            }
                        }
                    }
                }
            }
            
            free(channels_response);
        }
    }
    
    return true;
}

bool discord_fetch_messages(DiscordClient* client) {
    if (!client->connected || strlen(client->current_channel_id) == 0) {
        return false;
    }
    
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "/channels/%s/messages?limit=50", client->current_channel_id);
    
    char* response = discord_api_get(client, endpoint);
    if (!response) {
        printf("Failed to fetch messages\n");
        return false;
    }
    
    jsmntok_t tokens[2048]; // Large buffer for messages
    int r = json_parse(response, tokens, 2048);
    
    if (r < 0 || tokens[0].type != JSMN_ARRAY) {
        printf("Failed to parse messages JSON\n");
        free(response);
        return false;
    }
    
    // Clear existing messages
    client->message_count = 0;
    
    // Parse message array (tokens[0] is the array)
    int array_size = tokens[0].size;
    if (array_size > MAX_MESSAGES) {
        array_size = MAX_MESSAGES;
    }
    
    int tok_idx = 1; // Start after array token
    for (int i = 0; i < array_size && client->message_count < MAX_MESSAGES; i++) {
        if (tok_idx >= r || tokens[tok_idx].type != JSMN_OBJECT) {
            break;
        }
        
        DiscordMessage* msg = &client->messages[client->message_count];
        
        // Extract message fields
        jsmntok_t* id_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "id");
        jsmntok_t* content_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "content");
        jsmntok_t* timestamp_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "timestamp");
        
        // Get author object and extract username
        jsmntok_t* author_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "author");
        
        if (id_token) {
            json_get_string(response, id_token, msg->id, sizeof(msg->id));
        }
        
        if (content_token) {
            json_get_string(response, content_token, msg->content, sizeof(msg->content));
        }
        
        if (timestamp_token) {
            char full_timestamp[64];
            json_get_string(response, timestamp_token, full_timestamp, sizeof(full_timestamp));
            // Extract just time portion (HH:MM) from ISO 8601 timestamp
            if (strlen(full_timestamp) >= 16) {
                snprintf(msg->timestamp, sizeof(msg->timestamp), "%c%c:%c%c", 
                         full_timestamp[11], full_timestamp[12], 
                         full_timestamp[14], full_timestamp[15]);
            }
        }
        
        if (author_token) {
            // Find username within author object
            int author_idx = author_token - tokens;
            jsmntok_t* username_token = json_find_token(response, &tokens[author_idx], r - author_idx, "username");
            if (username_token) {
                json_get_string(response, username_token, msg->author, sizeof(msg->author));
            }
        }
        
        client->message_count++;
        
        // Skip to next object in array
        tok_idx += tokens[tok_idx].size + 1;
    }
    
    // Reverse messages so oldest is first
    for (int i = 0; i < client->message_count / 2; i++) {
        DiscordMessage temp = client->messages[i];
        client->messages[i] = client->messages[client->message_count - 1 - i];
        client->messages[client->message_count - 1 - i] = temp;
    }
    
    free(response);
    return true;
}

bool discord_fetch_servers(DiscordClient* client) {
    if (!client->connected) {
        return false;
    }
    
    char* response = discord_api_get(client, "/users/@me/guilds");
    if (!response) {
        printf("Failed to fetch servers\n");
        return false;
    }
    
    jsmntok_t tokens[512];
    int r = json_parse(response, tokens, 512);
    
    if (r < 0 || tokens[0].type != JSMN_ARRAY) {
        printf("Failed to parse servers JSON\n");
        free(response);
        return false;
    }
    
    client->server_count = 0;
    int array_size = tokens[0].size;
    if (array_size > MAX_SERVERS) {
        array_size = MAX_SERVERS;
    }
    
    int tok_idx = 1;
    for (int i = 0; i < array_size && client->server_count < MAX_SERVERS; i++) {
        if (tok_idx >= r || tokens[tok_idx].type != JSMN_OBJECT) {
            break;
        }
        
        DiscordServer* server = &client->servers[client->server_count];
        
        jsmntok_t* id_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "id");
        jsmntok_t* name_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "name");
        
        if (id_token && name_token) {
            json_get_string(response, id_token, server->id, sizeof(server->id));
            json_get_string(response, name_token, server->name, sizeof(server->name));
            client->server_count++;
        }
        
        tok_idx += tokens[tok_idx].size + 1;
    }
    
    free(response);
    return true;
}

bool discord_fetch_users(DiscordClient* client) {
    if (!client->connected || strlen(client->current_server_id) == 0) {
        return false;
    }
    
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "/guilds/%s/members?limit=50", client->current_server_id);
    
    char* response = discord_api_get(client, endpoint);
    if (!response) {
        printf("Failed to fetch users\n");
        return false;
    }
    
    jsmntok_t tokens[1024];
    int r = json_parse(response, tokens, 1024);
    
    if (r < 0 || tokens[0].type != JSMN_ARRAY) {
        printf("Failed to parse users JSON\n");
        free(response);
        return false;
    }
    
    client->user_count = 0;
    int array_size = tokens[0].size;
    if (array_size > MAX_USERS) {
        array_size = MAX_USERS;
    }
    
    int tok_idx = 1;
    for (int i = 0; i < array_size && client->user_count < MAX_USERS; i++) {
        if (tok_idx >= r || tokens[tok_idx].type != JSMN_OBJECT) {
            break;
        }
        
        DiscordUser* user = &client->users[client->user_count];
        
        // Get user object within member
        jsmntok_t* user_token = json_find_token(response, &tokens[tok_idx], r - tok_idx, "user");
        if (user_token) {
            int user_idx = user_token - tokens;
            
            jsmntok_t* id_token = json_find_token(response, &tokens[user_idx], r - user_idx, "id");
            jsmntok_t* username_token = json_find_token(response, &tokens[user_idx], r - user_idx, "username");
            jsmntok_t* discriminator_token = json_find_token(response, &tokens[user_idx], r - user_idx, "discriminator");
            
            if (id_token && username_token) {
                json_get_string(response, id_token, user->id, sizeof(user->id));
                json_get_string(response, username_token, user->username, sizeof(user->username));
                
                if (discriminator_token) {
                    json_get_string(response, discriminator_token, user->discriminator, sizeof(user->discriminator));
                } else {
                    strcpy(user->discriminator, "0");
                }
                
                // For now, assume all users are online (presence requires WebSocket)
                user->online = true;
                
                client->user_count++;
            }
        }
        
        tok_idx += tokens[tok_idx].size + 1;
    }
    
    free(response);
    return true;
}

bool discord_send_message(DiscordClient* client, const char* message) {
    if (!client->connected || !message || strlen(message) == 0 || strlen(client->current_channel_id) == 0) {
        return false;
    }
    
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "/channels/%s/messages", client->current_channel_id);
    
    // Create JSON payload
    char json_data[MAX_TEXT_LENGTH + 64];
    snprintf(json_data, sizeof(json_data), "{\"content\":\"%s\"}", message);
    
    char* response = discord_api_post(client, endpoint, json_data);
    if (!response) {
        printf("Failed to send message\n");
        return false;
    }
    
    free(response);
    
    // Refresh messages to show the new one
    discord_fetch_messages(client);
    
    return true;
}

bool discord_switch_server(DiscordClient* client, const char* server_id) {
    if (!client->connected || !server_id) {
        return false;
    }
    
    // Update current server ID
    strncpy(client->current_server_id, server_id, sizeof(client->current_server_id) - 1);
    client->current_server_id[sizeof(client->current_server_id) - 1] = '\0';
    
    // Clear current channel ID
    client->current_channel_id[0] = '\0';
    
    // Fetch channels for the server
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "/guilds/%s/channels", server_id);
    char* channels_response = discord_api_get(client, endpoint);
    
    if (!channels_response) {
        printf("Failed to fetch channels for server\n");
        return false;
    }
    
    jsmntok_t channel_tokens[256];
    int ch_count = json_parse(channels_response, channel_tokens, 256);
    
    if (ch_count > 0 && channel_tokens[0].type == JSMN_ARRAY && channel_tokens[0].size > 0) {
        // Find first text channel
        for (int i = 1; i < ch_count; i++) {
            jsmntok_t* type_token = json_find_token(channels_response, &channel_tokens[i], ch_count - i, "type");
            if (type_token) {
                char type_str[16];
                json_get_string(channels_response, type_token, type_str, sizeof(type_str));
                // Type 0 is text channel
                if (strcmp(type_str, "0") == 0) {
                    jsmntok_t* ch_id_token = json_find_token(channels_response, &channel_tokens[i], ch_count - i, "id");
                    if (ch_id_token) {
                        json_get_string(channels_response, ch_id_token, client->current_channel_id, sizeof(client->current_channel_id));
                        break;
                    }
                }
            }
        }
    }
    
    free(channels_response);
    
    // Clear message count since we're switching to a different server
    client->message_count = 0;
    client->user_count = 0;
    
    return strlen(client->current_channel_id) > 0;
}

void discord_cleanup(DiscordClient* client) {
    client->connected = false;
    memset(client->token, 0, sizeof(client->token));
    
    // Cleanup curl
    curl_global_cleanup();
}
