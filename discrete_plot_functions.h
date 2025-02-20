#pragma once

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "palettes.h"

void drawColoredSquare(SDL_Renderer* renderer, int centerX, int centerY, int size, SDL_Color color, int borderWidth = 1);

void plotData(SDL_Renderer* renderer, int Height, int Width,
    std::vector<std::vector<float>>& data, std::vector<SDL_Color>& palette,
    float center_x = 0, float center_y = 0, float zoom = 1);


