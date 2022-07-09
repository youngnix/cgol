#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Shit happens. SDL error: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("C's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface* boardSurface = SDL_LoadBMP("res\\board.bmp");

    const unsigned int MAP_WIDTH = boardSurface->w;
    const unsigned int MAP_HEIGHT = boardSurface->h;

    unsigned char firstBoard[MAP_WIDTH][MAP_HEIGHT];
    unsigned char secondBoard[MAP_WIDTH][MAP_HEIGHT];

    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            secondBoard[i][j] = ((unsigned char*)boardSurface->pixels)[(i * MAP_WIDTH) + j];
        }
    }
    memcpy(firstBoard, secondBoard, MAP_HEIGHT * MAP_WIDTH * sizeof(unsigned char));

    SDL_RenderSetLogicalSize(renderer, MAP_WIDTH, MAP_HEIGHT);

    unsigned char shouldClose = 0;
    while(!shouldClose){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    shouldClose = 1;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                        SDL_LockSurface(boardSurface);
                        for(int i = 0; i < MAP_HEIGHT; i++){
                            for(int j = 0; j < MAP_WIDTH; j++){
                                ((unsigned char*)boardSurface->pixels)[(i * MAP_WIDTH) + j] = firstBoard[i][j];
                            }
                        }
                        SDL_SaveBMP(boardSurface, "res\\boardSave.bmp");
                        SDL_UnlockSurface(boardSurface);
                    }
                    break;
            }
        }

        // Check every cell on the board 
        for(int i = 0; i < MAP_HEIGHT; i++){            
            for(int j = 0; j < MAP_WIDTH; j++){
                unsigned char surroundingCells = 0;
                // Check 8 surrounding cells
                for(int ii = -1; ii <= 1; ii++){
                    for(int jj = -1; jj <= 1; jj++){
                        if(jj == 0 && ii == 0) continue;
                        if(firstBoard[(ii + i) % MAP_HEIGHT][(jj + j) % MAP_WIDTH]) surroundingCells++;
                    }
                }
                secondBoard[i][j] = firstBoard[i][j] ? (surroundingCells >= 2 && surroundingCells < 4) : (surroundingCells == 3);
            }
        }

        memcpy(firstBoard, secondBoard, MAP_WIDTH * MAP_HEIGHT * sizeof(unsigned char));

        SDL_RenderClear(renderer);

        // Draw points
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int i = 0; i < MAP_HEIGHT; i++){
            for(int j = 0; j < MAP_WIDTH; j++){
                if(firstBoard[i][j]){
                    SDL_RenderDrawPoint(renderer, j, i);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
    }
    SDL_FreeSurface(boardSurface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}