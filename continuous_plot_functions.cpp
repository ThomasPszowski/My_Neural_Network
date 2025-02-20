#pragma once

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "palettes.h"

SDL_Color generateColorFromWeights(std::vector<SDL_Color>& palette, std::vector<float>& weights) {
    // Inicjalizujemy składniki koloru
    float r = 0, g = 0, b = 0, a = 255;
    float totalWeight = 0;

    // Sumujemy składowe kolorów z wagami
    for (size_t i = 0; i < weights.size(); ++i) {
        r += palette[i].r * weights[i];
        g += palette[i].g * weights[i];
        b += palette[i].b * weights[i];
        totalWeight += weights[i];
    }

    // Dzielimy przez sumę wag, aby uzyskać średnią ważoną
    if (totalWeight > 0) {
        r /= totalWeight;
        g /= totalWeight;
        b /= totalWeight;
    }

    // Przekształcamy wartości do pełnej gamy od 0 do 255
    return { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), static_cast<Uint8>(a) };
}

template <typename T>
SDL_Color blendColors(const SDL_Color& color1, const SDL_Color& color2, T weight1, T weight2) {
    T totalWeight = weight1 + weight2;
    SDL_Color blendedColor;
    blendedColor.r = static_cast<Uint8>((color1.r * weight1 + color2.r * weight2) / totalWeight);
    blendedColor.g = static_cast<Uint8>((color1.g * weight1 + color2.g * weight2) / totalWeight);
    blendedColor.b = static_cast<Uint8>((color1.b * weight1 + color2.b * weight2) / totalWeight);
    blendedColor.a = static_cast<Uint8>((color1.a * weight1 + color2.a * weight2) / totalWeight);
    return blendedColor;
}

std::vector<float> f(float x, float y) {
    std::vector<float> result;
    result.push_back(x * x + y * y - 1);
    result.push_back(x - y);
    result.push_back(sin(x * 3) + y);
    result.push_back(-y*y*y + y*y -0.4 - x);
    return result;
}

std::vector<float> mapCoordinatesSimple(int x, int y, int Height, int Width) {
    std::vector<float> result(2);
    result[0] = (x - Width / 2) / (Width / 4.0);
    result[1] = -(y - Height / 2) / (Height / 4.0);
    return result;
}

std::vector<float> mapCoordinates(int x, int y, int window_height, int window_width,
    float center_x = 0, float center_y = 0, float zoom = 1) {
    std::vector<float> result(2); 
    result[0] = (x - window_width / 2.0f) / (window_width / (4.0f * zoom)) + center_x;
    result[1] = -(y - window_height / 2.0f) / (window_height / (4.0f * zoom)) + center_y;
    return result;
}

std::vector<int> inverseMapCoordinates(float mapped_x, float mapped_y, int window_height, int window_width,
    float center_x = 0, float center_y = 0, float zoom = 1) {
    std::vector<int> result(2);
    result[0] = static_cast<int>((mapped_x - center_x) * (window_width / (4.0f * zoom)) + window_width / 2.0f);
    result[1] = static_cast<int>((-mapped_y + center_y) * (window_height / (4.0f * zoom)) + window_height / 2.0f);
    return result;
}


// Funkcja zwracająca kolor na podstawie współrzędnych x, y
SDL_Color getColorForPixel(int x, int y, int Height, int Width, std::vector<SDL_Color>& palette, float center_x = 0, float center_y = 0, float zoom = 1) {
    std::vector<float> coords = mapCoordinates(x, y, Height, Width, center_x, center_y, zoom);
    std::vector<float> weights = f(coords[0], coords[1]);
    for (float& w : weights) {
        if (w < 0) w = 0;
        else w = w;
    }
    SDL_Color color = generateColorFromWeights(palette, weights);
    return color;
}

// Funkcja wypełniająca każdy piksel kolorem zwróconym przez getColorForPixel
void fillPixels(SDL_Renderer* renderer, int Height, int Width, std::vector<SDL_Color>& palette, float center_x = 0, float center_y = 0, float zoom = 1) {
    for (int y = 0; y < Height; ++y) {
        for (int x = 0; x < Width; ++x) {
            SDL_Color color = getColorForPixel(x, y, Height, Width, palette, center_x, center_y, zoom);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}