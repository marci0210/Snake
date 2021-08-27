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

	//SDL inicializ�l�sa
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	//WIndows, surface pointer l�trehoz�sa
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	//aBlak inicializ�l�sa
	init(&window, &surface, "Snake", 800, 800);
	//Bet�lti a highscore-t a fj�lb�l
	unsigned highscore = read_highscore();
	//Exit bool inicializ�sl�sa
	bool exit_enable = false;
	//Megpr�b�lja bet�lti a bmp f�ljokat, ha nem siker�lm false-t dob vissza
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
	//T�rli a bet�lt�tt surfaceket, a text�r�k m�r t�r�lve vannak
	unload();
	
	SDL_FreeSurface(surface); //Felszabad�tja a surfacet
	SDL_DestroyWindow(window); //T�rli az ablakot
	SDL_Quit(); //Bez�rja az SDL-t

	return 0;
}