#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "palettes.h"
using namespace std;

const int Width = 800, Height = 800;
SDL_Rect rect;
SDL_Renderer* renderer = NULL;
vector<SDL_Color> chosenPalette;

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

vector<float> f(float x, float y) {
	vector<float> result;
    result.push_back(x*x + y*y - 1);
	result.push_back(x - y);
    
    result.push_back(sin(x) + y);

	return result;
}



// Funkcja zwracająca kolor na podstawie współrzędnych x, y
SDL_Color getColorForPixel(int x, int y) {
	vector<float> weights = f((x - Width / 2) / (Width / 4.0), -(y - Height / 2) / (Height / 4.0));
	for (float& w : weights) {
		if (w < 0) w = 0;
	}
	SDL_Color color = generateColorFromWeights(chosenPalette, weights);
    return color;
}

// Funkcja wypełniająca każdy piksel kolorem zwróconym przez getColorForPixel
void fillPixels() {
    for (int y = 0; y < Height; ++y) {
        for (int x = 0; x < Width; ++x) {
            SDL_Color color = getColorForPixel(x, y);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

int main(int argc, char** argv) {

	chosenPalette = createRGBPalette();

    rect.h = 10;
    rect.w = 10;

    SDL_Window* window = SDL_CreateWindow(
        "preview", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        Width,
        Height,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        std::cerr << "Nie można utworzyć okna SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Nie można utworzyć renderera SDL: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    bool is_running = true;
    SDL_Event ev;

    while (is_running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT)
                is_running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                fillPixels();

                SDL_RenderPresent(renderer);
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

