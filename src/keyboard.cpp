#include <SDL2/SDL.h>
#include "keyboard.hpp"


const char Keyboard::row1[]={'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P'};
const char Keyboard::row2[]={'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'};
const char Keyboard::row3[]={'Y', 'X', 'C', 'V', 'B', 'N', 'M', ' '};


Keyboard::Keyboard(int _x, int _y, int _width, int _height, int key_text_size, SDL_Renderer *_renderer){
	renderer=_renderer;

	color_not_pressed.r=50;
	color_not_pressed.g=50;
	color_not_pressed.b=50;
	color_not_pressed.a=255;

	color_pressed.r=120;
	color_pressed.g=120;
	color_pressed.b=150;
	color_pressed.a=255;
	
	color_letter.r=255;
	color_letter.g=0;
	color_letter.b=0;
	color_letter.a=255;

	keyboard_rect.x=_x;
	keyboard_rect.y=_y;
	keyboard_rect.w=(int)_width*0.95;
	keyboard_rect.h=_height;

	padding=3;
	pressedKey=-1;


	for(int i=0;i<10;++i){
		const char key[2]={row1[i], '\0'};
		grow1[i]=new GText(0, 0, key_text_size, renderer);	
		grow1[i]->setColors(255, 0, 0, 50, 50, 50);
		grow1[i]->setTransparent(true);
		grow1[i]->setText(key);
	}

	for(int i=0;i<9;++i){
		const char key[2]={row2[i], '\0'};
		grow2[i]=new GText(0, 0, key_text_size, renderer);	
		grow2[i]->setColors(255, 0, 0, 50, 50, 50);
		grow2[i]->setTransparent(true);
		grow2[i]->setText(key);
	}

	for(int i=0;i<8;++i){
		const char key[2]={row3[i], '\0'};
		grow3[i]=new GText(0, 0, key_text_size, renderer);
		grow3[i]->setColors(255, 0, 0, 50, 50, 50);	
		grow3[i]->setTransparent(true);
		grow3[i]->setText(key);
	}
	
	calculate();
}

Keyboard::~Keyboard(){
	for(int i=0;i<10;++i) delete grow1[i];
	for(int i=0;i<9;++i) delete grow2[i];
	for(int i=0;i<8;++i) delete grow3[i];	
}

int Keyboard::getPressedKey(SDL_Event *event){
	if(event->type==SDL_MOUSEBUTTONDOWN){
		int x, y, row, column;
		SDL_GetMouseState(&x, &y);
		if(x<keyboard_rect.x || y<keyboard_rect.y || 
		   x>(keyboard_rect.x+keyboard_rect.w+padding*10) || y>(keyboard_rect.y+keyboard_rect.h+padding*3)) 
			return -1;

		row=(int)( (y-keyboard_rect.y)/(key_rect.h+padding) );
		if(row==0) column=(x-keyboard_rect.x)/(key_rect.w+padding);
		else if(row==1) column=(x-keyboard_rect.x-(key_rect.w/2))/(key_rect.w+padding);
		else if(row==2) column=(x-keyboard_rect.x-key_rect.w)/(key_rect.w+padding);

		if(row==0) return pressedKey=row1[column];
		else if(row==1) return pressedKey=row2[column];
		else if(row==2) return pressedKey=row3[column];
		
		return -1;
	}else if(event->type==SDL_MOUSEBUTTONUP){
		return pressedKey=-1;
	}else if(event->type==SDL_KEYDOWN){		
		return pressedKey=event->key.keysym.sym-32;
	}else if(event->type==SDL_KEYUP){
		return pressedKey=-1;
	}else 
		return -1;
}

void Keyboard::setLocationX(int _x){
	keyboard_rect.x=_x;
}

void Keyboard::setLocationY(int _y){
	keyboard_rect.y=_y;
}

void Keyboard::setWidth(int _width){
	keyboard_rect.w=_width;
	calculate();
}

void Keyboard::setHeight(int _height){
	keyboard_rect.h=_height;
	calculate();
}
	
int Keyboard::getLocationX(){
	return keyboard_rect.x;
}
	
int Keyboard::getLocationY(){
	return keyboard_rect.y;
}

int Keyboard::getWidth(){
	return keyboard_rect.w;
}
	
int Keyboard::getHeight(){
	return keyboard_rect.h;
}

void Keyboard::render(){
	SDL_SetRenderDrawColor(renderer, color_not_pressed.r, color_not_pressed.g, color_not_pressed.b, color_not_pressed.a);

	key_rect.y=keyboard_rect.y+padding;
	for(int i=0;i<10;++i){
		key_rect.x=keyboard_rect.x+padding+i*(key_rect.w+padding);
		if(row1[i]==pressedKey){
			SDL_SetRenderDrawColor(renderer, color_pressed.r, color_pressed.g, color_pressed.b, color_pressed.a);
			SDL_RenderFillRect(renderer, &key_rect);     
			SDL_SetRenderDrawColor(renderer, color_not_pressed.r, color_not_pressed.g, color_not_pressed.b, color_not_pressed.a);
			
			grow1[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow1[i]->getHeight()/2));
			grow1[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow1[i]->getHeight()/2));
			grow1[i]->render();
		}else{
			SDL_RenderFillRect(renderer, &key_rect); 
			grow1[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow1[i]->getHeight()/2));
			grow1[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow1[i]->getHeight()/2));
			grow1[i]->render();        
		}
	}

	key_rect.y+=padding+key_rect.h;
	for(int i=0;i<9;++i){
		key_rect.x=key_rect.w*0.5+keyboard_rect.x+padding+i*(key_rect.w+padding);
		if(row2[i]==pressedKey){
			SDL_SetRenderDrawColor(renderer, color_pressed.r, color_pressed.g, color_pressed.b, color_pressed.a);
			SDL_RenderFillRect(renderer, &key_rect);     
			SDL_SetRenderDrawColor(renderer, color_not_pressed.r, color_not_pressed.g, color_not_pressed.b, color_not_pressed.a);
		
			grow2[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow2[i]->getHeight()/2));
			grow2[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow2[i]->getHeight()/2));
			grow2[i]->render();
		}else{
			SDL_RenderFillRect(renderer, &key_rect); 
			grow2[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow2[i]->getHeight()/2));
			grow2[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow2[i]->getHeight()/2));
			grow2[i]->render();    
		}
	}

	key_rect.y+=padding+key_rect.h;
	for(int i=0;i<8;++i){
		key_rect.x=key_rect.w*1.0+keyboard_rect.x+padding+i*(key_rect.w+padding);
		if(row3[i]==pressedKey){
			SDL_SetRenderDrawColor(renderer, color_pressed.r, color_pressed.g, color_pressed.b, color_pressed.a);
			SDL_RenderFillRect(renderer, &key_rect);     
			SDL_SetRenderDrawColor(renderer, color_not_pressed.r, color_not_pressed.g, color_not_pressed.b, color_not_pressed.a);

			grow3[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow3[i]->getHeight()/2));
			grow3[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow3[i]->getHeight()/2));
			grow3[i]->render();
		}else{
			SDL_RenderFillRect(renderer, &key_rect);   
			grow3[i]->setLocationX(key_rect.x+(key_rect.w/2)-(grow3[i]->getHeight()/2));
			grow3[i]->setLocationY(key_rect.y+(key_rect.h/2)-(grow3[i]->getHeight()/2));
			grow3[i]->render();    
		}
	}

}

void Keyboard::calculate(){
	key_rect.w=keyboard_rect.w/10;
	key_rect.h=keyboard_rect.h/3;
}


