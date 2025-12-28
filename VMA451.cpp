/******************************************************
 * Copyright (c) 2025 Simon Pracchinetti - ized.ch
 * RASPBERRY PI PICO library to drive the VMA451 (aip1640/tm1640) 16x8 LED Matrix Display
 * WORK-IN-PROGRESS
 * 
 * Developed from documentation and examples of the aip1640/tm1640 led matrix driver
 * for the VMA451 16x8 LED Matrix Display by Velleman/Whadda.
 * 
 * Communication protocol is NOT I2C compatible,
 * it uses a "two-wire" serial interface (CLK, DIO).
 * Uses "manual" bit-banging via GPIO pins to implement the protocol
 * according to the datasheet.
 * 
 * Should work with any aip1640/tm1640 based 16x8 led matrix display.
 * For 7 segements displays, charset must be adapted.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************/

#include "VMA451.h"
#include <cstdint>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "VMA451_charset.c"

// VMA451 commands
#define VMA451_CMD_SET_DATA    0x40
#define VMA451_CMD_SET_ADDR    0xC0
#define VMA451_CMD_SET_DISPLAY 0x80

// VMA451 data settings
#define VMA451_INCR_ADDR      0x00
#define VMA451_FIXED_ADDR     0x04

// VMA451 display settings
#define VMA451_DISP_PULSE_1   0x00
#define VMA451_DISP_PULSE_2   0x01
#define VMA451_DISP_PULSE_4   0x02
#define VMA451_DISP_PULSE_10  0x03
#define VMA451_DISP_PULSE_11  0x04
#define VMA451_DISP_PULSE_12  0x05
#define VMA451_DISP_PULSE_13  0x06
#define VMA451_DISP_PULSE_14  0x07
#define VMA451_DISP_ON        0x08

// Display dimensions
#define VMA451_PX_WIDTH       16
#define VMA451_PX_HEIGHT      8

#define SLEEP_US_DELAY        2

/******************************************************
 * @class VMA451
 * @brief Provides an interface to control the VMA451 LED matrix display.
 *
 * The VMA451 class is designed to manage and operate a 16x8 LED matrix display. It offers
 * functionalities like setting brightness, displaying characters, and manually controlling
 * individual columns or writing a full display buffer.
 *
 * This class encapsulates low-level communication protocols required to interface with the
 * VMA451 display, including GPIO initialization, start/stop commands, and byte data transfers.
 *
 * Features:
 * - Adjustable brightness levels (0-7).
 * - Management of display data using incremental or fixed addressing modes.
 * - Convenient methods to display numeric values and symbols.
 * - Compatibility with a column-by-column or full-buffer update approach.
 *
 * Example usage:
 * @code
 * VMA451 display(clk_pin, dio_pin);
 * display.set_brightness(5);
 * display.display_numbers("123.45", true);
 * display.clear();
 * @endcode
 ******************************************************/
VMA451::VMA451(uint8_t clk_pin, uint8_t dio_pin) :
    _clk_pin(clk_pin),
    _dio_pin(dio_pin),
    _brightness(0x04),
    _buffer{0}
{
    gpio_init(_clk_pin);
    gpio_init(_dio_pin);
    gpio_set_dir(_clk_pin, GPIO_OUT);
    gpio_set_dir(_dio_pin, GPIO_OUT);

    gpio_put(_clk_pin, 0);
    gpio_put(_dio_pin, 0);
    sleep_us(SLEEP_US_DELAY);
}

void VMA451::_start_CMD() {
    // Start condition input data is CLK Is high
    // DIO from high to low;

    // ensure DIO and CLK are high
    gpio_put(_dio_pin, 1);
    sleep_us(SLEEP_US_DELAY);
    gpio_put(_clk_pin, 1);
    sleep_us(SLEEP_US_DELAY);
    // pull DIO low while CLK is high
    gpio_put(_dio_pin, 0);
    sleep_us(SLEEP_US_DELAY);
}

void VMA451::_stop_CMD() {
    // termination condition is CLK is high
    // DIO from low to high

    // ensure DIO is LOW and CLK is high
    gpio_put(_dio_pin, 0);
    sleep_us(SLEEP_US_DELAY);
    gpio_put(_clk_pin, 1);
    sleep_us(SLEEP_US_DELAY);
    // pull DIO high while CLK is high
    gpio_put(_dio_pin, 1);
    sleep_us(SLEEP_US_DELAY);
    // CLK can remain high
}

void VMA451::_write_byte(uint8_t byte) {
    // CLK should be low before calling this function
    // see start function
    for (size_t i = 0; i < 8; i++) {
        gpio_put(_clk_pin, 0);
        sleep_us(SLEEP_US_DELAY);
        gpio_put(_dio_pin, (byte & 0x01));
        byte >>= 1;
        sleep_us(SLEEP_US_DELAY);
        gpio_put(_clk_pin, 1);
        sleep_us(SLEEP_US_DELAY);
    }
    // NO ACK handling, it finishes after 8 bits
}

