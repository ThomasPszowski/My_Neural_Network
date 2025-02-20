#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "palettes.h"
#include "continuous_plot_functions.h"
#include "discrete_plot_functions.h"
#include "example_data_points.h"

using namespace std;

const int Width = 800, Height = 800;
SDL_Rect rect;

SDL_Renderer* renderer = NULL;
vector<SDL_Color> chosenPalette;
int palette_index = 3;
bool key_1_pressed = false;
float center_x = 0; float center_y = 0; float zoom = 1;



void render() {
    fillPixels(renderer, Height, Width, chosenPalette, center_x, center_y, zoom);
	plotData(renderer, Height, Width, example_data, chosenPalette, center_x, center_y, zoom);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {

	chosenPalette = createBalancedPalette();

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
    render();
    while (is_running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT)
                is_running = false;
			if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (ev.button.button == SDL_BUTTON_LEFT) {
					vector<float> coords = mapCoordinates(ev.button.x, ev.button.y, Height, Width, center_x, center_y, zoom);
					center_x = coords[0];
					center_y = coords[1];
					render();
				}
			}
            if (ev.type == SDL_MOUSEWHEEL) {
                
                float delta = ev.wheel.y / 3.0;
				if (ev.wheel.y < 0) {
                    delta = -delta;
					zoom *= 1 + delta;
					render();
				}
				else if (ev.wheel.y > 0) {
					zoom /= 1 + delta;
					render();
				}
            }
            if (ev.key.keysym.sym == SDLK_DOWN) {
                center_y -= 0.1;
            }
            if (ev.key.keysym.sym == SDLK_UP) {
                center_y += 0.1;
            }
            if (ev.key.keysym.sym == SDLK_LEFT) {
                center_x -= 0.1;
            }
            if (ev.key.keysym.sym == SDLK_RIGHT) {
                center_x += 0.1;
            }

            if (ev.type == SDL_KEYDOWN) {
                
                if (ev.key.keysym.sym == SDLK_1 && !key_1_pressed) {
                    key_1_pressed = true;
                    palette_index++;
                    if (palette_index > 5) palette_index = 0;

                    switch (palette_index) {
                    case 0:
                        chosenPalette = createRainbowPalette();
                        break;
                    case 1:
                        chosenPalette = createRGBPalette();
                        break;
                    case 2:
                        chosenPalette = createPastelPalette();
                        break;
                    case 3:
                        chosenPalette = createBalancedPalette();
                        break;
                    case 4:
                        chosenPalette = createWarmPalette();
                        break;
                    case 5:
                        chosenPalette = createCoolPalette();
                        break;
                    }
                    
                }
                render();
            }
            else if (ev.type == SDL_KEYUP) {
                if (ev.key.keysym.sym == SDLK_1) {
                    key_1_pressed = false;  // Resetujemy stan klawisza, aby zmiana palety mogła się zdarzyć ponownie
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

