# BIP26_CAN Library
This library provides an interface for the BIP26_CAN module, which is designed to facilitate communication over the CAN bus. It was developed for the BIP 2026 at TH Köln and is intended to be used with the custom CAN transceiver boards designed for this project.

## Useful Information
- TX Pin: `GPIO 18`
- RX Pin: `GPIO 19`
- CAN Speed: 500 kbps
- Can ids are 11bit. The 3 most significant bits (MSB) define the priority, the next 4 bits represent the group ID, and the 4 least significant bits (LSB) are available for group-specific use.  
For example: A message from group 3 with priority 1 and a group-specific content ID of 5 would have the CAN ID `0b00100110101`
- The group ID can be configured via the 4-bit switch on the board and read through the GPIO pins `20` (LSB) to `23` (MSB).

## Installation
Simply copy the `BIP26_CAN` folder into the `lib` folder of your PlatformIO project. Then, include the library in your code with:  
```#include <BIP26_CAN.h>```

## Usage
There are two examples provided in the `examples` folder. Refer to the Files for more (specific) information.
