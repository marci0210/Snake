#pragma once
#include "Objects.h"

class Snake : public Objects
{
	unsigned direction; //W-0, A-1, S-1, D-3

	//Snake partjainak surfacei
	static SDL_Surface* snake_head;
	static SDL_Surface* snake_body;
	static SDL_Surface* snake_end;
	static SDL_Surface* snake_curve;

	//Snake partjainak texturai
	static SDL_Texture* snake_head_texture;
	static SDL_Texture* snake_body_texture;
	static SDL_Texture* snake_end_texture;
	static SDL_Texture* snake_curve_texture;
public:
	Snake(unsigned = 10, unsigned = 10); //Snake constructor

	static bool load(); //Betölti a snake surfaceket
	static void destroy(); //Törli a snake surfaceket
	
	static void set_texture(SDL_Renderer*);  //Betölti a snake textúrákat (surface inicializálása szükséges)
	static SDL_Texture* get_texture(unsigned); //Kikérhető egy snake partjának textúrája (0 -head, 1 - body, 2 - end, 3 - curve), def: NULL
	static void destroy_texture(); //Törli a snake textúrákat

	const unsigned getdirection() const; //Visszaadja a snake partjának irányát
	void setdirection(const unsigned); //Beállítja a snake partjának irányát
};

 

