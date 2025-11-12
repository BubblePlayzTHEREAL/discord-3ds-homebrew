#ifndef DISCORD_API_H
#define DISCORD_API_H

#include <3ds.h>

#define MAX_MESSAGES 50
#define MAX_SERVERS 20
#define MAX_USERS 50
#define MAX_TEXT_LENGTH 256

typedef struct {
    char id[32];
    char content[MAX_TEXT_LENGTH];
    char author[64];
    char timestamp[32];
} DiscordMessage;

typedef struct {
    char id[32];
    char name[64];
    char icon[128];
} DiscordServer;

typedef struct {
    char id[32];
    char username[64];
    char discriminator[8];
    bool online;
} DiscordUser;

typedef struct {
    char token[128];
    char current_channel_id[32];
    char current_server_id[32];
    
    DiscordMessage messages[MAX_MESSAGES];
    int message_count;
    
    DiscordServer servers[MAX_SERVERS];
    int server_count;
    
    DiscordUser users[MAX_USERS];
    int user_count;
    
    bool connected;
} DiscordClient;

// Initialize Discord client
void discord_init(DiscordClient* client, const char* token);

// Connect to Discord
bool discord_connect(DiscordClient* client);

// Fetch messages from current channel
bool discord_fetch_messages(DiscordClient* client);

// Fetch servers (guilds)
bool discord_fetch_servers(DiscordClient* client);

// Fetch users in current server
bool discord_fetch_users(DiscordClient* client);

// Send a message
bool discord_send_message(DiscordClient* client, const char* message);

// Switch to a different server
bool discord_switch_server(DiscordClient* client, const char* server_id);

// Cleanup
void discord_cleanup(DiscordClient* client);

#endif // DISCORD_API_H
