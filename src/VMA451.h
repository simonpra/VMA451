#ifndef VMA451_H
#define VMA451_H

#include <stdint.h>
#include "pico/stdlib.h"

/****************************************
 * VMA451 16x8 LED Matrix Display
 * using AiP1640 driver (similar to TM1640)
 * 16 columns, 8 rows -> 16x 8bits registers
 * Uses "two-wire" serial interface (CLK, DIO)
 * wich is NOT I2C compatible !
 ****************************************/
class VMA451 {
public:
    VMA451(uint8_t clk_pin, uint8_t dio_pin);
    void set_brightness(uint8_t brightness);
    void display_column(uint8_t colID, uint8_t byte);
    void display_numbers(const char* str, const bool clear_after = true);
    void set_buffer_numbers(const char* str, const bool clear_after = true);
    void set_buffer_symbol(const uint8_t* symbol, size_t symbol_len);
    void set_buffer_raw(const uint8_t* buffer, size_t length);
    void display_buffer();
    void clear();
    void flip_display_horizontal();
    void flip_display_vertical();

private:
    uint8_t _clk_pin;
    uint8_t _dio_pin;
    uint8_t _brightness;
    uint8_t _buffer[16];
    bool    _flipH;
    bool    _flipV;

    void _start_CMD();
    void _stop_CMD();
    void _write_byte(uint8_t byte);
    void _fixed_address_CMD();
    uint8_t _reverse_byte(uint8_t b);
};

#endif