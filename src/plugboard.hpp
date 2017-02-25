#ifndef PLUGBOARD_H_INCLUDED
#define PLUGBOARD_H_INCLUDED
#include <SDL2/SDL.h>
#include "gtext.hpp"

class Plugboard{
private:
	SDL_Renderer *renderer;
	GText *letters_texture[27];
	SDL_Color background;
	SDL_Rect plugboard_rect;
	int padding;

	int index;
	char data[13*2];
    
public:
	Plugboard(int _x, int _y, int _width, int _height, int _text_size, SDL_Renderer *_renderer);
	~Plugboard();

	bool isClicked(SDL_Event *event);
	bool removeChar();
	bool appendChar(char letter);	
	char* getData();

	void setLocationX(int _x);
	void setLocationY(int _y);
	int getLocationX();
	int getLocationY();
	int getWidth();
	int getHeight();

	void setBackground(int r, int g, int b);
	void render();
};

#endif
