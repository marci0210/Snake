#pragma once
#include <fstream>
#include <sstream>
#pragma once
#include <iostream>
#include "SDL.h"
#include "Table.hpp"

#include "Objects.h"
#include "Fruits.h"
#include "Snake.h"
#include "Menu.h"

//Ablak inicializálása
bool init(SDL_Window**, SDL_Surface**, const char*, const int, const int);
//Az X gomb megnyomását vizsgálja
bool exit();

//Beolvassa az eddigi lokális maximum pontot a txt fájlból
unsigned read_highscore();
//Kiirja az uj lokalis maximum pontot a txt fájlba
void write_highscore(const unsigned);

//A játék fő része, meghivja a szukseges függvényeket, osztályokat
void game(SDL_Window* window, unsigned&, bool&);
//Kezeli a menut, ,meghívja a szükséges függvényeket, osztályokat
void menu(SDL_Window* window, unsigned&, bool&);

//Beállítja az ablak nevét a paraméterként kapott stringre, amit összefűz az unsigned-el
void set_label(unsigned&, std::string, SDL_Window*);
//Betölti a bmp fáljokat a statikus surfacere, igazzal tér vissza, ha sikerül
bool load();
//Törli az összes betöltött textúrát a surfaceről
void unload();
//Visszaad egy egyel nagyobb snake tömböt, előzőt felszabadítja
Snake* make_snake_bigger(Snake* prev, unsigned& length, unsigned& fruit_x, unsigned& fruit_y);