# Contributing to Discord 3DS Homebrew

Thank you for your interest in contributing! This document provides guidelines and information for contributors.

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Help others learn and grow
- Follow the Nintendo 3DS homebrew community guidelines

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported in [Issues](https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew/issues)
2. If not, create a new issue with:
   - Clear title describing the bug
   - Steps to reproduce
   - Expected behavior
   - Actual behavior
   - 3DS model and firmware version
   - Any error messages or screenshots

### Suggesting Features

1. Check existing issues and pull requests
2. Create a new issue with:
   - Clear description of the feature
   - Use cases and benefits
   - Potential implementation approach
   - Any mockups or examples

### Submitting Code

1. **Fork the repository**
   ```bash
   # Click "Fork" on GitHub, then clone your fork
   git clone https://github.com/YOUR_USERNAME/discord-3ds-homebrew.git
   cd discord-3ds-homebrew
   ```

2. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make your changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Test your changes on actual 3DS hardware if possible

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add feature: description of your changes"
   ```

5. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Describe your changes clearly

## Development Setup

### Prerequisites

- **devkitPro**: Install from [devkitpro.org](https://devkitpro.org/wiki/Getting_Started)
- **devkitARM**: Comes with devkitPro
- **Git**: For version control
- **Text editor**: VS Code, Vim, Emacs, etc.

### Environment Setup

```bash
# On Linux/macOS
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=/opt/devkitpro/devkitARM
export PATH=$DEVKITARM/bin:$PATH

# On Windows (use PowerShell or Git Bash)
$env:DEVKITPRO = "C:/devkitPro"
$env:DEVKITARM = "C:/devkitPro/devkitARM"
```

### Building

```bash
# Clone the repository
git clone https://github.com/BubblePlayzTHEREAL/discord-3ds-homebrew.git
cd discord-3ds-homebrew

# Build
make

# Clean build
make clean
make
```

### Testing

- Test on actual 3DS hardware with custom firmware
- Use Citra emulator for initial testing (may have limitations)
- Test both top and bottom screen rendering
- Test all control inputs
- Test with different token configurations

## Code Style

### C Code Style

```c
// Use descriptive variable names
int message_count = 0;  // Good
int mc = 0;             // Bad

// Use clear function names
void discord_fetch_messages(DiscordClient* client);  // Good
void get_msgs(void* c);                              // Bad

// Add comments for complex logic
// Calculate the scroll position based on message count
int scroll_pos = (state->message_scroll * SCREEN_HEIGHT) / total_messages;

// Use consistent indentation (4 spaces or tabs)
if (condition) {
    do_something();
} else {
    do_something_else();
}

// Check for NULL pointers
if (client != NULL && client->connected) {
    // Safe to use client
}
```

### Header Files

```c
// Use include guards
#ifndef MY_HEADER_H
#define MY_HEADER_H

// Include necessary headers
#include <3ds.h>

// Declare functions
void my_function(void);

#endif // MY_HEADER_H
```

### Memory Management

```c
// Always initialize variables
char buffer[256] = {0};
int count = 0;

// Check malloc return values
void* ptr = malloc(size);
if (ptr == NULL) {
    // Handle error
    return false;
}

// Free allocated memory
free(ptr);
ptr = NULL;
```

## Project Structure

```
discord-3ds-homebrew/
├── source/              # C source files
│   ├── main.c          # Entry point
│   ├── discord_api.c   # Discord API implementation
│   └── ui.c            # UI rendering
├── include/            # Header files
│   ├── discord_api.h   # API declarations
│   └── ui.h            # UI declarations
├── Makefile            # Build configuration
├── icon.png            # App icon (48x48 PNG)
├── .github/
│   └── workflows/      # CI/CD workflows
└── README.md           # Documentation
```

## Areas for Contribution

### High Priority

1. **Real Discord API Integration**
   - Implement HTTP requests with libcurl
   - Add JSON parsing (jsmn library)
   - Handle API authentication
   - Error handling for network issues

2. **WebSocket Support**
   - Real-time message updates
   - Presence updates
   - Typing indicators

3. **Touchscreen Keyboard**
   - On-screen keyboard for text input
   - Better than D-Pad input method

### Medium Priority

4. **UI Improvements**
   - Graphics mode instead of console
   - Better message formatting
   - Rich embed support
   - Image thumbnails

5. **Additional Features**
   - Channel browsing
   - Direct messages
   - User profiles
   - Settings menu

### Low Priority (Nice to Have)

6. **Polish**
   - Custom themes
   - Sound notifications
   - Better icon design
   - Animations

## Testing Checklist

Before submitting a pull request, verify:

- [ ] Code compiles without errors or warnings
- [ ] Tested on real 3DS hardware (if possible)
- [ ] No memory leaks (check with valgrind if possible)
- [ ] All controls work as expected
- [ ] UI renders correctly on both screens
- [ ] Token loading works properly
- [ ] Documentation updated if needed
- [ ] Follows existing code style

## Additional Resources

### 3DS Development
- [3DBrew Wiki](https://www.3dbrew.org/)
- [devkitPro Documentation](https://devkitpro.org/wiki/Main_Page)
- [libctru Documentation](https://libctru.devkitpro.org/)
- [3DS Homebrew Development Discord](https://discord.gg/C29hYvh)

### Discord API
- [Discord Developer Portal](https://discord.com/developers/docs)
- [Discord API Reference](https://discord.com/developers/docs/reference)
- [Discord.js Guide](https://discordjs.guide/) (useful for understanding concepts)

### Tools
- [Citra Emulator](https://citra-emu.org/) - 3DS emulator for testing
- [3DS Link](https://github.com/astronautlevel2/3dslink) - Send homebrew over network
- [GodMode9](https://github.com/d0k3/GodMode9) - File manager for 3DS

## Questions?

If you have questions about contributing:
- Open a discussion on GitHub
- Check existing issues and PRs
- Join the 3DS Homebrew community Discord

Thank you for contributing to Discord 3DS Homebrew! 🎮
