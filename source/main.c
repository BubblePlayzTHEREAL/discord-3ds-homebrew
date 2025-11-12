#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <3ds.h>

#include "discord_api.h"
#include "ui.h"

#define TOKEN_FILE "sdmc:/3ds/discord_token.txt"

// Read Discord token from file
bool read_token(char* token, size_t max_len) {
    FILE* f = fopen(TOKEN_FILE, "r");
    if (!f) {
        return false;
    }
    
    if (fgets(token, max_len, f) == NULL) {
        fclose(f);
        return false;
    }
    
    // Remove newline if present
    size_t len = strlen(token);
    if (len > 0 && token[len-1] == '\n') {
        token[len-1] = '\0';
    }
    
    fclose(f);
    return true;
}

int main(int argc, char* argv[]) {
    // Initialize services
    gfxInitDefault();
    
    // Initialize network
    Result ret = 0;
    u32* soc_buffer = (u32*)memalign(0x1000, 0x100000);
    if (!soc_buffer) {
        printf("Failed to allocate SOC buffer\n");
        gfxExit();
        return 1;
    }
    
    ret = socInit(soc_buffer, 0x100000);
    if (R_FAILED(ret)) {
        printf("socInit failed: 0x%08lX\n", ret);
        free(soc_buffer);
        gfxExit();
        return 1;
    }
    
    consoleInit(GFX_TOP, NULL);
    consoleInit(GFX_BOTTOM, NULL);
    
    // Initialize UI
    ui_init();
    
    // Allocate DiscordClient on heap to avoid stack overflow
    DiscordClient* client = (DiscordClient*)malloc(sizeof(DiscordClient));
    if (!client) {
        printf("Failed to allocate DiscordClient\n");
        ui_cleanup();
        socExit();
        free(soc_buffer);
        gfxExit();
        return 1;
    }
    
    UIState ui_state = {0};
    char token[128] = {0};
    
    printf("Discord 3DS Client\n");
    printf("==================\n\n");
    
    // Try to read token from file
    if (!read_token(token, sizeof(token))) {
        printf("Token file not found!\n");
        printf("Please create:\n");
        printf("%s\n", TOKEN_FILE);
        printf("And paste your Discord token in it.\n\n");
        printf("Press START to exit.\n");
        
        while (aptMainLoop()) {
            hidScanInput();
            u32 kDown = hidKeysDown();
            
            if (kDown & KEY_START) {
                break;
            }
            
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
        }
        
        free(client);
        ui_cleanup();
        socExit();
        free(soc_buffer);
        gfxExit();
        return 0;
    }
    
    printf("Token loaded!\n");
    printf("Initializing Discord client...\n");
    
    discord_init(client, token);
    
    printf("Connecting to Discord...\n");
    if (!discord_connect(client)) {
        printf("Failed to connect to Discord!\n");
        printf("Check your token and internet.\n");
        printf("\nPress START to exit.\n");
        
        while (aptMainLoop()) {
            hidScanInput();
            u32 kDown = hidKeysDown();
            
            if (kDown & KEY_START) {
                break;
            }
            
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
        }
        
        discord_cleanup(client);
        free(client);
        ui_cleanup();
        socExit();
        free(soc_buffer);
        gfxExit();
        return 0;
    }
    
    printf("Connected! Loading data...\n");
    
    // Initial data fetch
    discord_fetch_servers(client);
    discord_fetch_messages(client);
    
    // Main loop
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        
        if (kDown & KEY_START) {
            break;
        }
        
        // Handle input
        ui_handle_input(client, &ui_state, kDown, kHeld);
        
        // Render UI
        ui_render_top_screen(client, &ui_state);
        ui_render_bottom_screen(client, &ui_state);
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    
    // Cleanup
    discord_cleanup(client);
    free(client);
    ui_cleanup();
    socExit();
    free(soc_buffer);
    gfxExit();
    
    return 0;
}
