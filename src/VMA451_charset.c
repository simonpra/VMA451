#include <stdint.h>
#include <stddef.h>


// On utilise 'const' pour que cela reste en mémoire Flash (si microcontrôleur)
// Format : Chaque octet est une colonne verticale. LSB (bit 0) en haut.
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
    // .# //
    // .# //
    ///////////////////////
    {
        0b00001,
        0b11111
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

// Fonction helper pour récupérer le pointeur vers le bon caractère
const uint8_t* getNumber2x5(char c, uint8_t offsetH, uint8_t *out_len) {
    static uint8_t charNum[3];
    const uint8_t *src = NULL;
    uint8_t src_len = 0;

    // Initialize with 0 (default space)
    charNum[0] = 0; charNum[1] = 0; charNum[2] = 0;

    if (c >= '0' && c <= '9') { src = numbers[c - '0']; src_len = 2; }
    else if (c == '-') { src = numbers[10]; src_len = 2; }
    else if (c == '.') {
        static const uint8_t dot[1] = {0b10000};
        src = dot;
        src_len = 1;
    }

    if (src) {
        for(uint8_t i=0; i<src_len; i++) {
            charNum[i] = src[i] << offsetH;
        }
    }

    // Return total length: source length + 1 spacer
    *out_len = src_len + 1;
    
    return charNum;
}