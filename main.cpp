#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <stdio.h>
#include <typeinfo>

int main(int argc, char** argv){
    srand(time(NULL));
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL couldn't start" << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface* boardSurface = SDL_LoadBMP("res\\board.bmp");

    const unsigned int mapWidth = (unsigned int)boardSurface->w;
    const unsigned int mapHeight = (unsigned int)boardSurface->h;

    unsigned char firstBoard[mapWidth][mapHeight];
    unsigned char secondBoard[mapWidth][mapHeight];

    unsigned char (*firstPointer)[mapHeight] = firstBoard;
    unsigned char (*secondPointer)[mapHeight] = secondBoard;

    for(int i = 0; i <= mapHeight; i++){
        for(int j = 0; j <= mapWidth; j++){
            break;
        }
    }

    SDL_RenderSetLogicalSize(renderer, mapWidth, mapHeight);

    bool shouldClose = 0;
    while(!shouldClose){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    shouldClose = 1;
            }
        }

        // Check every cell on the board 
        for(int i = 0; i < mapHeight; i++){            
            for(int j = 0; j < mapWidth; j++){
                unsigned char surroundingCells = 0;
                // Check 8 surrounding cells
                for(int ii = -1; ii <= 1; ii++){
                    for(int jj = -1; jj <= 1; jj++){
                        if(jj == 0 && ii == 0) continue;
                        if(firstPointer[(ii + i) % mapHeight][(jj + j) % mapWidth]) surroundingCells++;
                    }
                }
                secondPointer[i][j] = firstPointer[i][j] ? (surroundingCells >= 2 && surroundingCells < 4) : (surroundingCells == 3);
            }
        }

        // Swap arrays (double-buffering)
        unsigned char (*tempPointer)[mapHeight] = firstPointer;
        firstPointer = secondPointer;
        secondPointer = tempPointer;

        SDL_RenderClear(renderer);

        // Draw points
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int i = 0; i < mapHeight; i++){
            for(int j = 0; j < mapWidth; j++){
                if(firstPointer[i][j]){
                    SDL_RenderDrawPoint(renderer, j, i);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}