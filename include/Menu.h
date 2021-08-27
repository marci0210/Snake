#pragma once
#include "SDL.h"
#include "Menu.h"
#include <iostream>

class Menu
{
	//Menu surfacei
	static SDL_Surface* wallpaper;
	static SDL_Surface* play_game;
	static SDL_Surface* exit;

	//Menu textúrái
	static SDL_Texture* wallpaper_texture;
	static SDL_Texture* play_game_texture;
	static SDL_Texture* exit_texture;
public:
	static bool load(); //Betölti a menu surfaceket
	static void destroy(); //Törli a menu surfaceket

	static void set_texture(SDL_Renderer*);  //Betölti a snake textúrákat a kapott rendererre(surface iniciálizásása szükséges)
	static SDL_Texture* get_texture(unsigned); //Visszaadja a megfelelo textúrát(0 - wallpaper, 1 - start_game, 2 - quit), def: NULL
	static void destroy_texture(); //Törli a menu textúrákat
};

