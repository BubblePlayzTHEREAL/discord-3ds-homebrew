#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <3ds.h>

static PrintConsole topScreen, bottomScreen;

void ui_init(void) {
    // Initialize console on both screens
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
}

void ui_render_top_screen(DiscordClient* client, UIState* state) {
    consoleSelect(&topScreen);
    consoleClear();
    
    printf("\x1b[0;0H"); // Move cursor to top-left
    
    // Header
    printf("\x1b[1;37m=== Discord Chat Messages ===\x1b[0m\n");
    printf("Server: \x1b[32m%s\x1b[0m\n", 
           client->server_count > 0 ? client->servers[state->selected_server].name : "None");
    printf("\x1b[34m--------------------------------\x1b[0m\n");
    
    if (!client->connected) {
        printf("\n\x1b[31mNot connected to Discord!\x1b[0m\n");
        return;
    }
    
    // Display messages
    int start_msg = state->message_scroll;
    int max_msgs = 20; // Max messages to display on screen
    
    if (client->message_count == 0) {
        printf("\n\x1b[33mNo messages to display.\x1b[0m\n");
        printf("Try refreshing or check channel.\n");
    } else {
        for (int i = start_msg; i < client->message_count && i < start_msg + max_msgs; i++) {
            DiscordMessage* msg = &client->messages[i];
            
            // Display message with formatting
            printf("\x1b[36m[%s]\x1b[0m ", msg->timestamp);
            printf("\x1b[35m%s:\x1b[0m\n", msg->author);
            printf("  %s\n", msg->content);
        }
    }
    
    // Footer
    printf("\n\x1b[34m--------------------------------\x1b[0m\n");
    printf("\x1b[33mDPAD-UP/DOWN:\x1b[0m Scroll | \x1b[33mY:\x1b[0m Refresh\n");
}

void ui_render_bottom_screen(DiscordClient* client, UIState* state) {
    consoleSelect(&bottomScreen);
    consoleClear();
    
    printf("\x1b[0;0H"); // Move cursor to top-left
    
    if (!client->connected) {
        printf("\x1b[31mNot connected!\x1b[0m\n");
        printf("\nPress START to exit.\n");
        return;
    }
    
    // Servers section
    printf("\x1b[1;37m=== Servers ===\x1b[0m\n");
    for (int i = 0; i < client->server_count && i < 5; i++) {
        if (i == state->selected_server) {
            printf("\x1b[42;30m> %s\x1b[0m\n", client->servers[i].name);
        } else {
            printf("  %s\n", client->servers[i].name);
        }
    }
    
    printf("\n\x1b[1;37m=== Users Online ===\x1b[0m\n");
    
    // Fetch users if not loaded
    if (client->user_count == 0) {
        discord_fetch_users(client);
    }
    
    // Display users
    int online_count = 0;
    for (int i = 0; i < client->user_count && i < 5; i++) {
        DiscordUser* user = &client->users[i];
        if (user->online) {
            printf("\x1b[32m● \x1b[0m%s#%s\n", user->username, user->discriminator);
            online_count++;
        }
    }
    
    if (online_count == 0) {
        printf("\x1b[33mNo users online.\x1b[0m\n");
    }
    
    // Chat input section
    printf("\n\x1b[1;37m=== Chat Input ===\x1b[0m\n");
    printf("\x1b[47;30m[Press X for keyboard]\x1b[0m\n");
    printf("\n");
    
    // Controls
    printf("\n\x1b[34m-------------------\x1b[0m\n");
    printf("\x1b[33mL/R:\x1b[0m Change server\n");
    printf("\x1b[33mX:\x1b[0m Open keyboard\n");
    printf("\x1b[33mY:\x1b[0m Refresh messages\n");
    printf("\x1b[33mDPAD:\x1b[0m Scroll messages\n");
    printf("\x1b[33mSTART:\x1b[0m Exit\n");
}

void ui_handle_input(DiscordClient* client, UIState* state, u32 kDown, u32 kHeld) {
    // Normal mode controls
    if (kDown & KEY_X) {
        // Open touchscreen keyboard for text input
        SwkbdState swkbd;
        char text_buffer[MAX_TEXT_LENGTH];
        SwkbdButton button = SWKBD_BUTTON_NONE;
        
        swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
        swkbdSetHintText(&swkbd, "Enter message...");
        swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
        
        button = swkbdInputText(&swkbd, text_buffer, sizeof(text_buffer));
        
        if (button == SWKBD_BUTTON_CONFIRM && strlen(text_buffer) > 0) {
            // Send the message
            discord_send_message(client, text_buffer);
        }
        // If SWKBD_BUTTON_LEFT (cancel) or empty, do nothing
    } else if (kDown & KEY_Y) {
        // Refresh messages
        discord_fetch_messages(client);
    } else if (kDown & KEY_L) {
        // Previous server
        if (state->selected_server > 0) {
            state->selected_server--;
            strcpy(client->current_server_id, client->servers[state->selected_server].id);
            discord_fetch_messages(client);
            discord_fetch_users(client);
        }
    } else if (kDown & KEY_R) {
        // Next server
        if (state->selected_server < client->server_count - 1) {
            state->selected_server++;
            strcpy(client->current_server_id, client->servers[state->selected_server].id);
            discord_fetch_messages(client);
            discord_fetch_users(client);
        }
    } else if (kDown & KEY_DUP) {
        // Scroll messages up
        if (state->message_scroll > 0) {
            state->message_scroll--;
        }
    } else if (kDown & KEY_DDOWN) {
        // Scroll messages down
        if (state->message_scroll < client->message_count - 1) {
            state->message_scroll++;
        }
    }
}

void ui_cleanup(void) {
    // Nothing specific to cleanup for console-based UI
}
