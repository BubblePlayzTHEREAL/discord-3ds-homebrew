#ifndef UI_H
#define UI_H

#include <3ds.h>
#include "discord_api.h"

// UI state
typedef struct {
    int selected_server;
    int selected_user;
    int message_scroll;
} UIState;

// Initialize UI
void ui_init(void);

// Render top screen (messages)
void ui_render_top_screen(DiscordClient* client, UIState* state);

// Render bottom screen (servers, users, chat input)
void ui_render_bottom_screen(DiscordClient* client, UIState* state);

// Handle input
void ui_handle_input(DiscordClient* client, UIState* state, u32 kDown, u32 kHeld);

// Cleanup UI
void ui_cleanup(void);

#endif // UI_H
