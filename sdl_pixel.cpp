#include <SDL.h>
#include <iostream>
#include <cmath>

const short particle_size = 3;
const int repel_distance = 30, distance = 150; const double min_distance = 0.01;
bool simulate = false;
int relation_type = 2;
int placement_type[2] = {2,6};
const bool teleport = false;
const double speed = 0.000001, repel_speed = 0.001, rp2 = 100;
const int N = 2000;
double relations[6][6];
const int With = 800, Height = 800, max_x = With - 1, max_y = Height - 1;


void placement(int x, short variety);

class particle {
public:
    double x=0, y=0;
    short colour = 0;
    double v[2] = { 0,0 };

    void calc_v(particle* p, double relation) {
        double dx = p->x - x, dy = p->y - y;
        /*double sqr_d = abs(dx * dx + dy * dy);*/
        double d = sqrt((dx * dx) + (dy * dy));
        /*if (!d and p != this) {
            v[0] += (rand() % 201-100) / 100.0 * rp2;
            v[1] += (rand() % 201-100) / 100.0 * rp2;
        }*/
        if (d < repel_distance) {
            v[0] -= dx * (repel_distance - d) * repel_speed;
            v[1] -= dy * (repel_distance - d) * repel_speed;
            /*double factor = (d / (repel_distance - d)) * repel_speed;
            v[0] -= dx * factor;
            v[1] -= dy * factor;*/
        }
        else if (d < distance + repel_distance) {
            double factor = distance / 2 - abs(repel_distance + distance / 2 + d);
            factor = -factor * speed * relation;
            v[0] += dx * factor;
            v[1] += dy * factor;
        }
    }
    void move_old() {
        x += v[0]; y += v[1];
        v[0] = 0; v[1] = 0;
        if (x < 0) x += max_x;
        else if (x > max_x) x -= max_x;
        if (y < 0) y += max_y;
        else if (y > max_y) y -= max_y;
    }
};
void move(particle& p);
void plot(particle* p);
void relation(int type);
particle particles[N];
SDL_Rect rect;
SDL_Renderer* renderer = NULL; 
particle* board[With][Height]{};

int main(int argc, char** argv)
{
	placement(placement_type[0], placement_type[1]);
	relation(relation_type);

    rect.h = particle_size; rect.w = particle_size;
    
    SDL_Window* window = NULL;
    window = SDL_CreateWindow
    (
        "punkty", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        With,
        Height,
        SDL_WINDOW_SHOWN
    );

    // Setup renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    bool is_running = true;
    SDL_Event ev;
    while (is_running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT)
                is_running = false;
            if (ev.type == SDL_KEYDOWN and ev.key.keysym.sym == SDLK_SPACE and !simulate){
                simulate = true;
            }
            else if (ev.type == SDL_KEYDOWN and ev.key.keysym.sym == SDLK_SPACE and simulate) {
                simulate = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        for (int i = 0; i < N; i++) {
            plot(&particles[i]); 
            if (simulate) {
                /*particles[i].move_old();*/
                move(particles[i]);
                for (int j = 0; j < N; j++)
                    particles[i].calc_v(&particles[j], relations[particles[i].colour][particles[j].colour]);
            }
        }
        //SDL_Delay(100);
            
        

        SDL_RenderPresent(renderer);


    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void placement(int x = 0, short variety = 6) {
    int colour; double alpha = 0, phi = 0.61803398875;
    double pi = 3.14159265359, tmp;
    switch (x) {
    case 0:
        for (int i = 0; i < N; i++) {
            colour = variety - 1 - (i * variety) / N;
            particles[i].x = With / 2 + sin(alpha) * sqrt(i) * 7;
            particles[i].y = Height / 2 + cos(alpha) * sqrt(i) * 7;
            particles[i].colour = colour;
            alpha += 2 * pi * phi;
            if (alpha > 2 * pi) alpha -= 2 * pi;
        }
        break;
    case 1:
        for (int i = 0; i < N; i++) {
            particles[i].x = rand() % With;
            particles[i].y = rand() % Height;
            particles[i].colour = rand() % variety;
        }
        break;
    case 2:
        for (int i = 0; i < N; i++) {
            tmp = alpha / (2 * pi / variety);
            colour = (int)tmp;
            particles[i].x = With / 2 + sin(alpha) * sqrt(i) * 7;
            particles[i].y = Height / 2 + cos(alpha) * sqrt(i) * 7;
            particles[i].colour = colour;
            alpha += 2 * pi * phi;
            if (alpha > 2 * pi) alpha -= 2 * pi;
        }
        break;
    case 3:
        for (int i = 0; i < N; i++) {
            colour = variety - 1 - (i * variety) / N;
            particles[i].x = With / 2 + sin(alpha) * sqrt(i) * 7;
            particles[i].y = Height / 2 + cos(alpha) * sqrt(i) * 7;
            particles[i].colour = colour;
            alpha += 2 * pi * phi;
            if (alpha > 2 * pi) alpha -= 2 * pi;
        }
        break;
    }
}

void move(particle& p) {
    double tx = p.x;
    double ty = p.y;
    p.x += p.v[0]; p.y += p.v[1];

    
    p.v[0] = 0; p.v[1] = 0;

    if (p.x < 0) p.x += max_x;
    else if (p.x > max_x) p.x -= max_x;
    if (p.y < 0) p.y += max_y;
    else if (p.y > max_y) p.y -= max_y;
    for (int i = 0; i < N; i++) 
        if ((abs(p.x - particles[i].x) < min_distance or
            abs(p.y - particles[i].y) < min_distance) and
            (&particles[i] != &p)) {
            p.x = tx; p.y = ty;
            //std::cout << abs(p.x - particles[i].x) << " " << abs(p.y - particles[i].y) << std::endl;
            return;
        }
}

void plot(particle* p) {
    rect.x = p->x - (particle_size / 2);
    rect.y = p->y - (particle_size / 2);
    switch (p->colour) {
    case 0:
        SDL_SetRenderDrawColor(renderer, 3  , 181, 255, 255); break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 0  , 255, 0  , 255); break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 200, 0  , 255, 255); break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 255, 0  , 0  , 255); break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0  , 255); break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 255, 155, 0  , 255); break;
    }
    SDL_RenderFillRect(renderer, &rect);

}

