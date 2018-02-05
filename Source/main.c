/*
 * main.c
 *
 *  Created on: 05/02/2018
 *      Author: Dylan
 */

#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

const int WIDTH = 800;
const int HEIGHT = 600;

void clearScr(SDL_Renderer *renderer, int r, int g, int b, int a)
{
	int success = SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if(success == 1)
	{
		printf("clear error! %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	success = SDL_RenderClear(renderer);

	if(success == 1)
	{
		printf("clear error! %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}
}

typedef struct droplet
{
	int r, g, b, a;
	int x, y;
	int size, maxVel;

	double velocity, acceleration;
} droplet;

int main (int argc, char* argv[])
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *win;

	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &renderer);

	if (!win || !renderer)
	{
		printf("SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	int dropCount = 1000;
	int tailLength = 2;
	droplet drops[dropCount];

	//Initializing drops
	for(int i = 0; i < dropCount; i++)
	{
		int y = -1 * (rand() % (100 + 1 - 50) + 50); //spawns drop off screen
		int x = rand() % ((WIDTH-1) + 1 - 0) + 0;

		double acc = rand() % (3 + 1 - 1) + 1;

		droplet drop;
		drop.r = 0;
		drop.g = 0;
		drop.b = 255;
		drop.a = 255;
		drop.x = x;
		drop.y = y;
		drop.size = rand() % (3 + 1 - 1) + 1;
		drop.velocity = 0;
		drop.acceleration = acc;
		drop.maxVel = rand() % (25 + 1 - 8) + 8;

		drops[i] = drop;
	}

	int run = 1;
	while(run == 1)
	{
		if(SDL_PollEvent(&event) == 1)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					run = 0;
					break;
			}
		}

		clearScr(renderer, 0, 0, 0, 255);

		for(int i = 0; i < dropCount; i++)
		{
			drops[i].velocity += (drops[i].acceleration * (1.0/drops[i].size));
			if(drops[i].velocity >= drops[i].maxVel) drops[i].velocity = drops[i].maxVel;

			drops[i].y += drops[i].velocity;
			int dropLength = tailLength*(drops[i].velocity) + (drops[i].size);

			if(drops[i].y >= HEIGHT-1+dropLength)
			{
				//RE-CREATE DROP AT THE TOP OF THE SCREEN
				int y = -1 * (rand() % (100 + 1 - 50) + 50); //spawns drop off screen
				int x = rand() % ((WIDTH-1) + 1 - 0) + 0;

				double acc = rand() % (3 + 1 - 1) + 1;

				drops[i].x = x;
				drops[i].y = y;
				drops[i].size = rand() % (3 + 1 - 1) + 1;
				drops[i].velocity = 0;
				drops[i].acceleration = acc;
			}
			else if(drops[i].y > 0)
			{
				//DRAW DROP
				SDL_SetRenderDrawColor(renderer, drops[i].r, drops[i].g, drops[i].b, drops[i].a);
				for(int x = drops[i].x; x < (drops[i].x + drops[i].size); x++)
				{
					SDL_RenderDrawLine(renderer, x, drops[i].y, x, drops[i].y-dropLength);
				}
			}
		}

		SDL_RenderPresent(renderer);

		SDL_Delay(20);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
