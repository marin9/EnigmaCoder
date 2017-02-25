#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED
#include <SDL2/SDL.h>
#include "gtext.hpp"

class Keyboard{
public:
	const static char row1[];
	const static char row2[];
	const static char row3[];
private:
	SDL_Renderer *renderer;
	GText *grow1[10];
	GText *grow2[9];
	GText *grow3[8];
	SDL_Color color_pressed, color_not_pressed, color_letter;
	SDL_Rect keyboard_rect, key_rect;
	int padding;
	char pressedKey;
    

public:
	Keyboard(int _x, int _y, int _width, int _height, int key_text_size, SDL_Renderer *_renderer);
	~Keyboard();

	int getPressedKey(SDL_Event *event);
	void setLocationX(int _x);
	void setLocationY(int _y);
	void setWidth(int _width);
	void setHeight(int _height);
	int getLocationX();
	int getLocationY();
	int getWidth();
	int getHeight();
	void render();
private:
	void calculate();
};

#endif
