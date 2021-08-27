#pragma once

#include "Objects.h"
#include "Fruits.h"
#include "Snake.h"
#include "Menu.h"

#include "SDL.h"
#include "Control.h"
#include <time.h>

template <unsigned width, unsigned height>
class Map 
{
	Objects* cella[width][height]; //Objektumra mutató pointer
public:
	//Objektum indexelő operátor
	Objects** operator[](unsigned row) 
	{
		return cella[row];
	}
	//A kapott rendererre betölti a textúrákat
	void init_texture(SDL_Renderer* renderer)
	{
		Snake::set_texture(renderer);
		Objects::set_texture(renderer);
		Fruits::set_texture(renderer);
	}
	void delete_texture()
	{
		Snake::destroy_texture();
		Objects::destroy_texture();
		Fruits::destroy_texture();
	}
	//Map összes elemét NULL pointerre állítja
	void init_map()
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				cella[i][j] = NULL;
			}
		}
	}
	
	//A pálya legszélső elemeire létrehoz új objektumot, falhoz szükséges
	void init_wall()
	{
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {

				if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {

					cella[i][j] = new Objects(i, j, false);
				}
			}
		}
	}
	void texture_wall(SDL_Renderer* renderer)
	{
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {

				if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
					SDL_RenderCopy(renderer, Objects::get_texture(0), NULL, &cella[i][j]->get_sdl_render());
				}
			}
		}
	}
	//Iniciálizál egy dinamitot random koordinátára
	
	void init_dinamit(unsigned& din_x, unsigned& din_y, int& lifetime, bool& enable)
	{
		din_x = 0;
		din_y = 0;

		srand((unsigned)time(NULL));
		while (cella[din_x][din_y] != NULL)
		{
			din_x = rand() % width;
			din_y = rand() % height;
		}
		cella[din_x][din_y] = new Objects(din_x, din_y, false);
		lifetime = 60;
		enable = false;
	}
	//Betölti a textúráját a dinamitnak
	void texture_dinamit(SDL_Renderer* renderer, unsigned& din_x, unsigned& din_y)
	{
		SDL_RenderCopy(renderer, Objects::get_texture(1), NULL, &cella[din_x][din_y]->get_sdl_render());
	}
	//Törli a dinamitot
	void delete_dinamit(unsigned& din_x, unsigned& din_y, bool& enable)
	{
		delete[] cella[din_x][din_y];
		cella[din_x][din_y] = NULL;
		enable = true;

		din_x = 0;
		din_y = 0;
	}
	//Betölti fal textúrát a pálya szélére (inicializálás szükséges)
	
	//Random NULL pointerre létrehoz egy új random Fruit objektumot
	void init_fruit(unsigned& x, unsigned& y, unsigned& which)
	{
		srand((unsigned)time(NULL));
		which = rand() % 4;

		x = 0, y = 0;

		Fruits::set_point(5 - which);

		while (cella[x][y] != NULL)
		{
			x = rand() % width;
			y = rand() % height;
		}

		cella[x][y] = new Fruits(x, y, 5);
	}
	//Fruit objektumra betölti a megfelelp textúrát
	void fruit_texture(SDL_Renderer* renderer, unsigned& x, unsigned& y, unsigned& which)
	{
		SDL_Texture* texture = NULL;

		switch (which) {
		case 0:
			texture = Fruits::get_texture(0);
			break;
		case 1:
			texture = Fruits::get_texture(1);
			break;
		case 2:
			texture = Fruits::get_texture(2);
			break;
		case 3:
			texture = Fruits::get_texture(3);
			break;
		}

		SDL_RenderCopy(renderer, texture, NULL, &cella[x][y]->get_sdl_render());
	}
	
	//Inicializálja a snaket a default koordinátákra, és az elemeit beállítja
	void init_snake(Snake* snakepart, unsigned length)
	{
		for (unsigned i = 0; i < length; i++)
		{
			snakepart[i].set(10 - i, 10);
			snakepart[i].setdirection(3);

			cella[10 - i][10] = new Snake(10 - i, 10);
		}
	}
	//Betölti a snake textúrát a paraméterként kapott snake tömbre
	void snake_texture(SDL_Renderer* renderer, Snake* snakepart, unsigned length)
	{
		SDL_Texture* texture = NULL; //Létrehoz egy textúra pointert
		
		for (unsigned i = 0; i < length; i++)
		{
			//Kikéri az i-ik elem koordinátáját
			unsigned x = snakepart[i].get_x();
			unsigned y = snakepart[i].get_y();

			//Kikéri az i-ik elem irányát
			unsigned direction = snakepart[i].getdirection();

			//Beállítja az alapértelmezett forgatást
			int rotate = 0;
			int rotate_new = 0;

			//Beállítja az alapértelmezett tükrözést
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			//Segéd bool változó
			bool a = false;
			//Ha nem az első elemről van szó(azt nem kell forgatni)
			if (i > 0 && i < length)
			{
				//Kikéri az előző elem irányát
				unsigned prev = snakepart[i - 1].getdirection();
				//Megnézi hogy a két irány között van e iránybeli különbség
				//Ha van, a curve textúrát töltöm be
				if (direction != prev)
				{
					//Betölti a curve textúrát
					texture = Snake::get_texture(3);
					//Irány és előző irány függvényében forgatjuk majd a textúrát
					switch(direction)
					{
					case 0:
						if (prev == 3) {
							a = true;
							rotate_new = 90;
						}
						else rotate_new = -90;
						break;
					case 1:
						if (prev == 0) {
							rotate_new = 90;
							a = true;
						}
						else rotate_new = -90;
						break;
					case 2:
						if (prev == 1) {
							rotate_new = 90;
							a = true;
						}
						else rotate_new = -90;
						break;
					case 3:
						if (prev == 2) {
							rotate_new = 90;
							a = true;
						}
						else rotate_new = -90;
						break;
					}
				}
				//Ha nincs forgatás, body textúrát tölti be
				else {
					texture = Snake::get_texture(1);
				}
				//Ha utolsó elem, end-et tölti be
				if (i == length - 1) {
					texture = Snake::get_texture(2);
					rotate = rotate_new;
				}
			}
			//Ha az első elem, head-et tölti be
			else if (i == 0) texture = Snake::get_texture(0);

			//Tükrözi a textúrát az iránynak megfelelően
			switch (direction)
			{
			case 0:
				if (a == true) flip = SDL_FLIP_NONE;
				else flip = SDL_FLIP_VERTICAL;
				break;
			case 1:
				if (a == true) flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
				else flip = SDL_FLIP_HORIZONTAL;
				break;
			case 2:
				if (a == true) flip = SDL_FLIP_NONE;
				
				else flip = SDL_FLIP_VERTICAL;
				break;
			case 3:
				if (a == true) flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
				else flip = SDL_FLIP_HORIZONTAL;
				break;
			}

			//Az aktuális iránynak megfelelően elforgatom a textúrát
			const SDL_Rect* qqq = &(cella[x][y])->get_sdl_render();
			switch (direction) {
			case 0:
				SDL_RenderCopyEx(renderer, texture, NULL, qqq, (90 + rotate), NULL, flip);
				break;
			case 1:
				SDL_RenderCopyEx(renderer, texture, NULL, qqq, (180 + rotate), NULL, flip);
				break;
			case 2:
				SDL_RenderCopyEx(renderer, texture, NULL, qqq, (270 + rotate), NULL, flip);
				break;
			case 3:
				SDL_RenderCopyEx(renderer, texture, NULL, qqq, (0 + rotate), NULL, flip);
				break;
			}
		}
	}
	//Törli a paraméterként kapott Snake tömböt
	void delete_snake(Snake* snakepart, unsigned& length)
	{
		delete[] snakepart;
		length = 0;
	}
	//A kapott iránynak megfelelően átállítja a snake minden elemének koordinátáját
	bool movement(Snake* snakepart, unsigned length, int& direction, unsigned& new_x, unsigned& new_y)
	{
		//Beálltja a fej koordinátáit
		new_x = snakepart[0].get_x();
		new_y = snakepart[0].get_y();
		
		//Megfelelő helyre átállítja, és megnézni, van e ott valami
		//Ha nincs, létrehoz egy új snake elemet
		switch (direction)
		{
		case 0:
			new_y--;
			if (cella[new_x][new_y] != NULL) return false;

			cella[new_x][new_y] = new Snake(new_x, new_y);
			break;

		case 1:
			new_x--;
			if (cella[new_x][new_y] != NULL) return false;

			cella[new_x][new_y] = new Snake(new_x, new_y);
			break;

		case 2:
			new_y++;
			if (cella[new_x][new_y] != NULL) return false;

			cella[new_x][new_y] = new Snake(new_x, new_y);
			break;

		case 3:
			new_x++;
			if (cella[new_x][new_y] != NULL) return false;

			cella[new_x][new_y] = new Snake(new_x, new_y);
			break;
		}

		//Kikéri a farok koordinátáját
		unsigned x = snakepart[length - 1].get_x();
		unsigned y = snakepart[length - 1].get_y();

		//Törlöm a farok korábbi helyét
		delete[] cella[x][y];
		cella[x][y] = NULL;
		//Visszafelé átállítom a koordinátát az előtte lévőre
		for (int i = length - 1; i > 0; i--)
		{
			snakepart[i] = snakepart[i - 1];
		}
		//Beállítja a fej új irányát, és koordinátáját
		snakepart[0].setdirection(direction);
		snakepart[0].set(new_x, new_y);

		return true;
	}
	//Collison detector - igazzal tér vissza, ha breakable objektummal találkozik (egyedül a Fruit ilyen)
	bool collison(unsigned& x, unsigned& y, unsigned& score)
	{
		if (cella[x][y]->get_breakable())
		{
			score += Fruits::get_point();

			delete[] cella[x][y];
			cella[x][y] = new Snake(x, y);
			return true;
			
		}
		return false;
	}
	
	//Törli a pálya minden egyes felépített celláját
	void delete_map(unsigned map_size)
	{
		for (unsigned i = 0; i < map_size; i++) {
			for (unsigned j = 0; j < map_size; j++) {
				delete[] cella[i][j];
				cella[i][j] = NULL;
			}
		}
	}
};