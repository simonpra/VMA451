/**
 * Copyright (c) 2025 Simon Pracchinetti
 * Example usage of the VMA451 library for Raspberry Pi Pico.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "VMA451.h"

// Define pins
// Adjust these to match your wiring
#define CLK_PIN 14
#define DIO_PIN 15

int main() {
    stdio_init_all();
    sleep_ms(2000); // Wait for serial monitor

    printf("VMA451 Example Start\n");

    // Initialize display
    VMA451 display(CLK_PIN, DIO_PIN);

    // Set brightness (0-7)
    display.set_brightness(3);

    while (true) {
        // --- Direct Display Mode ---
        printf("Direct Display Mode\n");
        
        // Display a number
        display.display_numbers("12.34");
        sleep_ms(1500);

        // Display negative number
        display.display_numbers("-56.7");
        sleep_ms(1500);

        // Display integer
        display.display_numbers("890");
        sleep_ms(1500);

        display.clear();
        sleep_ms(500);

        // --- Buffer Mode ---
        printf("Buffer Mode\n");

        // Prepare buffer with a number
        display.set_buffer_numbers("1234");
        // Nothing is shown yet
        sleep_ms(500);
        // Now show it
        display.display_buffer();
        sleep_ms(1500);

        // Custom symbol (e.g., a smiley face or pattern)
        // 8 bytes for 8 columns
        uint8_t smiley[] = {
            0b00111100,
            0b01000010,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b01000010,
            0b00111100
        };
        
        // Clear buffer first
        display.clear(); 
        
        // Write custom symbol to buffer
        display.set_buffer_symbol(smiley, 8);
        display.display_buffer();
        sleep_ms(2000);

        // --- Manual Column Control ---
        printf("Manual Column Control\n");
        display.clear();
        for(int i=0; i<16; i++) {
            display.display_column(i, 0xFF); // Turn on all LEDs in column
            sleep_ms(50);
            display.display_column(i, 0x00); // Turn off
        }
        sleep_ms(1000);
    }

    return 0;
}
