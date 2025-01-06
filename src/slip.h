
#ifndef SLIP_H
#define SLIP_H
#include "clay.h"
// clang-format off

// Stollen from raylib ------
// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define COLOR_LIGHTGRAY  (Clay_Color){ 200, 200, 200, 255 }   // Light Gray
#define COLOR_GRAY       (Clay_Color){ 130, 130, 130, 255 }   // Gray
#define COLOR_DARKGRAY   (Clay_Color){ 80, 80, 80, 255 }      // Dark Gray
#define COLOR_YELLOW     (Clay_Color){ 253, 249, 0, 255 }     // Yellow
#define COLOR_GOLD       (Clay_Color){ 255, 203, 0, 255 }     // Gold
#define COLOR_ORANGE     (Clay_Color){ 255, 161, 0, 255 }     // Orange
#define COLOR_PINK       (Clay_Color){ 255, 109, 194, 255 }   // Pink
#define COLOR_RED        (Clay_Color){ 230, 41, 55, 255 }     // Red
#define COLOR_MAROON     (Clay_Color){ 190, 33, 55, 255 }     // Maroon
#define COLOR_GREEN      (Clay_Color){ 0, 228, 48, 255 }      // Green
#define COLOR_LIME       (Clay_Color){ 0, 158, 47, 255 }      // Lime
#define COLOR_DARKGREEN  (Clay_Color){ 0, 117, 44, 255 }      // Dark Green
#define COLOR_SKYBLUE    (Clay_Color){ 102, 191, 255, 255 }   // Sky Blue
#define COLOR_BLUE       (Clay_Color){ 0, 121, 241, 255 }     // Blue
#define COLOR_DARKBLUE   (Clay_Color){ 0, 82, 172, 255 }      // Dark Blue
#define COLOR_PURPLE     (Clay_Color){ 200, 122, 255, 255 }   // Purple
#define COLOR_VIOLET     (Clay_Color){ 135, 60, 190, 255 }    // Violet
#define COLOR_DARKPURPLE (Clay_Color){ 112, 31, 126, 255 }    // Dark Purple
#define COLOR_BEIGE      (Clay_Color){ 211, 176, 131, 255 }   // Beige
#define COLOR_BROWN      (Clay_Color){ 127, 106, 79, 255 }    // Brown
#define COLOR_DARKBROWN  (Clay_Color){ 76, 63, 47, 255 }      // Dark Brown
#define COLOR_WHITE      (Clay_Color){ 255, 255, 255, 255 }   // White
#define COLOR_BLACK      (Clay_Color){ 0, 0, 0, 255 }         // Black
#define COLOR_BLANK      (Clay_Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define COLOR_MAGENTA    (Clay_Color){ 255, 0, 255, 255 }     // Magenta
#define COLOR_RAYWHITE   (Clay_Color){ 245, 245, 245, 255 }   // My own White (raylib logo)

void Slider(float *value, float min, float max, Clay_String label);
void CheckBox(bool *value, Clay_String label);

void TextEditor(char *text, Clay_TextElementConfig cfg);

Clay_String ClayStringFromCString(const char *str); // str MUST be null terminated

// clang-format restore
#endif // slip_h