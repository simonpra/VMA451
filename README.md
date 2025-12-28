# VMA451 Library for Raspberry Pi Pico
VMA451 16x8 Leds Matrix (aip1640/tm1640 drivers) - C/C++ Raspberry PICO library
------------------

This is a C++ library for controlling the VMA451 16x8 LED Matrix Display (based on the AiP1640 driver) using a Raspberry Pi Pico.

## Features

*   **16x8 LED Matrix Control**: Individually control columns or display text.
*   **Brightness Control**: Adjustable brightness levels (0-7).
*   **Text Display**: Built-in support for displaying numbers, minus sign, and decimal points.
*   **Simple Interface**: Uses a custom 2-wire serial protocol (CLK, DIO).

## Hardware Connection

Connect the VMA451 module to the Raspberry Pi Pico:

*   **CLK**: Connect to any GPIO pin (e.g., GP14).
*   **DIO**: Connect to any GPIO pin (e.g., GP15).
*   **VCC**: 3.3V or 5V.
*   **GND**: Ground.

## Supported Characters

The `display_numbers` method currently supports a limited character set optimized for 2x5 pixel fonts:
*   Numbers: `0` - `9`
*   Symbols: `-` (minus), `.` (dot)
*   Space

## API Documentation

### Constructor
```cpp
VMA451(uint8_t clk_pin, uint8_t dio_pin);
```
Initializes the display with the specified Clock (CLK) and Data (DIO) pins.

### Methods

#### `void set_brightness(uint8_t brightness)`
Sets the display brightness.
*   `brightness`: A value from 0 (dimmest) to 7 (brightest).

#### `void display_numbers(const char* str, const bool clear_after = true)`
Displays a string of numbers on the matrix.
*   `str`: The string to display (e.g., "12.34", "-5").
*   `clear_after`: If `true` (default), clears any remaining columns after the string.
*   Note: The string is truncated if it exceeds the display width.

#### `void display_column(uint8_t colID, uint8_t byte)`
Sets the state of a specific column.
*   `colID`: The column index (0-15).
*   `byte`: The 8-bit value representing the row LEDs for that column (LSB is top).

#### `void clear()`
Clears the entire display (turns off all LEDs).

## Example Usage

See `examples/basic_example.cpp` for a complete example.

```cpp
#include "pico/stdlib.h"
#include "VMA451.h"

// Define pins
#define CLK_PIN 14
#define DIO_PIN 15

int main() {
    stdio_init_all();

    // Initialize display
    VMA451 display(CLK_PIN, DIO_PIN);

    // Set brightness (0-7)
    display.set_brightness(3);

    while (true) {
        // Display a number
        display.display_numbers("12.34", true);
        sleep_ms(2000);
    }

    return 0;
}

## License

BSD-3-Clause
