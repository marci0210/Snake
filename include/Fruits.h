#pragma once
#include "Objects.h"
#pragma once
class Fruits : public Objects
{
	static unsigned point;

	//Fruits surface-ei
	static SDL_Surface* apple;
	static SDL_Surface* cherry;
	static SDL_Surface* banana;
	static SDL_Surface* pineapple;

	//Fruits textúrái
	static SDL_Texture* apple_texture;
	static SDL_Texture* cherry_texture;
	static SDL_Texture* banana_texture;
	static SDL_Texture* pineapple_texture;

public:
	Fruits(unsigned, unsigned, unsigned); //Fruits constructor

	static bool load(); //Betölti a fruits surfaceket
	static void destroy(); //Törli a fruits surfaceket

	static void set_texture(SDL_Renderer*); //Betölti a fruits textúrákat (surface iniciálizásása szükséges)
	static SDL_Texture* get_texture(unsigned); //Kikérhető egy fruits partjának textúrája (0 -apple, 1 - cherry, 2 - banana, 3 - pineapple), def: NULL
	static void destroy_texture(); //Törli a fruits textúrákat

	static const unsigned get_point(); //Visszaadja a fruits partjának értékét
	static void set_point(unsigned); //Beállítja a fruits partjának értékét
};

