#pragma once
#include <iostream>
#include "SDL.h"

class Objects
{
	SDL_Rect rectangle;
	bool breakable;

	//Wall, dinamit surfacei
	static SDL_Surface* wall;
	static SDL_Surface* dinamit;

	//Wall, dinamit textúrái
	static SDL_Texture* wall_texture;
	static SDL_Texture* dinamit_texture;

public:
	Objects(unsigned, unsigned, bool = true); //Objektum contructor

	void set(const unsigned, const unsigned);
	const unsigned get_x() const;
	const unsigned get_y() const;

	const SDL_Rect& get_sdl_render() const;

	static bool load(); //Betölti az objektumok surfaceit
	static void destroy(); //Törli az objektumok surfaceit

	static SDL_Texture* get_texture(const unsigned); //Visszadja az objektumok textúrákat (0 - wall, 1 - dinamit)
	static void set_texture(SDL_Renderer* renderer); //Beállítja az objektum textúrája, def: NULL
	static void destroy_texture();  //Törli az objektum textúrákat

	const bool get_breakable() const; //Visszaadja az objektum törhetőségét
};