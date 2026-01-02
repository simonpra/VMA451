#pragma once

#ifndef VMA451_CHARSET_H
#define VMA451_CHARSET_H

#include <stdint.h>
#include <stddef.h>

// As we are in a C++ project, ensure the symbols are not mangled
// as it is a C-style header file.
#ifdef __cplusplus
extern "C" {
#endif

const uint8_t* getNumber2x5(char c, uint8_t offsetH, uint8_t *out_len);

//Symbols for environmental data display
extern const uint8_t symbol_degree_3x4[4];
extern const uint8_t symbol_humidity_3x3[3];
extern const uint8_t symbol_ppm_2x11[11];

#ifdef __cplusplus
}
#endif

#endif // VMA451_CHARSET_H