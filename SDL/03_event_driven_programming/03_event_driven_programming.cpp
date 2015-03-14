#include <iostream>
#include <SDL.h>

using namespace std;

bool game_init(SDL_Window *&window, SDL_Surface *&screen_surface)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Hello SDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		cout << "SDL_CreateWindow error. " << SDL_GetError() << endl;
		return false;
	}

	screen_surface = SDL_GetWindowSurface(window);
	if (screen_surface == nullptr)
	{
		cout << "SDL_GetWindowSurface error. " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool load_image(SDL_Surface *&image_surface)
{
	image_surface = SDL_LoadBMP("img/hello.bmp");
	if (image_surface == nullptr)
	{
		cout << "SDL_LoadBMP error. " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool show_image(SDL_Window *&window,
	SDL_Surface *&screen_surface,
	SDL_Surface *image_surface)
{
	int result = 0;
	result = SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
	if (result == -1)
	{
		cout << "SDL_BlitSurface error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_UpdateWindowSurface(window);
	if (result == -1)
	{
		cout << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool game_close(SDL_Window *&window, SDL_Surface *&image_surface)
{
	SDL_FreeSurface(image_surface);
	image_surface = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();

	return true;
}

int main(int argc, char *argv[])
{
	int result = 0;
	SDL_Window *window;
	SDL_Surface *screen_surface;
	SDL_Surface *image_surface;
	SDL_Event event;
	bool is_running = true;

	memset(&event, 0, sizeof(event));

	// somebody may say, you exit without clean up, actually OS helps us clean up.
	if (!game_init(window, screen_surface))
	{
		return -1;
	}

	if (!load_image(image_surface))
	{
		return -2;
	}

	while (is_running)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
			}

			if (event.type == SDL_KEYDOWN)
			{
				cout << "SDL_KEYDOWN" << endl;
			}
		}
		
		show_image(window, screen_surface, image_surface);
	}
	

	

	game_close(window, image_surface);

	system("pause");
	return 0;
}