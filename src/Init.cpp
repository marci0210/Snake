#include "Init.h"


//Ablak inicializálása
bool init(SDL_Window** window, SDL_Surface** surface, const char* name, const int width, const int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
	else {
		*window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, NULL);
		if (window == NULL) return false;
		else {
			*surface = SDL_GetWindowSurface(*window);
			return true;
		}
	}
}
//Kilepo event
bool exit()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT ||
			(event.type == SDL_WINDOWEVENT &&
				event.window.event == SDL_WINDOWEVENT_CLOSE)) {
			return true;
		}
		return false;
	}
}

//Beolvassa az eddigi lokális maximum pontot
unsigned read_highscore()
{
	unsigned highscore = 0;
	std::ifstream file("highscore.txt");
	if (file.is_open())
	{
		file >> highscore;
		file.close();
	}
	else {
		write_highscore(highscore);
	}
	file.close();
	return highscore;

}
void write_highscore(const unsigned score)
{
	std::ofstream file("highscore.txt");
	file << score;
	file.close();
}
//Futtatja a gamet és a menut
void game(SDL_Window* window, unsigned& highscore, bool& exit_enable) {
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	//Beállítja a szükséges változók értékét
	const int map_size = 20; //Pálya mérete
	unsigned score = 0; //Aktuális pontszám
	unsigned snake_length = 7; //Kigyo aktuális hossza

	unsigned fruit_x = 0, fruit_y = 0, fruit_which = 0; //Gyümölcs koordinárái, és tipusa
	unsigned snake_x = 0, snake_y = 0;  //Kigyo egy elemének koordinátája
	unsigned dinamit_x = 0, dinamit_y = 0; //Dinamit koordinátája
	int lifetime = -1; //Dinamit élettartalma, -1 a kezdetekben
	bool enable_dinamit = false, seged = false; //Bomba engedélyezéséhez szükséges bool-ok

	//0 - W, 1 - A, 2 - S, 3 - D
	int default_direction = 3; //Alapértelmezett és segéd irány a reszponzivitás végett
	int direction = default_direction; //map.movement fuggveny altal kapott irány

	set_label(score, "Snake Score: ", window); //Ablak nevének beállítása

	Map<map_size, map_size> map; //Palya letrehozasa
	Snake* snakepart = new Snake[snake_length]; //snake tomb letrehozasa

	map.init_texture(renderer); //Előhívja a pálya textúráit

	//Inicializája a páyát, és annak minden tartalmát
	map.init_map(); //Map iniciílizásása
	map.init_wall(); //Fal létrehozása
	map.init_snake(snakepart, snake_length); //Snake inicializásása snakepart tömb segítségével
	map.init_fruit(fruit_x, fruit_y, fruit_which); //Egy gyömölcs létrehozása

	//Betolti a texturakat, majd var 1sec-et az indulásig
	map.texture_wall(renderer); //Betölti a szükséges textúrákat
	map.snake_texture(renderer, (snakepart), snake_length); //Snake textúra rátöltése ideiglenesen a snakepart-ra
	SDL_RenderPresent(renderer); //Renderelés

	//1sec-es várakozásii idő felkészülés végett, majd render tisztítás
	SDL_Delay(1000);
	SDL_RenderClear(renderer);

	//Sebesség beállítása
	unsigned hanyadik = 0; //Korszamlalo
	unsigned szamlalo = 12; //A kigyo sebesseget hatarozza meg

	while (true)
	{
		hanyadik++; //noveli a korszamlalot

		//A X gomb megnyomását figyeli
		if (exit()) {
			exit_enable = true;
			break;
		}
		//Ha elértünk a megfelelő körig (sebesség fv-e)
		if (hanyadik % szamlalo == 0)
		{
			SDL_RenderClear(renderer); //Render tiszítás
			direction = default_direction; //Beállítja az irányt az elmúlt szamlalo ido alatt kaptt irány fv-ben

			//Ha a mozgás ütözéssel zárul
			if (!map.movement(snakepart, snake_length, direction, snake_x, snake_y))
			{
				//Ha a szembe kerülő objektum breakable (fruits)
				if (map.collison(snake_x, snake_y, score))
				{
					//Snake tömböt egyel nagyobbá teszem, majd új gyömölcsöt generál
					snakepart = make_snake_bigger(snakepart, snake_length, fruit_x, fruit_y);
					map.init_fruit(fruit_x, fruit_y, fruit_which); 
					//Átállítja a címet a megnövelt score-el
					set_label(score, (std::string)"Snake Score: ", window);

					//Aktuális pontszam fuggvenyeben noveli a kigyo sebesseget
					if (score >= 100) {
						szamlalo = 6;
					}
					else if (score >= 80) {
						szamlalo = 7;
					}
					else if (score >= 60) {
						szamlalo = 9;
					}
					else if (score >= 40) {
						szamlalo = 10;
						//Egyszer aktiválja a dinamit engedályezését
						//Különben 40 és 60 között folyamatosan uj dinamitok jönnének létre
						if (seged == false)
						{
							enable_dinamit = true;
							seged = true;
						}

					}
					else if (score >= 20) {
						szamlalo = 11;
					}
				}
				//Ha a szembekerülő objektum nem breakable (wall, dinamit)
				else {
					break;
				}
			}
			//Ha a dinamit engedélyezve van, és az élettartalma kisebbegyenlő 0 (-1 a kezdekeben)
			if (enable_dinamit == true && lifetime <= 0)
			{
				map.init_dinamit(dinamit_x, dinamit_y, lifetime, enable_dinamit);
			}
			//Ha vana pályán dinamit, álettartalma csökkentése
			if (enable_dinamit == false && lifetime > 0) {
				lifetime--;
			}
			//Ha van dinamit a pályám és az élettartalma 0, törlöm
			else if (enable_dinamit == false && lifetime == 0)
			{
				map.delete_dinamit(dinamit_x, dinamit_y, enable_dinamit);
			}

			hanyadik = 0; //Alapra allitja a korszamlalot
			//Ha van dinamit a pályán, tetórájának betöltése
			if (dinamit_x != 0 && dinamit_y != 0) map.texture_dinamit(renderer, dinamit_x, dinamit_y);
			map.texture_wall(renderer); //Betölti a wall textúráját
			map.fruit_texture(renderer, fruit_x, fruit_y, fruit_which); //Betölti a fruit textúráját
			map.snake_texture(renderer, (snakepart), snake_length); //Betölti a snake textúráját

			SDL_RenderPresent(renderer); //Frissiti a render-t
		}
		keypress(default_direction, direction); //Iranyt allit
		SDL_Delay(10); //Minimális sdl delay

	}
	//Törli a betöltött wal, fruit, snake textúrát
	map.delete_texture();
	map.delete_snake(snakepart, snake_length); //Törli a lefoglalt snake tömböt
	//Törli a renderer "tartalmát", majd törli a renderert
	SDL_RenderClear(renderer);
	SDL_DestroyRenderer(renderer);
	//Torli a palya celláit, és NULL pointerré állítja
	map.delete_map(map_size);

	//Ha a pontszám nagyobb, mint az eddigi maximális, felülírja azt
	if (highscore < score)
	{
		write_highscore(score);
		highscore = score;
	}
}
void menu(SDL_Window* window, unsigned& highscore, bool& exit_enable)
{
	//Létrehozza a renderert
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	//Változók definiálsása
	int x, y; //Egér koordinátái
	SDL_Rect* rect = new SDL_Rect[3]; //Lefoglalja a menu elemeket tartalmazó tömböt
	//Beállítja a címet az eddigi rekordra
	set_label(highscore, "Snake Record: ", window);

	//Betölti a menu textúráit
	Menu::set_texture(renderer);

	//Beállítja a háttér paramétereit
	rect[0].x = rect[0].y = 0;
	rect[0].w = rect[0].h = 800;
	//Ramásolja a háttérképet a rect-re
	SDL_RenderCopy(renderer, Menu::get_texture(0), NULL, &rect[0]);

	//Beállítja a cím és menugombok paramétereit, majd rámásolja a textúrájukat
	for (int i = 0; i < 2; i++)
	{
		rect[i + 1].x = 475;
		rect[i + 1].y = 550 + 125 * i;
		rect[i + 1].w = 300;
		rect[i + 1].h = 100;

		SDL_RenderCopy(renderer, Menu::get_texture(i + 1), NULL, &rect[i + 1]);
	}
	//Lefrissti a renderert
	SDL_RenderPresent(renderer);
	
	//Ameddig nem történik menugombra való kattintás
	while (true)
	{
		SDL_GetMouseState(&x, &y);

		if (click_rectangle(x, y, &rect[1], &rect[2], exit_enable)) break;
		if (exit()) {
			exit_enable = true;
			break;
		}
		SDL_Delay(50);
	}

	//Törli a betöltött textúrákat
	Menu::destroy_texture();
	//Törli a renderer "tartalmát", majd törli a renderert
	SDL_RenderClear(renderer);
	SDL_DestroyRenderer(renderer);

	//Törli a lefoglalt menü elemeket tartalmazó tömböt
	delete[] rect;
}
//Egy kaptt SDL_Rect-en belül megnézni, történt a kattintás