void relation(int type) {
    
        double relationstmp[6][6]{
        { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},//b
        { 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},//g
        { 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},//v
        { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},//r
        { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},//y
        { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp1[6][6]{
        {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0},//b
        { 0.0,-1.0, 0.0, 0.0, 0.0, 0.0},//g
        { 0.0, 0.0,-1.0, 0.0, 0.0, 0.0},//v
        { 0.0, 0.0, 0.0,-1.0, 0.0, 0.0},//r
        { 0.0, 0.0, 0.0, 0.0,-1.0, 0.0},//y
        { 0.0, 0.0, 0.0, 0.0, 0.0,-1.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp2[6][6]{
        { 1.0, 1.0, 0.0, 0.0, 0.0, 0.0},//b
        { 0.0, 1.0, 1.0, 0.0, 0.0, 0.0},//g
        { 0.0, 0.0, 1.0, 1.0, 0.0, 0.0},//v
        { 0.0, 0.0, 0.0, 1.0, 1.0, 0.0},//r
        { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0},//y
        { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0} //o
        //b    g    v    r    y    o
        };
        double relationstmp3[6][6]{
        { 1.0, 1.0, 0.0, 0.0, 0.0,-1.0},//b
        {-1.0, 1.0, 1.0, 0.0, 0.0, 0.0},//g
        { 0.0,-1.0, 1.0, 1.0, 0.0, 0.0},//v
        { 0.0, 0.0,-1.0, 1.0, 1.0, 0.0},//r
        { 0.0, 0.0, 0.0,-1.0, 1.0, 1.0},//y
        { 1.0, 0.0, 0.0, 0.0,-1.0, 1.0} //o
        //b    g    v    r    y    o
        };
        double relationstmp4[6][6]{
        { 0.1, 1.0, 0.0, 0.0, 0.0,-1.0},//b
        {-1.0,-1.0, 1.0, 0.0, 0.0, 0.0},//g
        { 0.0,-1.0,-1.0, 1.0, 0.0, 0.0},//v
        { 0.0, 0.0,-1.0,-1.0, 1.0, 0.0},//r
        { 0.0, 0.0, 0.0,-1.0,-1.0, 1.0},//y
        { 1.0, 0.0, 0.0, 0.0,-1.0,-1.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp5[6][6]{
        { 1.0,-1.0,-1.0, 1.0, 0.5, 1.0},//b
        { 1.0, 1.0, 1.0, 1.0, 1.0, -0.1},//g
        {-0.1,-0.5,-1.0,-1.0, 1.0,-1.0},//v
        { 0.0,-1.0, 1.0,-1.0, 0.8, 0.0},//r
        { 1.0,-1.0,-1.0,-1.0,-1.0,-1.0},//y
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp6[6][6]{
        { 0.0, 0.5, 0.0, 0.0, 0.0, 0.0},//b
        {-0.5, 0.0, 0.5, 0.0, 0.0, 0.0},//g
        { 0.0, 0.0, 1.0, 1.0, 0.0, 0.0},//v
        { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},//r
        { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},//y
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp7[6][6]{
        { 0.0, 1.0, 0.0, 0.0, 0.0,-1.0},//b
        {-1.0, 0.0, 1.0, 0.0, 0.0, 0.0},//g
        { 0.0,-1.0, 0.0, 1.0, 0.0, 0.0},//v
        { 0.0, 0.0,-1.0, 0.0, 1.0, 0.0},//r
        { 0.0, 0.0, 0.0,-1.0, 0.0, 1.0},//y
        { 1.0, 0.0, 0.0, 0.0,-1.0, 0.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp8[6][6]{
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//b
        {-1.0, 0.0, 0.1, 0.0, 0.0, 0.0},//g
        {-1.0, 1.0, 0.0,-1.0, 0.0, 0.0},//v
        {-1.0, 0.0,-0.1, 0.0, 0.1, 0.0},//r
        {-1.0, 0.0, 0.0,-0.5, 0.0, 0.5},//y
        {-1.0, 0.0, 0.0, 0.0,-0.5, 1.0} //o
        //b    g    v    r    y    o
        }; 
        double relationstmp9[6][6]{
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},//b
        { 1.0, 1.0, 1.0, 0.0, 0.0, 1.0},//g
        { 1.0, 1.0, 1.0, 1.0, 0.0, 1.0},//v
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},//r
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},//y
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0} //o
        //b    g    v    r    y    o
        }; 
        switch (type) {
        case 0:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp[i][j]; break;
        case 1:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp1[i][j]; break;
        case 2:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp2[i][j]; break;
        case 3:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp3[i][j]; break;
        case 4:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp4[i][j]; break;
        case 5:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp5[i][j]; break;
        case 6:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp6[i][j]; break;
        case 7:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp7[i][j]; break;
        case 8:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp8[i][j]; break;
        case 9:
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++) relations[i][j] = relationstmp9[i][j]; break;
    }
}