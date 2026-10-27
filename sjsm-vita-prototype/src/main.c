#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_W 960
#define SCREEN_H 544
#define MAX_DEPTH 12

static void line(SDL_Renderer *r, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
}

static void fill(SDL_Renderer *r, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(r, &rect);
}

static void draw_corridor(SDL_Renderer *r, float walk, float turn) {
    SDL_SetRenderDrawColor(r, 9, 9, 15, 255);
    SDL_RenderClear(r);

    /* Ceiling and floor planes */
    SDL_SetRenderDrawColor(r, 23, 22, 31, 255);
    fill(r, 0, 0, SCREEN_W, SCREEN_H / 2);
    SDL_SetRenderDrawColor(r, 37, 32, 35, 255);
    fill(r, 0, SCREEN_H / 2, SCREEN_W, SCREEN_H / 2);

    /* Perspective frame: intentionally simple while testing Vita rendering/input */
    for (int i = 0; i < MAX_DEPTH; ++i) {
        float t = (float)i / (float)MAX_DEPTH;
        float depth = fmodf((float)i + walk, (float)MAX_DEPTH) / (float)MAX_DEPTH;
        float scale = 1.0f - t * 0.91f;
        int half_w = (int)(SCREEN_W * 0.48f * scale);
        int half_h = (int)(SCREEN_H * 0.44f * scale);
        int cx = SCREEN_W / 2 + (int)(turn * 42.0f * scale);
        int cy = SCREEN_H / 2 + (int)(depth * 5.0f);
        Uint8 shade = (Uint8)(34 + (MAX_DEPTH - i) * 3);

        SDL_SetRenderDrawColor(r, shade, (Uint8)(shade - 3), (Uint8)(shade + 5), 255);
        line(r, cx - half_w, cy - half_h, cx + half_w, cy - half_h);
        line(r, cx - half_w, cy + half_h, cx + half_w, cy + half_h);
        line(r, cx - half_w, cy - half_h, cx - half_w, cy + half_h);
        line(r, cx + half_w, cy - half_h, cx + half_w, cy + half_h);

        if (i % 2 == 0) {
            SDL_SetRenderDrawColor(r, 47, 35, 40, 255);
            line(r, cx - half_w, cy, cx + half_w, cy);
        }
    }

    /* Small ceiling light at the end of the hall */
    SDL_SetRenderDrawColor(r, 184, 177, 150, 255);
    fill(r, SCREEN_W / 2 - 10 + (int)(turn * 35.0f), SCREEN_H / 2 - 90, 20, 5);
    SDL_SetRenderDrawColor(r, 68, 61, 52, 255);
    line(r, SCREEN_W / 2 - 22, SCREEN_H / 2 - 84, SCREEN_W / 2 + 22, SCREEN_H / 2 - 84);

    /* Minimal HUD */
    SDL_SetRenderDrawColor(r, 6, 6, 10, 210);
    fill(r, 16, 16, 220, 38);
    SDL_SetRenderDrawColor(r, 207, 202, 187, 255);
    for (int i = 0; i < 5; ++i) {
        line(r, 28, 28 + i * 4, 42 + (i % 2) * 5, 28 + i * 4);
    }
    for (int i = 0; i < 3; ++i) {
        SDL_Rect pip = {190 + i * 12, 27, 7, 14};
        SDL_RenderDrawRect(r, &pip);
    }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Spooky Corridor Vita - Prototype",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Quit();
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    SDL_GameController *pad = NULL;
    if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0)) {
        pad = SDL_GameControllerOpen(0);
    }

    bool running = true;
    float walk = 0.0f;
    float turn = 0.0f;
    Uint32 previous = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        Uint32 now = SDL_GetTicks();
        float dt = (float)(now - previous) / 1000.0f;
        previous = now;
        if (dt > 0.05f) dt = 0.05f;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float forward = 0.0f;
        float horizontal = 0.0f;
        if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) forward += 1.0f;
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) forward -= 1.0f;
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) horizontal -= 1.0f;
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) horizontal += 1.0f;

        if (pad) {
            Sint16 ly = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);
            Sint16 lx = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
            if (abs(ly) > 7000) forward = -(float)ly / 32768.0f;
            if (abs(lx) > 7000) horizontal = (float)lx / 32768.0f;
            if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START)) running = false;
        }

        walk += forward * dt * 2.4f;
        turn += horizontal * dt * 2.1f;
        if (turn > 1.0f) turn = 1.0f;
        if (turn < -1.0f) turn = -1.0f;

        draw_corridor(renderer, walk, turn);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (pad) SDL_GameControllerClose(pad);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
