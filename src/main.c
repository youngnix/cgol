#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Shit happens. SDL error: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("C's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* bitmapImage = SDL_LoadBMP("res/board.bmp");

    if(bitmapImage == NULL){
        printf("Couldn't load bitmap. SDL error: %s\n", SDL_GetError());
    }

    const unsigned int MAP_WIDTH = bitmapImage->w;
    const unsigned int MAP_HEIGHT = bitmapImage->h;

    unsigned char frontBuffer[MAP_HEIGHT][MAP_WIDTH];
    unsigned char backBuffer[MAP_HEIGHT][MAP_WIDTH];

    memcpy(frontBuffer, bitmapImage->pixels, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));

    unsigned char drawGrid = 1;
    unsigned char boardPaused = 0;

    SDL_Point mousePosition;
    unsigned char pressedButtons[4];
    unsigned char pressedKeys[512];

    unsigned char shouldClose = 0;
    while(!shouldClose){
        memset(pressedKeys, 0, sizeof(pressedKeys));
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    if(!event.key.repeat){
                        pressedKeys[event.key.keysym.scancode] = 1;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pressedButtons[event.button.button] = 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    pressedButtons[event.button.button] = 0;
                    break;
                case SDL_MOUSEMOTION:
                    mousePosition.x = event.motion.x;
                    mousePosition.y = event.motion.y;
                    break;
                case SDL_QUIT:
                    shouldClose = 1;
                    break;
            }
        }

        if(pressedKeys[SDL_SCANCODE_C]){
            memset(frontBuffer, 0, sizeof(frontBuffer));
        }

        if(pressedKeys[SDL_SCANCODE_S]){
            SDL_LockSurface(bitmapImage);
            memcpy(bitmapImage->pixels, frontBuffer, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));
            SDL_SaveBMP(bitmapImage, "res/boardSave.bmp");
            SDL_UnlockSurface(bitmapImage);
        }
        
        if(pressedKeys[SDL_SCANCODE_R]){
            if(pressedKeys[SDL_SCANCODE_LSHIFT]){
                bitmapImage = SDL_LoadBMP("res/boardSave.bmp");
                if(bitmapImage != NULL){
                    memcpy(frontBuffer, bitmapImage->pixels, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));
                }
            }
            else {
                bitmapImage = SDL_LoadBMP("res/board.bmp");
                if(bitmapImage != NULL){
                    memcpy(frontBuffer, bitmapImage->pixels, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));
                }
            }
        }

        if(pressedKeys[SDL_SCANCODE_SPACE]){
            boardPaused = !boardPaused;
        }

        if(pressedKeys[SDL_SCANCODE_G]){
            drawGrid = !drawGrid;
        }

        // If the board isn't paused, run the simulation
        if(!boardPaused){
            // Check every cell on the board 
            for(int i = 0; i < MAP_HEIGHT; i++){            
                for(int j = 0; j < MAP_WIDTH; j++){
                    unsigned char cellCount = 0;
                    // Check 8 surrounding cells
                    for(int ii = i - 1; ii <= i + 1; ii++){
                        for(int jj = j - 1; jj <= j + 1; jj++){
                            if(jj < 0 || jj >= MAP_WIDTH) continue;
                            if(ii < 0 || ii >= MAP_HEIGHT) continue;
                            if(jj == j && ii == i) continue;
                            if(frontBuffer[ii][jj]) cellCount++;
                        }
                    }
                    backBuffer[i][j] = frontBuffer[i][j] ? (cellCount >= 2 && cellCount < 4) : (cellCount == 3);
                }
            }
            // Copy the back buffer to the front buffer
            memcpy(frontBuffer, backBuffer, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));   
        }

        if((mousePosition.x < MAP_WIDTH * 8 && mousePosition.x > 0)
        && (mousePosition.y < MAP_HEIGHT * 8 && mousePosition.y > 0)){
            if(pressedButtons[SDL_BUTTON_LEFT]){
                frontBuffer[mousePosition.y / 8][mousePosition.x / 8] = 1;
            }

            if(pressedButtons[SDL_BUTTON_RIGHT]){
                frontBuffer[mousePosition.y / 8][mousePosition.x / 8] = 0;
            }
        }

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
        // Draw cells
        for(int i = 0; i < MAP_HEIGHT; i++){
            for(int j = 0; j < MAP_WIDTH; j++){
                SDL_Rect cellRect = { j * 8, i * 8, 8, 8 };
                if(frontBuffer[i][j]) SDL_RenderFillRect(renderer, &cellRect);
            }
        }

        // If drawGrid is true, draw the grid
        if(drawGrid){
            SDL_SetRenderDrawColor(renderer, 64, 64, 64, 64);

            for(int i = 1; i < MAP_HEIGHT; i++){
                SDL_RenderDrawLine(renderer, 0, i * 8, MAP_WIDTH * 8, i * 8);
                SDL_RenderDrawLine(renderer, i * 8, 0, i * 8, MAP_HEIGHT * 8);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(bitmapImage);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}
