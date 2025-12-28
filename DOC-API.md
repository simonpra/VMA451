## API Documentation

### Class `VMA451`
Provides an interface to control the VMA451 (AiP1640/TM1640) 16x8 LED matrix display. It manages the low-level "two-wire" serial communication (CLK, DIO) via bit-banging.

### Constructor

#### `VMA451(uint8_t clk_pin, uint8_t dio_pin)`
Initializes the display driver.
*   **clk_pin**: GPIO pin number connected to the CLK pin of the module.
*   **dio_pin**: GPIO pin number connected to the DIO pin of the module.

---

### Public Methods

#### `void set_brightness(uint8_t brightness)`
Sets the display brightness.
*   **brightness**: A value between `0` (min) and `7` (max).

#### `void display_numbers(const char* str, const bool clear_after = true)`
Displays a string of numbers (0-9), dots (.), and minus signs (-).
*   **str**: The string to display (e.g., `"12.34"`, `"-5"`).
*   **clear_after**: If `true` (default), clears any remaining columns after the string.
*   **Note**: Characters are 2x5 pixels. The function handles spacing automatically. If the string is too long, it will be truncated at 16 columns.

#### `void display_column(uint8_t colID, uint8_t byte)`
Manually updates a single column.
*   **colID**: The column index (0-15).
*   **byte**: The 8-bit data for the column (LSB is the top pixel).
*   **Note**: This uses "Fixed Address" mode to update only one register without rewriting the whole display.

#### `void clear()`
Clears the entire display (sets all pixels to off).
