#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <ctime>

#define MAP_WIDTH 48
#define MAP_HEIGHT 48

int main(int argc, char** argv){
    unsigned char shouldClose = 0;

    int firstBoard[MAP_WIDTH][MAP_HEIGHT];
    int secondBoard[MAP_WIDTH][MAP_HEIGHT];

    int (*firstPointer)[MAP_HEIGHT] = firstBoard;
    int (*secondPointer)[MAP_HEIGHT] = secondBoard;

    srand(time(NULL));

    // Set up the intial board
    for(int x = 0; x < MAP_WIDTH; x++){
        for(int y = 0; y < MAP_HEIGHT; y++){
            if(rand() % 1){
                firstPointer[x][y] = 1;
            }
        }
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL couldn't start" << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetLogicalSize(renderer, MAP_WIDTH, MAP_HEIGHT);

    while(!shouldClose){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    shouldClose = 1;
            }
        }

        // Check every cell on the board 
        for(int x = 0; x < MAP_WIDTH; x++){            
            for(int y = 0; y < MAP_HEIGHT; y++){
                unsigned char surroundingCells = 0;
                // Check 8 surrounding cells
                for(int xx = -1; xx <= 1; xx++){
                    for(int yy = -1; yy <= 1; yy++){
                        if(xx == 0 && yy == 0) continue;
                        if(firstPointer[(xx + x) % MAP_WIDTH][(yy + y) % MAP_HEIGHT]) surroundingCells++;
                    }
                }
                secondPointer[x][y] = firstPointer[x][y] ? (surroundingCells >= 2 && surroundingCells < 4) : (surroundingCells == 3);
            }
        }

        // Swap arrays (double-buffering)
        int (*tempPointer)[MAP_HEIGHT] = firstPointer;
        firstPointer = secondPointer;
        secondPointer = tempPointer;

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int x = 0; x < MAP_WIDTH; x++){
            for(int y = 0; y < MAP_HEIGHT; y++){
                if(firstPointer[x][y]){
                    SDL_RenderDrawPoint(renderer, x, y);
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