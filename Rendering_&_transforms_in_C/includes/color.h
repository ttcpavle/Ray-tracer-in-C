#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef union {
    struct {
        uint8_t r, g, b;
    };
} Color;

extern const Color red;
extern const Color green;
extern const Color blue;
extern const Color gray;
extern const Color yellow;
extern const Color cyan;
extern const Color magenta;
extern const Color orange;
extern const Color purple;
extern const Color pink;
extern const Color brown;
extern const Color lightGray;
extern const Color darkGray;
extern const Color teal;
extern const Color navy;
extern const Color olive;
extern const Color maroon;
extern const Color lime;
extern const Color gold;
extern const Color black;

#endif