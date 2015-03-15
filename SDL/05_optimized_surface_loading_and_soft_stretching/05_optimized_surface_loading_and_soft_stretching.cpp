#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <SDL.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool game_init(SDL_Window *&window)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Hello SDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		cout << "SDL_CreateWindow error. " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

SDL_Surface* load_image(SDL_Window * window)
{
	SDL_Surface *temp_surface = nullptr;
	SDL_Surface *screen_surface = nullptr;
	SDL_Surface *image_surface = nullptr;

	screen_surface = SDL_GetWindowSurface(window);
	if (screen_surface == nullptr)
	{
		cout << "SDL_GetWindowSurface error. " << SDL_GetError() << endl;
		return nullptr;
	}

	temp_surface = SDL_LoadBMP("img/stretch.bmp");
	if (temp_surface == nullptr)
	{
		cout << "SDL_LoadBMP error. " << SDL_GetError() << endl;
		return nullptr;
	}

	image_surface = SDL_ConvertSurface(temp_surface, screen_surface->format, 0);
	if (image_surface == nullptr)
	{
		cout << "SDL_ConvertSurface error. " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_FreeSurface(temp_surface);

	return image_surface;
}

/*  
	you can change window size if you want
	program will repaint if window size changed.
*/
bool show_image(SDL_Window *window, SDL_Surface *image_surface)
{
	int result = 0;
	SDL_Surface *screen_surface = nullptr;
	
	screen_surface = SDL_GetWindowSurface(window);

	SDL_Rect dstrect{ 0, 0, screen_surface->w, screen_surface->h };

	result = SDL_BlitScaled(image_surface, nullptr, screen_surface, &dstrect);
	if (result == -1)
	{
		cout << "SDL_BlitScaled error. " << SDL_GetError() << endl;
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
	SDL_Surface *image_surface;
	SDL_Event event;

	bool is_running = true;

	// somebody may say, you exit without clean up, actually OS helps us clean up.
	if (!game_init(window))
	{
		return -1;
	}

	image_surface = load_image(window);
	if (image_surface == nullptr)
	{
		cout << "load_image error. " << SDL_GetError() << endl;
		system("pause");
	}

	while (is_running)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
			}
		}
		

		show_image(window, image_surface);
	}

	game_close(window, image_surface);

	system("pause");
	return 0;
}
