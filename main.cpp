#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

#include "palettes.h"
#include "continuous_plot_functions.h"
#include "discrete_plot_functions.h"
#include "example_data_points.h"
#include "NeuralNetwork.h"

using namespace std;

int Width = 800, Height = 800;
SDL_Rect rect;

SDL_Renderer* renderer = NULL;
vector<SDL_Color> chosenPalette;
int palette_index = 3;
bool key_1_pressed = false;
float center_x = 0; float center_y = 0; float zoom = 1;
mutex mtx;

float relu(float x) {
	return max(0.0f, x);
}

vector<int> layer_sizes = { 2, 3, 4 };
NeuralNetwork nn(layer_sizes, relu);
vector<float> func_smooth(float x, float y) {
	vector<float> input = { x, y };
	vector<float> output = nn.feedforward(input);
	SquareVector(output);
	return SoftMax(output);
}

vector<float> func_hard(float x, float y) {
    vector<float> input = { x, y };
    vector<float> output = nn.feedforward(input);
	float max = *max_element(output.begin(), output.end());
    for (int i = 0; i < output.size(); i++) {
		if (output[i] == max) {
			output[i] = 1;
		}
		else {
			output[i] = 0;
		}
    }
	return output;
}

vector<float>(*pfunc)(float, float) = func_hard;

void render() {
    while (1) {
		mtx.lock();
        fillPixels(renderer, Height, Width, chosenPalette, center_x, center_y, zoom, pfunc);
        mtx.unlock();
        plotData(renderer, Height, Width, example_data2, chosenPalette, center_x, center_y, zoom);
        SDL_RenderPresent(renderer);
		
    }
}

int main(int argc, char** argv) {
    nn.randomizeWeights(0);

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
    std::thread t(render);
    while (is_running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT)
                is_running = false;



			if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (ev.button.button == SDL_BUTTON_LEFT) {
					vector<float> coords = mapCoordinates(ev.button.x, ev.button.y, Height, Width, center_x, center_y, zoom);
					center_x = coords[0];
					center_y = coords[1];
				}
			}
            if (ev.type == SDL_MOUSEWHEEL) {
                
                float delta = ev.wheel.y / 3.0;
				if (ev.wheel.y < 0) {
                    delta = -delta;
					zoom *= 1 + delta;
				}
				else if (ev.wheel.y > 0) {
					zoom /= 1 + delta;
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
                if (ev.key.keysym.sym == SDLK_h) {
					pfunc = func_hard;
                }
                if (ev.key.keysym.sym == SDLK_j) {
					pfunc = func_smooth;
                }
                if (ev.key.keysym.sym == SDLK_c) {
					std::cout << "Cost: " << nn.cost(example_data2) << std::endl;
                }
                if (ev.key.keysym.sym == SDLK_s) {
					nn.save("network.txt");
                }
                if (ev.key.keysym.sym == SDLK_l) {
					mtx.lock();
					nn.load("network.txt");
					mtx.unlock();
                }

                if (ev.key.keysym.sym == SDLK_ESCAPE) is_running = false;
				if (ev.key.keysym.sym == SDLK_SPACE) {
					nn.randomizeWeights(rand());
				}

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

