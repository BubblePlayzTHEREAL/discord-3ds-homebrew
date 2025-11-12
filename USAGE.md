# Usage Guide

## First Time Setup

1. **Copy the app to your SD card**
   - Place `discord-3ds.3dsx` in `/3ds/` folder on your SD card

2. **Create token file**
   - Create a file at `/3ds/discord_token.txt` on your SD card
   - Paste your Discord token (see main README for how to get it)
   - Save the file

3. **Insert SD card and launch**
   - Put the SD card back in your 3DS
   - Open Homebrew Launcher
   - Find and launch "Discord 3DS"

## Understanding the Interface

### Top Screen (Messages View)
```
=== Discord Chat Messages ===
Server: General Server
--------------------------------
[12:00 PM] User1:
  Hello from 3DS!
[12:01 PM] User2:
  This is awesome!
--------------------------------
DPAD-UP/DOWN: Scroll | Y: Refresh
```

The top screen shows:
- Current server name
- Chat messages with timestamps
- Author names
- Message content
- Control hints at bottom

### Bottom Screen (Control Panel)
```
=== Servers ===
> General Server
  Gaming
  3DS Homebrew

=== Users Online ===
● User1#0001
● User2#0002
● BubblePlayz#1234

=== Chat Input ===
[Press X to type]
> 

-------------------
L/R: Change server
X: Type message
A: Send | B: Cancel
START: Exit
```

The bottom screen shows:
- Server list (selected server highlighted)
- Online users in current server
- Chat input area
- Control reference

## Controls Reference

### Navigation Mode (Default)
| Button | Action |
|--------|--------|
| D-Pad Up | Scroll messages up |
| D-Pad Down | Scroll messages down |
| L Button | Previous server |
| R Button | Next server |
| Y Button | Refresh messages |
| X Button | Enter typing mode |
| START | Exit app |

### Typing Mode
| Button | Action |
|--------|--------|
| D-Pad Up | Add "Hello! " |
| D-Pad Right | Add letter "a" |
| D-Pad Left | Add space |
| D-Pad Down | Add "!" |
| X Button | Backspace |
| A Button | Send message |
| B Button | Cancel (exit typing mode) |

*Note: Typing mode is simplified. A touchscreen keyboard is planned for future updates.*

## Tips and Tricks

### Efficient Navigation
- Use **L/R** to quickly switch between servers
- Use **D-Pad Up/Down** to scroll through long conversations
- Press **Y** to refresh messages and see new ones

### Typing Messages
- Enter typing mode with **X**
- Use **D-Pad Up** to quickly type "Hello! "
- Use **X** to backspace if you make a mistake
- Press **A** when ready to send
- Press **B** to cancel without sending

### Troubleshooting
- If messages don't load, press **Y** to refresh
- If connection fails, check your token in `/3ds/discord_token.txt`
- Ensure your 3DS has internet access (current version uses mock data)

## Common Issues

### "Token file not found!"
**Solution**: Create `/3ds/discord_token.txt` on your SD card with your token

### App crashes on launch
**Solution**: 
- Ensure you have custom firmware installed
- Check that Homebrew Launcher is up to date
- Verify the `.3dsx` file isn't corrupted

### No messages showing
**Solution**: 
- Press Y to refresh
- Current version uses mock data for demonstration
- Full API integration coming in future update

### Can't type properly
**Solution**: 
- Use the simplified D-Pad controls for now
- Full touchscreen keyboard support is planned

## Advanced Usage

### Mock Data vs Real API
The current version uses **mock data** to demonstrate the UI and controls. This means:
- Server and user lists are pre-populated
- Messages are examples, not real Discord messages
- Sending messages adds them locally but doesn't actually send to Discord

A future update will integrate the real Discord API with:
- Actual message fetching from Discord
- Real-time updates via WebSocket
- Proper message sending
- Full server and channel browsing

### For Developers
If you're building from source and want to add real API support:

1. Add libcurl support in Makefile:
   ```makefile
   LIBS := -lcurl -lcitro2d -lcitro3d -lctru -lm
   ```

2. Implement HTTP requests in `discord_api.c`:
   - GET `/users/@me/guilds` for servers
   - GET `/channels/{id}/messages` for messages
   - POST `/channels/{id}/messages` for sending

3. Add JSON parsing with jsmn or similar library

4. Consider WebSocket for real-time updates

## Safety and Privacy

⚠️ **Important Security Notes**:
- Your token is stored in plain text on the SD card
- Anyone with access to your SD card can read it
- Using third-party Discord clients violates Discord ToS
- Your account could be banned for using this

**Recommendations**:
- Use a secondary Discord account for testing
- Never share your token with anyone
- Delete the token file when not using the app
- Keep your SD card secure

## Support

For issues or questions:
- Check the main README.md
- Open an issue on GitHub
- Check existing issues for solutions

## Future Features

Planned improvements:
- [ ] Real Discord API integration
- [ ] WebSocket for live updates
- [ ] Touchscreen keyboard
- [ ] Image/attachment support
- [ ] Voice channel indicators
- [ ] Rich embed rendering
- [ ] Notifications
- [ ] Custom themes

Stay tuned for updates!
