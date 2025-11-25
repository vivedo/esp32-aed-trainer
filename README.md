# ESP32 AED Trainer

> ⚠️ **EXPERIMENTAL BUILD** ⚠️  
> This is a highly experimental project exploring the possibilities of ESP32 audio playback and interaction methods.  
> Features and functionality are subject to change.

## Overview

An ESP32-based **Automated External Defibrillator (AED) Trainer** designed for First Responder training courses. This project demonstrates real-time audio playback capabilities of the ESP32 platform while providing an interactive training tool for emergency response scenarios.

## Hardware Requirements

- ESP32
- SD card module (for audio file storage)
- Audio amplifier and speaker (I2S compatible)

> tested on Ai-Thinker ESP32 Audio Kit - rev A436

## Software Dependencies

This project is built using PlatformIO and requires:

- **Platform**: ESP32 (Arduino framework)
- **Libraries**:
  - SdFat v2.3.1+
  - arduino-libhelix v0.9.2
  - arduino-audio-tools v1.2.1
  - arduino-audio-driver v0.1.4

## Installation

**Build and upload**:
   ```bash
   pio run --target upload
   ```

**Monitor serial output**:
   ```bash
   pio device monitor
   ```

## Configuration

- Serial monitor baud rate: **115200**
- Custom partition scheme defined in `partitions.csv`

## Development Status

This is an active experimental project. Current areas of exploration:

- ESP32 audio playback
- User interaction methods (web, buttons)
- Multi-scenario training sequences

## Contributing

This is an experimental project. Contributions, suggestions, and feedback are welcome!

## License

See the [LICENSE](LICENSE) file for details.

## Disclaimer

This is a **training device only** and is not intended for use in real medical emergencies. This device does not provide actual defibrillation capabilities and should only be used for educational purposes in controlled training environments.

## Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- Audio libraries by [pschatzmann](https://github.com/pschatzmann)