//Megpróbálja betölteni a textúrákat, igazzal tér vissza, ha sikerült
void set_label(unsigned& score, std::string title, SDL_Window* window)
{
	std::stringstream ss; //Létrehoz egy sringstreamet
	ss << title << score; //Összefüzi a kapott stringet és a számot
	std::string label = ss.str(); //Stringé alakítja a ss-t
	const char* const_label = label.c_str(); //COnst char*-á alakítja label stringet (setwindowtitle-hez kell)

	SDL_SetWindowTitle(window, const_label);
}
//Visszaad egy egyel nagyobb snake tömböt, előzőt felszabadítja
Snake* make_snake_bigger(Snake* prev, unsigned& length, unsigned& fruit_x, unsigned& fruit_y)
{
	Snake* newone = new Snake[length + 1]; //Létrehozza a nagyobb snake tömböt
	newone[0].set(fruit_x, fruit_y); //Beállítja a gyümölcs koordinátájára az első elemet (csak akkor nőhet)

	//Meghatározza a snake fejének új irányát
	if (prev[0].get_x() == fruit_x - 1 && prev[0].get_y() == fruit_y) newone[0].setdirection(3);
	else if (prev[0].get_x() == fruit_x + 1 && prev[0].get_y() == fruit_y) newone[0].setdirection(1);
	else if (prev[0].get_x() == fruit_x && prev[0].get_y() == fruit_y + 1) newone[0].setdirection(0);
	else if (prev[0].get_x() == fruit_x && prev[0].get_y() == fruit_y - 1) newone[0].setdirection(2);

	//Átmásolja a korábbi snake elemeit az újba egyel eltolva (őj fej helye miatt)
	for (unsigned i = 0; i < length; i++)
	{
		newone[i + 1] = prev[i];
	}
	length++; //Növeli a snake tömb elemszámát, ez egy referencia

	delete[] prev; //Törli az előző tömböt

	return newone; //Visszatér az új snake tömbbel
}

bool load()
{
	//Betölti a szükséges bmp fájlokat a surfacere (statikus, pointereket használ a program ezekre)
	if (!Snake::load()) return false;
	if (!Objects::load()) return false;
	if (!Fruits::load()) return false;
	if (!Menu::load()) return false;
	return true;
}
//Törli a betöltött surfaceket (textúrák már törölve lettek)
void unload()
{
	Snake::destroy();
	Fruits::destroy();
	Objects::destroy();
	Menu::destroy();
}
