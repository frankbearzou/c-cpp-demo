#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <SDL.h>

using namespace std;

bool game_init(SDL_Window *&window, SDL_Surface *&screen_surface)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Hello SDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
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

SDL_Surface* load_image(string image_name)
{
	SDL_Surface *image_surface;
	image_surface = SDL_LoadBMP(image_name.c_str());
	if (image_surface == nullptr)
	{
		cout << "SDL_LoadBMP error. " << SDL_GetError() << endl;
		return false;
	}

	return image_surface;
}

bool load_image_collections(map<string, SDL_Surface*> &image_collections)
{
	image_collections["press"] = load_image("img/press.bmp");
	if (image_collections["press"] == nullptr)
	{
		cout << "press." << SDL_GetError() << endl;
		return false;
	}

	image_collections["up"] = load_image("img/up.bmp");
	if (image_collections["up"] == nullptr)
	{
		cout << "up." << SDL_GetError() << endl;
		return false;
	}

	image_collections["down"] = load_image("img/down.bmp");
	if (image_collections["down"] == nullptr)
	{
		cout << "down." << SDL_GetError() << endl;
		return false;
	}

	image_collections["left"] = load_image("img/left.bmp");
	if (image_collections["left"] == nullptr)
	{
		cout << "left." << SDL_GetError() << endl;
		return false;
	}

	image_collections["right"] = load_image("img/right.bmp");
	if (image_collections["right"] == nullptr)
	{
		cout << "right." << SDL_GetError() << endl;
		return false;
	}

	return true;
}

bool show_image(SDL_Window *window,
	SDL_Surface *screen_surface,
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

// I don't know what SDL_FreeSurface does, else I may use unique_ptr
bool game_close(SDL_Window *&window, map<string, SDL_Surface*> &image_collections)
{
	for (auto &p : image_collections)
	{
		SDL_FreeSurface(p.second);
	}
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
	map<string, SDL_Surface*> image_collections;

	bool is_running = true;

	memset(&event, 0, sizeof(event));

	// somebody may say, you exit without clean up, actually OS helps us clean up.
	if (!game_init(window, screen_surface))
	{
		return -1;
	}

	if (!load_image_collections(image_collections))
	{
		return -2;
	}

	image_surface = image_collections["press"];

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
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					image_surface = image_collections["up"];
					break;
				case SDLK_DOWN:
					image_surface = image_collections["down"];
					break;
				case SDLK_LEFT:
					image_surface = image_collections["left"];
					break;
				case SDLK_RIGHT:
					image_surface = image_collections["right"];
					break;
				case SDLK_SPACE:
					image_surface = image_collections["press"];
					break;
				default:
					image_surface = image_collections["press"];
					break;
				}
			}
		}

		show_image(window, screen_surface, image_surface);
	}




	game_close(window, image_collections);

	system("pause");
	return 0;
}