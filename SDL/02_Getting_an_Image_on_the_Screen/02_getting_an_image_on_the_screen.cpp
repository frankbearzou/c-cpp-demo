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

bool show_begin_scene(SDL_Window *window, 
	SDL_Surface *screen_surface, 
	SDL_Surface *image_surface)
{
	int result = 0;

	result = SDL_FillRect(image_surface, NULL,
		SDL_MapRGB(image_surface->format, 0x00, 0x00, 0x00));
	if (result == -1)
	{
		cout << "SDL_fillRect error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
	if (result == -1)
	{
		cout << "SDL_BlitSurface error.	" << SDL_GetError() << endl;
		return false;
	}

	result = SDL_UpdateWindowSurface(window);
	if (result == -1)
	{
		cout << "SDL_UpdateWindowSurface error. " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool show_image_scene(SDL_Window *window,
	SDL_Surface *screen_surface,
	SDL_Surface *&image_surface)
{
	int result = 0;

	image_surface = SDL_LoadBMP("img/hello.bmp");
	if (image_surface == nullptr)
	{
		cout << "SDL_LoadBMP error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
	if (result == -1)
	{
		cout << "SDL_BlitSurface error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_UpdateWindowSurface(window);
	if (result == -1)
	{
		cout << "SDL_UpdateWindowSurface error. " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool show_end_scene(SDL_Window *window,
	SDL_Surface *screen_surface,
	SDL_Surface *image_surface)
{
	int result = 0;
	result = SDL_FillRect(image_surface, nullptr, 
		SDL_MapRGB(image_surface->format, 0xff, 0xff, 0xff));
	if (result == -1)
	{
		cout << "SDL_FillRect error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_BlitSurface(image_surface, nullptr, screen_surface, nullptr);
	if (result == -1)
	{
		cout << "SDL_BlitSurface error. " << SDL_GetError() << endl;
		return false;
	}

	result = SDL_UpdateWindowSurface(window);
	if (result == -1)
	{
		cout << "SDL_UpdateWindowSurface error. " << SDL_GetError() << endl;
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

	// somebody may say, you exit without clean up, actually OS helps us clean up.
	if (!game_init(window, screen_surface))
	{
		return -1;
	}

	if (!load_image(image_surface))
	{
		return -2;
	}


	if (!show_begin_scene(window, screen_surface, image_surface))
	{
		return -3;
	}
	
	SDL_Delay(3000);

	if (!show_image_scene(window, screen_surface, image_surface))
	{
		return -4;
	}

	SDL_Delay(3000);

	if (!show_end_scene(window, screen_surface, image_surface))
	{
		return -5;
	}

	SDL_Delay(3000);

	game_close(window, image_surface);

	system("pause");
	return 0;
}