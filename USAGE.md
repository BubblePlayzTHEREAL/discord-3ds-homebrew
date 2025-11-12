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
[Press X for keyboard]

-------------------
L/R: Change server
X: Open keyboard
Y: Refresh messages
DPAD: Scroll messages
START: Exit
```

The bottom screen shows:
- Server list (selected server highlighted)
- Online users in current server
- Chat input prompt
- Control reference

## Controls Reference

| Button | Action |
|--------|--------|
| D-Pad Up | Scroll messages up |
| D-Pad Down | Scroll messages down |
| L Button | Previous server |
| R Button | Next server |
| Y Button | Refresh messages |
| X Button | Open touchscreen keyboard |
| START | Exit app |

When you press **X**, the native 3DS touchscreen keyboard appears. Type your message using the touchscreen, then press the **OK** button to send it or **Cancel** to discard it.

## Tips and Tricks

### Efficient Navigation
- Use **L/R** to quickly switch between servers
- Use **D-Pad Up/Down** to scroll through long conversations
- Press **Y** to refresh messages and see new ones

### Typing Messages
- Press **X** to open the touchscreen keyboard
- Type your message using the stylus or finger
- Press **OK** on the keyboard to send
- Press **Cancel** on the keyboard to discard
- The keyboard supports full text entry with special characters

### Troubleshooting
- If messages don't load, press **Y** to refresh
- If connection fails, check your token in `/3ds/discord_token.txt`
- Ensure your 3DS has internet access and is connected to WiFi
- Check that your Discord token is valid and hasn't expired

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
- Check your internet connection
- Verify your token is valid
- Make sure you have access to the channels

### Connection errors
**Solution**: 
- Verify your 3DS is connected to WiFi
- Check that your Discord token is correct
- Ensure Discord's API is accessible from your network
- Some networks may block Discord - try a different connection

## Advanced Usage

### Real Discord API
The app now uses the **real Discord API** for all operations:
- **Server List**: Fetched from Discord's `/users/@me/guilds` endpoint
- **Messages**: Retrieved from Discord's `/channels/{id}/messages` endpoint
- **Sending**: Messages are posted to Discord via `/channels/{id}/messages`
- **Users**: Member list from `/guilds/{id}/members` endpoint

All communication uses HTTPS with SSL/TLS encryption for security.

### For Developers
The app is built with:

1. **libcurl**: For HTTPS requests to Discord API
   ```makefile
   LIBS := -lcurl -lmbedtls -lmbedx509 -lmbedcrypto -lcitro2d -lcitro3d -lctru -lm
   ```

2. **jsmn**: Lightweight JSON parser for API responses

3. **Network Stack**: 3DS network initialization with socInit()

4. **API Endpoints Implemented**:
   - `GET /users/@me` - Verify authentication
   - `GET /users/@me/guilds` - Fetch servers
   - `GET /guilds/{id}/channels` - Fetch channels
   - `GET /channels/{id}/messages` - Fetch messages
   - `POST /channels/{id}/messages` - Send messages
   - `GET /guilds/{id}/members` - Fetch users

Future improvements could include WebSocket support for real-time updates.

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

Already implemented:
- [x] Real Discord API integration
- [x] Touchscreen keyboard
- [x] Message sending/receiving
- [x] Server and user browsing

Planned improvements:
- [ ] WebSocket for live updates (auto-refresh)
- [ ] Image/attachment support
- [ ] Voice channel indicators
- [ ] Rich embed rendering
- [ ] Notifications
- [ ] Custom themes
- [ ] Direct messages
- [ ] Better error handling

Stay tuned for updates!
