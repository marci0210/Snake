#include "Snake.h"

Snake::Snake(unsigned x, unsigned y) : Objects(x, y, false) {}; 
//Visszaadja a snakepart aktuális irányát
const unsigned Snake::getdirection() const
{
	return direction;
}
//Beállítja a snakepart új riányát
void Snake::setdirection(const unsigned a)
{
	direction = a;
}
//Betölti a bmp fájlokat a surfacere
bool Snake::load() {
	if (!(snake_head = SDL_LoadBMP("texture/snake_head.bmp"))) return false;
	if (!(snake_body = SDL_LoadBMP("texture/snake_body.bmp"))) return false;
	if (!(snake_end = SDL_LoadBMP("texture/snake_end.bmp"))) return false;
	if (!(snake_curve = SDL_LoadBMP("texture/snake_curve_new.bmp"))) return false;

	return true;
}
//Törli a betöltitt bmp fájlokat a surfaceről
void Snake::destroy() {
	SDL_FreeSurface(snake_head);
	SDL_FreeSurface(snake_body);
	SDL_FreeSurface(snake_end);
	SDL_FreeSurface(snake_curve);

	Snake::snake_body = NULL;
	Snake::snake_head = NULL;
	Snake::snake_end = NULL;
	Snake::snake_curve = NULL;
}
//Betölti a textúrát a kapott rendererre a surfaceről
void Snake::set_texture(SDL_Renderer* renderer)
{
	snake_head_texture = SDL_CreateTextureFromSurface(renderer, snake_head);
	snake_body_texture = SDL_CreateTextureFromSurface(renderer, snake_body);
	snake_end_texture = SDL_CreateTextureFromSurface(renderer, snake_end);
	snake_curve_texture = SDL_CreateTextureFromSurface(renderer, snake_curve);
}
//Visszaadja a snake egy partját
SDL_Texture* Snake::get_texture(unsigned a)
{
	switch (a) {
	case 0:
		return snake_head_texture;
	case 1:
		return snake_body_texture;
	case 2:
		return snake_end_texture;
	case 3:
		return snake_curve_texture;

	default:
		return NULL;
	}
}
//Törli a betöltott textúrákat
void Snake::destroy_texture()
{
	SDL_DestroyTexture(snake_head_texture);
	SDL_DestroyTexture(snake_body_texture);
	SDL_DestroyTexture(snake_end_texture);
	SDL_DestroyTexture(snake_curve_texture);

	Snake::snake_body_texture = NULL;
	Snake::snake_head_texture = NULL;
	Snake::snake_end_texture = NULL;
	Snake::snake_curve_texture = NULL;
}

//Statikus változók inicializása

SDL_Surface* Snake::snake_body = NULL;
SDL_Surface* Snake::snake_head = NULL;
SDL_Surface* Snake::snake_end = NULL;
SDL_Surface* Snake::snake_curve = NULL;

SDL_Texture* Snake::snake_body_texture = NULL;
SDL_Texture* Snake::snake_head_texture = NULL;
SDL_Texture* Snake::snake_end_texture = NULL;
SDL_Texture* Snake::snake_curve_texture = NULL;
