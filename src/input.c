#include "../include/input.h"

struct Keyboard keyboard;
struct Mouse mouse;

void clearInput(){
    memset(keyboard.keyJustPressed, 0, sizeof(keyboard.keyJustPressed));
    memset(keyboard.keyJustReleased, 0, sizeof(keyboard.keyJustReleased));
    memset(mouse.buttonJustPressed, 0, sizeof(mouse.buttonJustPressed));
    memset(mouse.buttonJustReleased, 0, sizeof(mouse.buttonJustReleased));
}

void eventKeyDown(SDL_Event event){
    keyboard.keyJustPressed[event.key.keysym.scancode] = 1;
    keyboard.keyPressed[event.key.keysym.scancode] = 1;
}

void eventKeyUp(SDL_Event event){
    keyboard.keyJustReleased[event.key.keysym.scancode] = 1;
    keyboard.keyPressed[event.key.keysym.scancode] = 0;
}

unsigned char isKeyJustPressed(SDL_Scancode scancode){
    return keyboard.keyJustPressed[scancode];
}

unsigned char isKeyPressed(SDL_Scancode scancode){
    return keyboard.keyPressed[scancode];
}

unsigned char isKeyJustReleased(SDL_Scancode scancode){
    return keyboard.keyJustReleased[scancode];
}

void eventMotion(SDL_Event event){
    mouse.position.x = event.motion.x;
    mouse.position.y = event.motion.y;
}

void eventButtonDown(SDL_Event event){
    mouse.buttonPressed[event.button.button] = 1;
    mouse.buttonJustPressed[event.button.button] = 1;
}

void eventButtonUp(SDL_Event event){
    mouse.buttonJustReleased[event.button.button] = 1;
    mouse.buttonPressed[event.button.button] = 0;
}

unsigned char isButtonJustPressed(unsigned char button){
    return mouse.buttonJustPressed[button];
}
unsigned char isButtonPressed(unsigned char button){
    return mouse.buttonPressed[button];
}
unsigned char isButtonJustReleased(unsigned char button){
    return mouse.buttonJustReleased[button];
}

SDL_Point* getMousePosition(){
    return &mouse.position;
}