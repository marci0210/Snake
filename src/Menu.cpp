#include "Menu.h"

//Bet�lti a menu surfaceket
bool Menu::load() {
	if (!(play_game = SDL_LoadBMP("texture/start_game.bmp"))) return false;
	if (!(exit = SDL_LoadBMP("texture/quit.bmp"))) return false;
	if (!(wallpaper = SDL_LoadBMP("texture/wallpaper.bmp"))) return false;

	return true;
}
//T�rli a menu surfaceket
void Menu::destroy() {
	SDL_FreeSurface(wallpaper);
	SDL_FreeSurface(play_game);
	SDL_FreeSurface(exit);

	Menu::wallpaper = NULL;
	Menu::play_game = NULL;
	Menu::exit = NULL;
}
//Bet�lti a snake text�r�kat a kapott rendererre(surface inici�liz�s�sa sz�ks�ges)
void Menu::set_texture(SDL_Renderer* renderer)
{
	wallpaper_texture = SDL_CreateTextureFromSurface(renderer, wallpaper);
	play_game_texture = SDL_CreateTextureFromSurface(renderer, play_game);
	exit_texture = SDL_CreateTextureFromSurface(renderer, exit);
}
//Visszaadja a megfelelo text�r�t
SDL_Texture* Menu::get_texture(const unsigned a)
{
	switch (a) {
	case 0:
		return wallpaper_texture;
	case 1:
		return play_game_texture;
	case 2:
		return exit_texture;
	default:
		return NULL;
	}
}
//T�rli a menu texturakat
void Menu::destroy_texture()
{
	SDL_DestroyTexture(wallpaper_texture);
	SDL_DestroyTexture(play_game_texture);
	SDL_DestroyTexture(exit_texture);

	Menu::wallpaper_texture = NULL;
	Menu::play_game_texture = NULL;
	Menu::exit_texture = NULL;
}
//statikus surface inicializ�l�sa (sz�ks�ges)
SDL_Surface* Menu::wallpaper = NULL;
SDL_Surface* Menu::play_game = NULL;
SDL_Surface* Menu::exit = NULL;
//statikus textura inicializalasa
SDL_Texture* Menu::wallpaper_texture = NULL;
SDL_Texture* Menu::play_game_texture = NULL;
SDL_Texture* Menu::exit_texture = NULL;
