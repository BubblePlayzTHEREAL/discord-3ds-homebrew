# Discord 3DS Homebrew

A Discord client for Nintendo 3DS that allows you to chat with friends directly from your handheld console!

![Build Status](https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew/workflows/Build%203DSX/badge.svg)

## Features

- 🎮 **Top Screen**: View chat messages with timestamps and authors
- 📱 **Bottom Screen**: Server list, online users, and chat input
- 🔐 **Token Authentication**: Secure login using your Discord token
- 🏗️ **Auto-Build**: GitHub Actions automatically builds the `.3dsx` file
- 🎨 **User-Friendly UI**: Color-coded interface with intuitive controls

## Screenshots

### Top Screen (Chat Messages)
The top screen displays chat messages with:
- Server name at the top
- Message timestamps
- Author names
- Message content
- Scroll functionality

### Bottom Screen (Servers & Input)
The bottom screen shows:
- List of your Discord servers
- Online users in the current server
- Chat input box
- Control hints

## Installation

### Prerequisites
- Nintendo 3DS or 2DS with custom firmware (CFW)
- Homebrew Launcher installed
- SD card with at least 10MB free space

### Steps

1. **Download the Latest Release**
   - Go to the [Releases](https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew/releases) page
   - Download `discord-3ds.3dsx`

2. **Install on SD Card**
   ```bash
   # Copy to your SD card's 3ds folder
   /3ds/discord-3ds.3dsx
   ```

3. **Set Up Your Discord Token**
   - Create a file at `/3ds/discord_token.txt` on your SD card
   - Paste your Discord token into this file
   - Save and safely eject your SD card

4. **Launch the App**
   - Insert SD card into your 3DS
   - Open Homebrew Launcher
   - Select "Discord 3DS"

## Getting Your Discord Token

⚠️ **WARNING**: Your Discord token is like your password. Keep it secret and never share it!

### Desktop/Browser Method:
1. Open Discord in your web browser
2. Press `F12` to open Developer Tools
3. Go to the "Console" tab
4. Type this and press Enter:
   ```javascript
   (webpackChunkdiscord_app.push([[''],{},e=>{m=[];for(let c in e.c)m.push(e.c[c])}]),m).find(m=>m?.exports?.default?.getToken!==void 0).exports.default.getToken()
   ```
5. Copy the token (without quotes)
6. Paste it into `discord_token.txt`

⚠️ **Security Notice**: Using your Discord token in third-party apps is against Discord's Terms of Service. Use at your own risk.

## Controls

- **D-Pad Up/Down**: Scroll through messages
- **L/R Buttons**: Switch between servers
- **Y Button**: Refresh messages
- **X Button**: Open touchscreen keyboard to type and send messages
- **START**: Exit application

The app uses the native 3DS touchscreen keyboard for text input, making it easy to compose messages!

## Building from Source

### Prerequisites
- [devkitPro](https://devkitpro.org/wiki/Getting_Started) with devkitARM
- Git

### Build Steps

```bash
# Clone the repository
git clone https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew.git
cd discord-3ds-homebrew

# Build using Make
make

# The output files will be:
# - discord-3ds.3dsx (the homebrew app)
# - discord-3ds.smdh (metadata file)
```

### Clean Build

```bash
make clean
make
```

## Automatic Building

This project uses GitHub Actions to automatically build the `.3dsx` file on every commit to `main` or `copilot/add-discord-chat-app` branches.

### Workflow Features
- 🔄 Builds on push and pull requests
- 📦 Uploads build artifacts
- 🏷️ Creates releases for main branch commits
- 🐳 Uses official devkitPro Docker container

Build artifacts are available in the [Actions](https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew/actions) tab after each successful build.

## Project Structure

```
discord-3ds-homebrew/
├── source/              # Source code
│   ├── main.c          # Application entry point
│   ├── discord_api.c   # Discord API client
│   └── ui.c            # UI rendering and input
├── include/            # Header files
│   ├── discord_api.h   # Discord API definitions
│   └── ui.h            # UI definitions
├── Makefile            # Build configuration
├── icon.png            # Application icon
├── .github/
│   └── workflows/
│       └── build.yml   # GitHub Actions workflow
└── README.md           # This file
```

## Technical Details

### Architecture
- **Language**: C
- **Graphics**: libctru console API
- **Networking**: libcurl with mbedTLS for HTTPS
- **JSON Parsing**: jsmn (lightweight JSON parser)
- **Text Input**: Native SwkbdButton (touchscreen keyboard)
- **Build System**: Make with devkitARM

### Features
- ✅ **Real Discord API**: Full integration with Discord's REST API v10
- ✅ **Touchscreen Keyboard**: Native 3DS keyboard for message input
- ✅ **Message Reading**: Fetch and display messages from channels
- ✅ **Message Sending**: Send messages to channels via API
- ✅ **Server Browsing**: View and switch between your Discord servers
- ✅ **User List**: See members in your current server
- ✅ **Secure**: Uses HTTPS with SSL/TLS verification

### Current Limitations
- Manual message refresh (press Y to update)
- No real-time updates (WebSocket not yet implemented)
- Console-based UI (graphics-based UI planned)
- No image/attachment support yet
- No rich embed rendering

### Planned Features
- [ ] WebSocket support for real-time updates
- [ ] Auto-refresh messages
- [ ] Rich embed support
- [ ] Image/attachment viewing
- [ ] Voice channel indication
- [ ] Notifications
- [ ] Custom themes
- [ ] Direct messages support

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is for educational purposes. Please respect Discord's Terms of Service.

## Disclaimer

This is an unofficial Discord client and is not affiliated with Discord Inc. Use at your own risk. Using third-party Discord clients may violate Discord's Terms of Service and could result in account termination.

## Acknowledgments

- devkitPro team for the amazing 3DS development tools
- libctru contributors
- Nintendo 3DS homebrew community

## Support

If you encounter issues:
1. Check that your token is valid and in the correct location
2. Ensure your 3DS has internet access (if using real API)
3. Open an issue on GitHub with details

---

Made with ❤️ for the 3DS homebrew community