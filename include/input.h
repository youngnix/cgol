#include <SDL2/SDL.h>

struct Keyboard {
    unsigned char keyJustPressed[512];
    unsigned char keyPressed[512];
    unsigned char keyJustReleased[512];
};

struct Mouse {
    SDL_Point position;

    unsigned char buttonJustPressed[4];
    unsigned char buttonPressed[4];
    unsigned char buttonJustReleased[4];
};

void clearInput();

void eventKeyDown(SDL_Event event);
void eventKeyUp(SDL_Event event);

unsigned char isKeyJustPressed(SDL_Scancode scancode);
unsigned char isKeyPressed(SDL_Scancode scancode);
unsigned char isKeyJustReleased(SDL_Scancode scancode);

void eventMotion(SDL_Event event);
void eventButtonDown(SDL_Event event);
void eventButtonUp(SDL_Event event);

unsigned char isButtonJustPressed(unsigned char button);
unsigned char isButtonPressed(unsigned char button);
unsigned char isButtonJustReleased(unsigned char button);

SDL_Point* getMousePosition();