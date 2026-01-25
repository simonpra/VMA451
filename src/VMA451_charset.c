#include <stdint.h>
#include <stddef.h>


// Using CONST to store character data in flash memory
// Format : each byte is a column of 8 pixels (LSB at top)
const uint8_t numbers[][2] = {
    ///// NUMBER 0 ////////
    // ## // Full 2x5 block
    // ## // 
    // ## //
    // ## //
    // ## //
    ///////////////////////
    {
        0b11111,
        0b11111
    },
    ///// NUMBER 1 ////////
    // .# //
    // ## //
    // .# //
    // .# //
    // .#] //
    ///////////////////////
    {
        0b00010,
        0b11111
    },
    ///// NUMBER 2 ////////
    // ## //
    // .# //
    // ## //
    // #. //
    // ## //
    ///////////////////////
    {
        0b11101,
        0b10111
    },
    ///// NUMBER 3 ////////
    // ## //
    // .# //
    // ## //
    // .# //
    // ## //
    ///////////////////////
    {
        0b10101,
        0b11111
    },
    ///// NUMBER 4 ////////
    // ## // It's kind of work
    // ## //
    // ## //
    // .# //
    // .# //
    ///////////////////////
    {
        0b00111,
        0b11111
    },
    ///// NUMBER 5 ////////
    // ## //
    // #. //
    // ## //
    // .# //
    // ## //
    ///////////////////////
    {
        0b10111,
        0b11101
    },
    ///// NUMBER 6 ////////
    // ## //
    // #. //
    // ## //
    // ## //
    // ## //
    ///////////////////////
    {
        0b11111,
        0b11101
    },
    ///// NUMBER 7 ////////
    // ## //
    // .# //
    // .# //
    // #. //
    // #. //
    ///////////////////////
    {
        0b11001,
        0b00111
    },
    ///// NUMBER 8 ////////
    // ## // 2x 2x2 blocks
    // ## // Kind of works
    // .. //
    // ## //
    // ## //
    ///////////////////////
    {
        0b11011,
        0b11011
    },
    ///// NUMBER 9 ////////
    // ## //
    // ## //
    // ## //
    // .# //
    // ## //
    ///////////////////////
    {
        0b10111,
        0b11111
    },
    ///// MINUS (-) ////////
    // .. //
    // .. //
    // ## //
    // .. //
    // .. //
    ///////////////////////
    {
        0b00100,
        0b00100
    }
};

// Get character data for number 0-9, minus sign and dot
// By default, the character is aligned to the bottom of the 8-pixel height
// offsetH: vertical offset in pixels, 0 = top, default 3 = bottom (5+3 = 8bits)
const uint8_t* getNumber2x5(char c, uint8_t offsetH, uint8_t *out_len) {
    static uint8_t charNum[5] = {0,0,0,0,0}; // Max 5 = %
    const uint8_t *src = NULL;
    uint8_t src_len = 0; // set to 1 for default spacer if no match

    // Clear previous data
    for(size_t i=0; i<sizeof(charNum); i++) {
        charNum[i] = 0x00;
    }

    if (c >= '0' && c <= '9') {
        src = numbers[c - '0'];
        src_len = 2;
    }
    else if (c == '-') {
        src = numbers[10]; src_len = 2;
    }
    else if (c == '.') {
        static const uint8_t dot[1] = {0b10000};
        src = dot;
        src_len = 1;
    }
    else if( c == '%' ) {
        // Special case for percent sign % : 4x5
        // ....
        // #..#
        // ..#.
        // .#..
        // #..#
        static const uint8_t percent[5] = {
            0b00000,
            0b10010,
            0b01000,
            0b00100,
            0b10010
        };
        src = percent;
        src_len = 5;
    }

    if (src) {
        for(size_t i=0; i<src_len; i++) {
            charNum[i] = src[i] << offsetH;
        }
    }

    // Return total length
    *out_len = (c=='%') ? src_len : src_len + 1;
    
    return charNum;
}

////////////////////////////////////////////////
// Additional symbols for environmental data display
////////////////////////////////////////////////
// By default, symbols are aligned to the top of the 8-pixel height
////////////////////////////////////////////////
const uint8_t symbol_degree_3x4[4] =
    ///// °C //////////////
    // #.## //
    // ..#. // 
    // ..## //
    ///////////////////////
    {
        0b001,
        0b000,
        0b111,
        0b101,
    };
const uint8_t symbol_humidity_3x3[3] =
    ///// H //////////////
    // #.# //
    // ### //
    // #.# //
    ///////////////////////
    {
        0b111,
        0b010,
        0b111,
    };
const uint8_t symbol_ppm_2x11[11] =
    ///// PPM //////////////
    // ##.##.##### //
    // #..#..#.#.# //
    ///////////////////////
    {
        0b11,
        0b01,
        0b00,
        0b11,
        0b01,
        0b00,
        0b11,
        0b01,
        0b11,
        0b01,
        0b11,
    };
const uint8_t symbol_tvoc_3x14[14] =
    ///// TVOC ////////////
    // ###.#.#.###.## //
    // .#..#.#.#.#.#. //
    // .#...#..###.## //
    ///////////////////////
    {
        0b001,
        0b111,
        0b001,
        0b000,
        0b011,
        0b100,
        0b011,
        0b000,
        0b111,
        0b101,
        0b111,
        0b000,
        0b111,
        0b101,
    };
const uint8_t symbol_eco2_3x13[13] =
    ///// ECO2 //////////////
    // ###.##.###... //
    // ##..#..#.#.## //
    // ###.##.###.## //
    ///////////////////////
    {
        0b111,
        0b111,
        0b101,
        0b000,
        0b111,
        0b101,
        0b000,
        0b111,
        0b101,
        0b111,
        0b000,
        0b110,
        0b110,
    };
const uint8_t symbol_aqi_3x10[10] =
    ///// AQI //////////////
    // ###.###..# //
    // ###.#.#..# //
    // #.#.####.# //
    ///////////////////////
    {
        0b111,
        0b011,
        0b111,
        0b000,
        0b111,
        0b101,
        0b111,
        0b100,
        0b000,
        0b111,
    };