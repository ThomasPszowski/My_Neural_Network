#pragma once

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "palettes.h"

SDL_Color generateColorFromWeights(std::vector<SDL_Color>& palette, std::vector<float>& weights);

template <typename T>
SDL_Color blendColors(const SDL_Color& color1, const SDL_Color& color2, T weight1, T weight2);

std::vector<float> f(float x, float y);

std::vector<float> mapCoordinatesSimple(int x, int y, int Height, int Width);

std::vector<float> mapCoordinates(int x, int y, int window_height, int window_width,
    float center_x = 0, float center_y = 0, float zoom = 1);

std::vector<int> inverseMapCoordinates(float mapped_x, float mapped_y, int window_height, int window_width,
    float center_x = 0, float center_y = 0, float zoom = 1);

SDL_Color getColorForPixel(int x, int y, int Height, int Width, std::vector<SDL_Color>& palette, float center_x = 0, float center_y = 0, float zoom = 1);

void fillPixels(SDL_Renderer* renderer, int Height, int Width, std::vector<SDL_Color>& palette, float center_x = 0, float center_y = 0, float zoom = 1);

