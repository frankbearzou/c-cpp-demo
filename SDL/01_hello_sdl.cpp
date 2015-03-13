#include <iostream>
#include <SDL.h>

using namespace std;

int main(int argc, char *argv[])
{
	int result = 0;
	SDL_Window *window;
	SDL_Surface *screen_surface;


	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Hello SDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		cout << "SDL_CreateWindow error. " << SDL_GetError() << endl;
		return -1;
	}

	screen_surface = SDL_GetWindowSurface(window);
	if (screen_surface == NULL)
	{
		cout << "SDL_GetWindowSurface error. " << SDL_GetError() << endl;
		return -2;
	}

	result = SDL_FillRect(screen_surface, NULL, 
		SDL_MapRGB(screen_surface->format, 0xff, 0x00, 0x00));
	if (result == -1)
	{
		cout << "SDL_fillRect error. " << SDL_GetError() << endl;
		return -3;
	}

	result = SDL_UpdateWindowSurface(window);
	if (result == -1)
	{
		cout << "SDL_UpdateWindowSurface error. " << SDL_GetError() << endl;
		return -4;
	}

	SDL_Delay(3000);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
