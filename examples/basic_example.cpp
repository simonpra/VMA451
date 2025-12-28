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

    // Initialize display
    VMA451 display(CLK_PIN, DIO_PIN);

    // Set brightness (0-7)
    display.set_brightness(3);

    while (true) {
        // Display a number
        // The second parameter 'true' clears the rest of the display
        display.display_numbers("12.34", true);
        sleep_ms(2000);

        // Clear display
        display.clear();
        sleep_ms(500);
        
        // Display another number with minus sign
        display.display_numbers("-56.7", true);
        sleep_ms(2000);

        // Display an integer
        display.display_numbers("890", true);
        sleep_ms(2000);
    }

    return 0;
}
