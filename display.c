#include "display.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

BYTE init_disp(disp *d)
{
    //initialise SDL
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Could not initialise SDL\n");
        return 0;
    }

    //window and renderer creation
    d->window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, DISP_WIDTH * CELL_SIZE,
                                DISP_HEIGHT * CELL_SIZE, 0);

    if(!d->window)
    {
        printf("Could not create a SDL window\n");
        return 0;
    }

    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED);

    if(!d->renderer)
    {
        printf("Could not create a SDL renderer\n");
        return 0;
    }

    //set the position of each cell in the map and initialise gfx
    for(BYTE i = 0; i < DISP_HEIGHT; i++)
    {
        for(BYTE j = 0; j < DISP_WIDTH; j++)
        {
            uint32_t index = i * DISP_WIDTH + j;

            d->gfx[index] = 0;

            d->map[index].x = j * CELL_SIZE;
            d->map[index].y = i * CELL_SIZE;
            d->map[index].w = CELL_SIZE;
            d->map[index].h = CELL_SIZE;
        }
    }

    return 1;
}

void disp_events(BYTE *running)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        //close window
        if(event.type == SDL_QUIT)
        {
            *running = 0;
            break;
        }
    }
}

void render(disp *d)
{
    //set background color and clear screen
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);

    //render the map
    for(uint32_t i = 0; i < DISP_WIDTH * DISP_HEIGHT; i++)
    {
        //if the cell is on, render it white
        if(d->gfx[i] == 1)
        {
            SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(d->renderer, &d->map[i]);
        }
        else
        {
            SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(d->renderer, &d->map[i]);
        }
    }

    SDL_RenderPresent(d->renderer);
}

void disp_close(disp *d)
{
    SDL_DestroyWindow(d->window);
    SDL_DestroyRenderer(d->renderer);
}
