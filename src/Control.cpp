#include "SDL.h"
#include "Control.h"

void keypress(int& a, int direction)
{
	//Létrehozza az SDL eventet
	SDL_Event event;
	SDL_PollEvent(&event);
	//Billentyőzetet UTF-8-ra állítja
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	//Beállítja az új irányt, ha az nem tiltott
	if (keyboard[SDL_GetScancodeFromName("W")] && direction != 2)
	{
		a = 0;
	}
	else if (keyboard[SDL_GetScancodeFromName("A")] && direction != 3)
	{
		a = 1;
	}
	else if (keyboard[SDL_GetScancodeFromName("S")] && direction != 0)
	{
		a = 2;
	}
	else if (keyboard[SDL_GetScancodeFromName("D")] && direction != 1)
	{
		a = 3;
	}
}
bool click_rectangle(int& x, int& y, const SDL_Rect* rect_1, const SDL_Rect* rect_2, bool& exit_enable)
{
	//SDL event létrehozása
	SDL_Event event;
	SDL_PollEvent(&event);
	//Ha x koordinátán irányban van az egér
	if (x >= rect_1->x && x < (rect_1->x + rect_1->w))
	{
		//Ha az y koordináta az első rect-en belöl van
		if (y >= rect_1->y && y < (rect_1->y + rect_1->h))
		{
			if (event.type == SDL_MOUSEBUTTONUP) {
				return true;
			}
		}
		//Ha az y koordináta a  második rect-en belöl van
		else if (y >= rect_2->y && y < (rect_2->y + rect_2->h))
		{
			if (event.type == SDL_MOUSEBUTTONUP) {
				exit_enable = true; //Quit gomb
				return true;
			}
		}
	}
	return false;
}
//Write score