void VMA451::set_brightness(uint8_t brightness) {
    _brightness = brightness;
    // Brightness (0-7)
    _start_CMD();
    _write_byte(VMA451_CMD_SET_DISPLAY | VMA451_DISP_ON | (_brightness & 0x07));
    _stop_CMD();
}

void VMA451::_fixed_address_CMD() {
    _start_CMD();
    _write_byte(VMA451_CMD_SET_DATA | VMA451_FIXED_ADDR);
    _stop_CMD();
}

/******************************************************
 * Displays a single column on the VMA451 LED matrix.
 *
 * This function sets the fixed address mode, specifies the column address,
 * and writes the provided byte of data to the specified column.
 * Byte is send LSB first, first bit beeing top pixel.
 *
 * @param colID The column index (0-15) to update on the display.
 * @param byte The data byte representing pixel values for the column.
 * Byte is send LSB first, first bit beeing top pixel.
 *
 * The function ensures that only one column is updated at a time while
 * leaving the remaining display contents unchanged. The operation
 * is performed in a single write cycle.
 ******************************************************/
void VMA451::display_column(uint8_t colID, uint8_t byte) {
    // Set fixed address mode
    _fixed_address_CMD();

    _start_CMD();
    // colID = colID & 0x0F; // 16 columns max
    _write_byte(VMA451_CMD_SET_ADDR | colID & 0x0F);
    _write_byte(byte);
    _stop_CMD();
}

/******************************************************
 * Writes the entire display buffer to the VMA451 LED matrix.
 *
 * Sets the incremental address mode for the write operation and starts
 * from the default address (0x00). Transfers all 16 bytes from the
 * internal _buffer to the display, updating all columns sequentially.
 *
 * @note The _buffer must be fully populated with the desired pixel data
 * before calling this function. Each byte represents a column of 8 pixels.
 *
 * This function ensures that the current display image stored in _buffer
 * is shown on the VMA451 LED matrix in a single operation.
 ******************************************************/
void VMA451::_write_buffer() {
    // Set incremental address mode
    _start_CMD();
    _write_byte(VMA451_CMD_SET_DATA | VMA451_INCR_ADDR);
    _stop_CMD();

    _start_CMD();
    // Set starting address (default 0)
    _write_byte(VMA451_CMD_SET_ADDR | 0x00);
    // Write all bytes from buffer
    for (size_t i = 0; i < 16; i++) {
        _write_byte(_buffer[i]);
    }
    _stop_CMD();
}

/******************************************************
 * Display number 0-9 with . separator and - sign (12.5, -3.0, 0-9, etc.)
 * Number and - characters are 2x5 pixels + 1x5 pixel spacer.
 * Dot is 1x5 pixels + 1x5 pixel spacer.
 * @param str The input string to display, containing numeric characters and symbols.
 * @param clear_after If true, clears the remaining display columns after writing the string.
 *
 * The function processes each character in the input string, retrieves their
 * 2x5 bitmap representation, and writes them to the display buffer.
 * @see getNumber2x5 in VMA451_charset.c for character bitmaps.
 * 
 * Characters process will stop if the display buffer is full (16 columns).
 * TODO: Implement scrolling for longer strings.
 * 
 * Any unused columns in the display are cleared based on the clear_after parameter.
 ******************************************************/
void VMA451::display_numbers(const char* str, const bool clear_after) {
    // size_t is "native" 32-bits unsigned integer type
    // better for use inside cpu functions and loops
    // as it matches the cpu 32-bits architecture
    size_t len = strlen(str);
    size_t colID = 0;

    for (size_t i = 0; i < len; i++) {
        uint8_t out_len = 0;
        const uint8_t* char_bitmap = getNumber2x5(str[i], 3, &out_len); // Align char to bottom with offsetH=3
        // out_len is the number of columns for the character (including spacer)
        // could be 1 (space), 2 (dot), or 3 for numbers and minus sign
        for(size_t j = 0; j < out_len; j++) {
            // Write byte to _buffer
            _buffer[colID] = char_bitmap[j];
            // Next column
            colID++;
            if (colID >= sizeof(_buffer)) return; // Terminate to prevent overflow
        }
    }

    // Clear remaining columns if string is shorter than VMA451_PX_WIDTH (_buffer size)
    if( clear_after && colID < sizeof(_buffer) ) {
        for (; colID < sizeof(_buffer); colID++) {
            _buffer[colID] = 0x00;
        }
    }
    _write_buffer();
}

void VMA451::clear() {
    for (size_t i = 0; i < 16; i++) {
        display_column(i, 0x00);
    }
}
