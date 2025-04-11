# ImGui Android Project

A C++ project using ImGui for Android applications with support for Arabic text rendering and touch input.

<img src="https://github.com/user-attachments/assets/ce079abe-99ef-49a7-bcad-473e08e7551e" width="200" alt="ImGui Android Demo">

## Overview

This project provides a framework for developing Android applications using the ImGui library, with special emphasis on Arabic text rendering and touch input handling. It enables developers to create visually appealing cross-platform applications with a consistent interface.

## Requirements

- Android Studio with Android SDK installed
- Android NDK (Native Development Kit)
- ADB (Android Debug Bridge) tools
- A rooted Android device for running the application
- Visual Studio or any C++ IDE for development

## Current Limitations

- **Emulator Support**: Currently, the project does not support Android emulators. This is planned for future development.
- **Android Version Support**: The project currently supports Android versions up to 13. Support for Android 14 and 15 is planned for future updates.

## Project Structure

```
├── jni/
│   ├── include/          # Header files
│   │   ├── main.h        # Main header file
│   │   ├── utils/        # Utility functions
│   │   ├── imgui/        # ImGui implementation
│   │   ├── graphics/     # Graphics related code
│   │   └── aosp_android/ # Android specific code
│   └── src/              # Source files
│       ├── main.cpp      # Main application code
│       ├── utils/        # Utility implementations
│       ├── imgui/        # ImGui source
│       ├── graphics/     # Graphics implementations
│       └── aosp_android/ # Android specific implementations
└── build.bat             # Build and deployment script
```

## Building the Project

1. **Setup Environment**
   - Install Android Studio and Android SDK
   - Install Android NDK
   - Ensure ADB is in your system PATH
   - Connect your rooted Android device via USB

2. **Build Process**
   - Open the project in your preferred C++ IDE
   - Build the project using the provided build.bat script
   - The script will:
     - Compile the native code
     - Push the compiled library to the device
     - Set appropriate permissions
     - Launch the application

## Running on Device

1. **Prerequisites**
   - Your Android device must be rooted
   - USB debugging must be enabled
   - Device must be connected via USB

2. **Using build.bat**
   - Open build.bat in a text editor
   - Modify the following paths according to your setup:
     - Path to your compiled library
     - Target package name (if different from default)
   - Remove comments (REM) from the deployment section
   - Run build.bat as administrator

## Features

- ImGui integration for Android
- Arabic text support
- Touch input handling
- Frame rate control
- Dynamic orientation handling
- Custom graphics rendering

## Future Goals

1. **Emulator Support**
   - Implementation of emulator compatibility
   - Testing and optimization for various emulator configurations

2. **Android Version Support**
   - Extension of support to Android 14 and 15
   - Adaptation to new Android APIs and features

3. **Community Contributions**
   - We welcome community contributions through pull requests
   - Contributors can help by:
     - Implementing emulator support
     - Adding support for newer Android versions
     - Optimizing performance
     - Adding new features
     - Improving documentation

## Configuration

Key configuration parameters can be found in `jni/include/main.h`:
- `control_desired_fps`: Controls the application's frame rate
- Display dimensions and orientation settings

## Troubleshooting

1. **Build Issues**
   - Ensure all dependencies are properly installed
   - Check NDK version compatibility
   - Verify include paths in your IDE

2. **Runtime Issues**
   - Verify device is rooted
   - Check USB debugging is enabled
   - Ensure proper permissions are set on the device

## Support

For issues and feature requests, please open an issue in the project repository. We encourage community participation and welcome pull requests for:
- Emulator support implementation
- Android 14/15 compatibility
- Performance optimizations
- New feature additions
- Documentation improvements

## Acknowledgments

This project would not have been possible without the excellent work of the following open-source projects and their dedicated developers:

### [TouchSimulation](https://github.com/kp7742/TouchSimulation)
- Developed by [kp7742](https://github.com/kp7742)
- Provided essential touch simulation implementation in Golang and C++ for Android devices
- Offered valuable insights into Android input handling mechanisms

### [Android_Native_Surface](https://github.com/SsageParuders/Android_Native_Surface)
- Developed by [SsageParuders](https://github.com/SsageParuders)
- Provided critical native surface implementation techniques for Android
- Demonstrated effective approaches for low-level graphics rendering

We express our sincere gratitude to these developers and their communities for their exceptional contributions to open-source software, which have significantly influenced and enhanced this project.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
