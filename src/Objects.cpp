#include <iostream>

#include "Objects.h"

Objects::Objects(unsigned a, unsigned b, bool alma)
{
	rectangle.x = a * 40;
	rectangle.y = b * 40;

	rectangle.w = 40;
	rectangle.h = 40;

	breakable = alma;
}
//Beállítja az objektum x, y koordinátáját
void Objects::set(unsigned x, unsigned y) {
	rectangle.x = x * 40;
	rectangle.y = y * 40;
}
//Visszaadja az objektum x, illetve y koordinátáját
const unsigned Objects::get_x() const {
	return rectangle.x / 40;
}
const unsigned Objects::get_y() const {
	return rectangle.y / 40;
}

//Visszaadja az objektum rectangle-ét
const SDL_Rect& Objects::get_sdl_render() const
{
	return rectangle;
}

//Betölti a bmp fájlokat a surfacere
bool Objects::load()
{
	if (!(wall = SDL_LoadBMP("texture/wall.bmp"))) return false;
	if (!(dinamit = SDL_LoadBMP("texture/dinamit.bmp"))) return false;

	return true;
}
//Törli a betöltitt bmp fájlokat a surfaceről
void Objects::destroy()
{
	SDL_FreeSurface(wall);
	SDL_FreeSurface(dinamit);

	Objects::wall = NULL;
	Objects::dinamit = NULL;
}
//Visszaadja a snake egy partját
SDL_Texture* Objects::get_texture(unsigned a)
{
	switch (a) {
	case 0:
		return wall_texture;
	case 1:
		return dinamit_texture;
	default:
		return NULL;
	}
}
//Betölti a textúrát a kapott rendererre a surfaceről
void Objects::set_texture(SDL_Renderer* renderer)
{
	wall_texture = SDL_CreateTextureFromSurface(renderer, wall);
	dinamit_texture = SDL_CreateTextureFromSurface(renderer, dinamit);
}
//Törli a betöltott textúrákat
void Objects::destroy_texture()
{
	SDL_DestroyTexture(wall_texture);
	SDL_DestroyTexture(dinamit_texture);

	Objects::wall_texture = NULL;
	Objects::dinamit_texture = NULL;
}
//Visszaadja az objektum törhetőségét
const bool Objects::get_breakable() const 
{
	return breakable;
}

//Statikus változók inicializása
SDL_Surface* Objects::wall = NULL;
SDL_Surface* Objects::dinamit = NULL;

SDL_Texture* Objects::wall_texture = NULL;
SDL_Texture* Objects::dinamit_texture = NULL;
