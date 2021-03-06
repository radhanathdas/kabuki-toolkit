```
  ____      _         _       _____    _____   U  ___ u   ____     __  __   
U|  _"\ u  |"|    U  /"\  u  |_ " _|  |" ___|   \/"_ \/U |  _"\ uU|' \/ '|u
\| |_) |/U | | u   \/ _ \/     | |   U| |_  u   | | | | \| |_) |/\| |\/| |/
 |  __/   \| |/__  / ___ \    /| |\  \|  _|/.-,_| |_| |  |  _ <   | |  | |  
 |_|       |_____|/_/   \_\  u |_|U   |_|    \_)-\___/   |_| \_\  |_|  |_|  
 ||>>_     //  \\  \\    >>  _// \\_  )(\\,-      \\     //   \\_<<,-,,-.   
(__)__)   (_")("_)(__)  (__)(__) (__)(__)(_/     (__)   (__)  (__)(./  \.)  
```
# Kabuki VM Platform

The Kabuki VM Platform is based on mbed, an IoT platform similar to Arduino for ARM, adapted to work on x64 processors. Starting from the lowest going to highest levels the major components are:

1. **Hardware Abstraction Layer (HAL)** - Abstracts away the hardware to create a low-level virtual machine.
   1. Abstract hardware model for common processor peripherals.
   2. Hardware agnostic code shared between targets processors.
   3. Target implementations for various processors.
2. **Drivers** - Drivers for common communication peripherals such as SPI, UART, Timers, GPIO, etc.
3. **Platform** - Platform such as an operating system or bare-metal firmware that creates a higher-level virtual machine.
   1. Platform abstraction layer for common platform features.
   2. Platform agnostic code shared between platforms.
   3. Platform implementation.
4. **Features** - Cross-platform features.
   1. Feature model with common interface for features.
   2. Feature framework shares cross-platform code between features.
   3. Feature implementation.
5. **Firmware-to-Software Development Kit** - Cross-platform toolkit built on top of the features.
   1. Module model.
   3. Crabs layer.
   3. Module implementation.
