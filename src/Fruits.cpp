#include "Fruits.h"

Fruits::Fruits(unsigned x, unsigned y, unsigned point) : Objects(x, y, true){};

//Betölti a bmp fájlokat a surfacere
bool Fruits::load() {
	if (!(apple = SDL_LoadBMP("texture/apple.bmp"))) return false;
	if (!(cherry = SDL_LoadBMP("texture/cherry.bmp"))) return false;
	if (!(pineapple = SDL_LoadBMP("texture/pineapple.bmp"))) return false;
	if (!(banana = SDL_LoadBMP("texture/banana.bmp"))) return false;

	return true;
}
//Törli a betöltitt bmp fájlokat a surfaceről
void Fruits::destroy() {
	SDL_FreeSurface(apple);
	SDL_FreeSurface(cherry);
	SDL_FreeSurface(pineapple);
	SDL_FreeSurface(banana);

	Fruits::apple = NULL;
	Fruits::cherry = NULL;
	Fruits::banana = NULL;
	Fruits::pineapple = NULL;
}
//Betölti a textúrát a kapott rendererre a surfaceről
void Fruits::set_texture(SDL_Renderer* renderer)
{
	apple_texture = SDL_CreateTextureFromSurface(renderer, apple);
	cherry_texture = SDL_CreateTextureFromSurface(renderer, cherry);
	banana_texture = SDL_CreateTextureFromSurface(renderer, banana);
	pineapple_texture = SDL_CreateTextureFromSurface(renderer, pineapple);
}
//Visszaad egy Fruit textúrát
SDL_Texture* Fruits::get_texture(unsigned a)
{
	switch (a) {
	case 0:
		return apple_texture;
	case 1:
		return cherry_texture;
	case 2:
		return banana_texture;
	case 3:
		return pineapple_texture;
	default:
		return NULL;
	}

}
//Törli a betöltott textúrákat
void Fruits::destroy_texture()
{
	SDL_DestroyTexture(apple_texture);
	SDL_DestroyTexture(cherry_texture);
	SDL_DestroyTexture(banana_texture);
	SDL_DestroyTexture(pineapple_texture);

	Fruits::apple_texture = NULL;
	Fruits::cherry_texture = NULL;
	Fruits::banana_texture = NULL;
	Fruits::pineapple_texture = NULL;
}

const unsigned Fruits::get_point()
{
	return point;
}
void Fruits::set_point(unsigned a)
{
	point = a;
}
//Statikus változók inicializálása (szükséges)
SDL_Surface* Fruits::apple = NULL;
SDL_Surface* Fruits::cherry = NULL;
SDL_Surface* Fruits::banana = NULL;
SDL_Surface* Fruits::pineapple = NULL;

SDL_Texture* Fruits::apple_texture = NULL;
SDL_Texture* Fruits::cherry_texture = NULL;
SDL_Texture* Fruits::banana_texture = NULL;
SDL_Texture* Fruits::pineapple_texture = NULL;

unsigned Fruits::point = 0;