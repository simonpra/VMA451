# VMA451 Library API Documentation

## Class `VMA451`

The `VMA451` class provides an interface to control the VMA451 (AiP1640/TM1640) 16x8 LED matrix display. It manages the low-level "two-wire" serial communication (CLK, DIO) and provides methods for displaying numbers and manipulating the display buffer.

### Constructor

#### `VMA451(uint8_t clk_pin, uint8_t dio_pin)`
Initializes the display driver.

*   **Parameters:**
    *   `clk_pin`: The GPIO pin number connected to the CLK pin of the module.
    *   `dio_pin`: The GPIO pin number connected to the DIO pin of the module.

---

### Display Control Methods

#### `void set_brightness(uint8_t brightness)`
Sets the display brightness.

*   **Parameters:**
    *   `brightness`: A value between `0` (dimmest) and `7` (brightest).

#### `void clear()`
Clears the entire display (sets all pixels to off) and clears the internal buffer.

---

### Direct Display Methods

#### `void display_numbers(const char* str, const bool clear_after = true)`
Displays a string of numbers (0-9), dots (.), and minus signs (-) directly to the display.

*   **Parameters:**
    *   `str`: The string to display (e.g., `"12.34"`, `"-5"`).
    *   `clear_after`: If `true` (default), clears any remaining columns after the string.
*   **Note:** This method updates the display immediately. Characters are 2x5 pixels. The function handles spacing automatically. If the string is too long, it will be truncated at 16 columns.

#### `void display_column(uint8_t colID, uint8_t byte)`
Manually updates a single column on the display.

*   **Parameters:**
    *   `colID`: The column index (0-15).
    *   `byte`: The 8-bit data for the column (LSB is the top pixel).
*   **Note:** This uses "Fixed Address" mode to update only one register without rewriting the whole display.

---

### Buffer Management Methods

These methods allow you to manipulate an internal buffer before sending it to the display. This is useful for constructing complex frames or animations.

#### `void set_buffer_numbers(const char* str, const bool clear_after = true)`
Writes a string of numbers to the internal buffer.

*   **Parameters:**
    *   `str`: The string to write to the buffer.
    *   `clear_after`: If `true` (default), clears the rest of the buffer after the string.
*   **Note:** This does NOT update the display. Call `display_buffer()` to show the changes.

#### `void set_buffer_symbol(const uint8_t* symbol, size_t symbol_len)`
Writes a custom raw byte sequence (symbol) to the internal buffer.

*   **Parameters:**
    *   `symbol`: Pointer to an array of bytes representing the columns of the symbol.
    *   `symbol_len`: The number of columns to write.
*   **Note:** Writes into the first 3 bits of each column, starting from the end. Meaning symbol will be top-right of the display.

#### `void display_buffer()`
Sends the contents of the internal buffer to the display.
This updates all 16 columns of the display at once.
