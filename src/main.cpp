//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

#include <iostream>
#include <vector>
#include <random>
#include <time.h>

#include "SDL.h"
#include "Objects.h"
#include "Fruits.h"
#include "Snake.h"

#include "Control.h"
#include "Table.hpp"

#include "Init.h"

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	//SDL inicializálása
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	//WIndows, surface pointer létrehozása
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	//aBlak inicializálása
	init(&window, &surface, "Snake", 800, 800);
	//Betölti a highscore-t a fjálból
	unsigned highscore = read_highscore();
	//Exit bool inicializáslása
	bool exit_enable = false;
	//Megpróbálja betölti a bmp fáljokat, ha nem sikerülm false-t dob vissza
	bool everything_alright = load();
	if(everything_alright == false) std::cout << "Texture file(s) missing. PLease check your files." << std::endl;
	//"Main" ciklus
	while (everything_alright)
	{
		if (exit_enable == false) {
			menu(window, highscore, exit_enable);
		}
		if (exit_enable == false) {
			game(window, highscore, exit_enable);
		}
		if (exit_enable == true) break;
	}
	//Törli a betöltött surfaceket, a textórák már törölve vannak
	unload();
	
	SDL_FreeSurface(surface); //Felszabadítja a surfacet
	SDL_DestroyWindow(window); //Törli az ablakot
	SDL_Quit(); //Bezárja az SDL-t

	return 0;
}