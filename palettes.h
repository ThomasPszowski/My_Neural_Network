#pragma once

#include <SDL.h>
#include <cmath>
#include <vector>

// Definicja palet kolorów (bez polskich znaków)
inline std::vector<SDL_Color> createRainbowPalette() {
    return {
        {255, 0, 0, 255},    // Red
        {0, 0, 255, 255},    // Blue
        {255, 255, 0, 255},  // Yellow
        {0, 128, 0, 255},    // Green
        {255, 165, 0, 255}   // Orange
    };
}

inline std::vector<SDL_Color> createRGBPalette() {
    return {
        
		{255, 0, 0, 255},    // Red
		{0, 255, 0, 255},    // Green
		{0, 0, 255, 255},    // Blue
        {255, 0, 255, 255},
		{255, 255, 255, 255},  
    };
}

inline std::vector<SDL_Color> createPastelPalette() {
    return {
        {255, 139, 158, 255}, // Pastel pink (delikatnie intensywniejszy)
        {153, 204, 255, 255}, // Pastel blue (delikatnie intensywniejszy)
        {153, 255, 178, 255}, // Pastel green (delikatnie intensywniejszy)
        {255, 179, 102, 255}, // Pastel orange (delikatnie intensywniejszy)
        {255, 255, 102, 255}  // Pastel yellow (delikatnie intensywniejszy)
    };
}

inline std::vector<SDL_Color> createBalancedPalette() {
    return {
        {255, 102, 119, 255}, // Pastel pink (mocno nasycony, ale nie na 100%)
        {102, 153, 255, 255}, // Pastel blue (mocno nasycony, ale nie na 100%)
        {102, 204, 102, 255}, // Pastel green (mocno nasycony, ale nie na 100%)
        {255, 153, 51, 255},  // Pastel orange (mocno nasycony, ale nie na 100%)
        {255, 255, 51, 255}   // Pastel yellow (mocno nasycony, ale nie na 100%)
    };
}


inline std::vector<SDL_Color> createMonochromePalette() {
    return {
        {50, 50, 50, 255},   // Dark gray
        {250, 250, 250, 255}, // Almost white
        {100, 100, 100, 255},// Medium gray
        {200, 200, 200, 255},// Very light gray
        {150, 150, 150, 255} // Light gray
        
        
    };
}

inline std::vector<SDL_Color> createWarmPalette() {
    return {
        {255, 87, 51, 255},  // Red-orange
        {205, 92, 92, 255},   // Indian red
        {255, 195, 113, 255},// Light orange
        {255, 160, 122, 255},// Light coral
        {255, 250, 205, 255}// Light gold
        
        
    };
}

inline std::vector<SDL_Color> createCoolPalette() {
    return {
        {0, 128, 128, 255},  // Teal
        {72, 61, 139, 255},   // Dark slate blue
        {70, 130, 180, 255}, // Steel blue
        {123, 104, 238, 255},// Medium slate blue
        {100, 149, 237, 255}// Cornflower blue
        
        
    };
}