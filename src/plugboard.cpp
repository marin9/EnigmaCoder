#include <SDL2/SDL.h>
#include "plugboard.hpp"
#include <math.h>

Plugboard::Plugboard(int _x, int _y, int _width, int _height, int _text_size, SDL_Renderer *_renderer){
	renderer=_renderer;

	for(int i=0;i<27;++i){
		const char key[2]={(char)(65+i), '\0'};
		letters_texture[i]=new GText(0, 0, 3, renderer);	
		letters_texture[i]->setColors(220, 220, 220, 100, 100, 100);
		letters_texture[i]->setTransparent(true);
		letters_texture[i]->setText(key);
	}

	setBackground(255, 170, 00);

	plugboard_rect.x=_x;
	plugboard_rect.y=_y;
	plugboard_rect.w=_width;
	plugboard_rect.h=_height;

	padding=5;	
	index=0;

	for(int i=0;i<13*2;++i){
		data[i]=0;
	}
}

Plugboard::~Plugboard(){
	for(int i=0;i<27;++i) delete letters_texture[i];
}

bool Plugboard::isClicked(SDL_Event *event){
	if(event->type==SDL_MOUSEBUTTONDOWN){
		int x, y;
		SDL_GetMouseState(&x, &y);

		if(x>plugboard_rect.x && y>plugboard_rect.y &&
		   x<(plugboard_rect.x+plugboard_rect.w) && y<(plugboard_rect.y+plugboard_rect.h)){		
			return true;
		}		
	}
	return false;
}

bool Plugboard::removeChar(){	
	if(index>0){
		--index;
		data[index]=0;
	 	
	}

	return index%2==0;
}
	
bool Plugboard::appendChar(char letter){
	for(int i=0;i<index;++i){
		if(data[i]==letter) return false;
	}
	
	data[index]=letter;
	++index;
	return index%2==0;
}

char* Plugboard::getData(){
	return data;
}

void Plugboard::setLocationX(int _x){
	plugboard_rect.x=_x;
}

void Plugboard::setLocationY(int _y){
	plugboard_rect.y=_y;
}
	
int Plugboard::getLocationX(){
	return plugboard_rect.x;
}
	
int Plugboard::getLocationY(){
	return plugboard_rect.y;
}
	
int Plugboard::getWidth(){
	return plugboard_rect.w;
}
	
int Plugboard::getHeight(){
	return plugboard_rect.h;
}

void Plugboard::setBackground(int r, int g, int b){
	background.r=r;
	background.g=g;
	background.b=b;
	background.a=255;
}
	
void Plugboard::render(){
	SDL_SetRenderDrawColor(renderer, 60, 60, 70, 255);
	SDL_RenderFillRect(renderer, &plugboard_rect);

	SDL_Rect rect=plugboard_rect;
	rect.x=rect.x+padding;
	rect.y=rect.y+padding;
	rect.w=rect.w-2*padding;
	rect.h=rect.h-2*padding;
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderFillRect(renderer, &rect);

	int space=0;
	for(int i=0;i<index;++i){
		int j=(int)data[i]-65;

		letters_texture[j]->setLocationX(plugboard_rect.x+padding+space+i*(letters_texture[0]->getWidth()));
		if(space%2==0) letters_texture[j]->setLocationY(plugboard_rect.y+padding);
		else letters_texture[j]->setLocationY(plugboard_rect.y+padding*3); 
		letters_texture[j]->render();	
		if(i%2==1 && i!=0) space+=5;		
	}
}


