# PlatformIO Project Structure

This project has been converted to PlatformIO format for better dependency management and cross-platform development.

## Directory Structure

```
├── src/                 # Source code files
│   └── main.cpp        # Main application code (formerly IKEA_ESP32.ino)
├── include/            # Header files
├── lib/                # Private libraries
├── test/               # Unit tests
├── platformio.ini      # PlatformIO configuration
├── images/             # Documentation images
├── SmartphoneApp/      # Android app files
└── README.md          # Project documentation
```

## Building and Uploading

1. Install [PlatformIO](https://platformio.org/install)
2. Open this folder in VS Code with PlatformIO extension
3. Build: `pio run`
4. Upload: `pio run --target upload`
5. Monitor: `pio device monitor`

## VS Code Commands

- Build: Ctrl+Alt+B
- Upload: Ctrl+Alt+U
- Serial Monitor: Ctrl+Alt+S

The original Arduino IDE workflow still works with the `IKEA_ESP32.ino` file.