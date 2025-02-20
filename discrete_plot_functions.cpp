#pragma once

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "palettes.h"
#include "continuous_plot_functions.h"

void drawColoredSquare(SDL_Renderer* renderer, int centerX, int centerY, int size, SDL_Color color, int borderWidth = 1) {
    SDL_Rect square;
    square.w = size;
    square.h = size;
    square.x = centerX - size / 2;
    square.y = centerY - size / 2;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &square);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &square);
}

void plotData(SDL_Renderer* renderer, int Height, int Width,
    std::vector<std::vector<float>>& data, std::vector<SDL_Color>& palette,
    float center_x = 0, float center_y = 0, float zoom = 1) {
	for (auto& data_point : data) {
		std::vector<int> mapped = inverseMapCoordinates(data_point[0], data_point[1], Height, Width, center_x, center_y, zoom);
        SDL_Color color = palette[data_point[2]];
		drawColoredSquare(renderer, mapped[0], mapped[1], 10, color);
	}
}

