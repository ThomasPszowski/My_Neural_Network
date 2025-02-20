#pragma once

#include <SDL.h>
#include <cmath>
#include <vector>

// Definicja palet kolorów (bez polskich znaków)
inline std::vector<SDL_Color> createRainbowPalette() {
    return {
        {255, 0, 0, 255},    // Red
        {255, 165, 0, 255},  // Orange
        {255, 255, 0, 255},  // Yellow
        {0, 128, 0, 255},    // Green
        {0, 0, 255, 255}     // Blue
    };
}

inline std::vector<SDL_Color> createRGBPalette() {
    return {
        
		{255, 0, 0, 255},    // Red
		{0, 255, 0, 255},    // Green
		{0, 0, 255, 255},    // Blue
		{255, 255, 255, 255},  // Yellow
    };
}

inline std::vector<SDL_Color> createPastelPalette() {
    return {
        {255, 179, 186, 255}, // Pastel pink
        {255, 223, 186, 255}, // Pastel orange
        {255, 255, 186, 255}, // Pastel yellow
        {186, 255, 201, 255}, // Pastel green
        {186, 225, 255, 255}  // Pastel blue
    };
}

inline std::vector<SDL_Color> createMonochromePalette() {
    return {
        {50, 50, 50, 255},   // Dark gray
        {100, 100, 100, 255},// Medium gray
        {150, 150, 150, 255},// Light gray
        {200, 200, 200, 255},// Very light gray
        {250, 250, 250, 255} // Almost white
    };
}

inline std::vector<SDL_Color> createWarmPalette() {
    return {
        {255, 87, 51, 255},  // Red-orange
        {255, 195, 113, 255},// Light orange
        {255, 250, 205, 255},// Light gold
        {255, 160, 122, 255},// Light coral
        {205, 92, 92, 255}   // Indian red
    };
}

inline std::vector<SDL_Color> createCoolPalette() {
    return {
        {0, 128, 128, 255},  // Teal
        {70, 130, 180, 255}, // Steel blue
        {100, 149, 237, 255},// Cornflower blue
        {123, 104, 238, 255},// Medium slate blue
        {72, 61, 139, 255}   // Dark slate blue
    };
}