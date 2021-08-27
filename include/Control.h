#pragma once
#include "SDL.h"
#include "Snake.h"

//0 - W, 1 - A, 2 - S, 3 - D irányt visszaadó függvény
void keypress(int&, int); 

//Rectanglek-be való kattintást érzékeli (az elsp start game gomb, második quit)
bool click_rectangle(int&, int&, const SDL_Rect*, const SDL_Rect*, bool&);
