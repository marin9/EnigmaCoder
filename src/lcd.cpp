#include "lcd.hpp"
#include <stdio.h>

LCD::LCD(int _x, int _y, int _pixel_size, int _column, SDL_Renderer *_renderer){
	renderer=_renderer;

	lcd_rect.x=_x;
	lcd_rect.y=_y;
	pixel_size=_pixel_size;
	column=_column;
	border=(int)pixel_size*1.5;
	lcd_rect.w=2*border+2*pixel_size+(5*pixel_size+4)*column+pixel_size*(column-1);
	lcd_rect.h=2*border+2*pixel_size+7*pixel_size+6;

	text=NULL;	
	for(int i=0;i<27;++i) digit_texture[i]=NULL;
	setBackground(0, 255, 0);
}

LCD::~LCD(){
	clr();
	for(int i=0;i<27;++i)
		if(digit_texture[i]!=NULL)
			SDL_DestroyTexture(digit_texture[i]);	
}

void LCD::setText(const char* str){
	clr();
	text_len=0;
	while(str[text_len]!='\0') ++text_len;

	text=(char*)malloc((text_len+1)*sizeof(char));
	for(int i=0;i<text_len;++i){
		if((str[i]>='A' && str[i]<='Z') || str[i]==' ') text[i]=str[i];
		else if(str[i]>='a' && str[i]<='z') text[i]=str[i]-32;
		else text[i]=' ';
	}
	text[text_len]='\0';
}

const char* LCD::getText(){
	return text;
}

void LCD::clr(){
	if(text!=NULL) free(text);
	text=NULL;
}

void LCD::setLocationX(int x){
	lcd_rect.x=x;
}

void LCD::setLocationY(int y){
	lcd_rect.y=y;
}

int LCD::getLocationX(){
	return lcd_rect.x;
}

int LCD::getLocationY(){
	return lcd_rect.y;
}

int LCD::getWidth(){
	return lcd_rect.w;
}

int LCD::getHeight(){
	return lcd_rect.h;
}
    
void LCD::setBackground(int r, int g, int b){
	digit_color_on.r=30;
	digit_color_on.g=30;
	digit_color_on.b=30;
	digit_color_on.a=255;

	background.r=r;
	background.g=g;
	background.b=b;
	background.a=255;
	
	digit_color_off.r=(int)background.r*0.9;
	digit_color_off.g=(int)background.g*0.9;
	digit_color_off.b=(int)background.b*0.9;
	digit_color_off.a=255;	

	createTextures();
}

bool LCD::isClicked(SDL_Event *event){
	if(event->type==SDL_MOUSEBUTTONDOWN){
		int x, y;
		SDL_GetMouseState(&x, &y);
		if(x>lcd_rect.x && y>lcd_rect.y && 
		   x<(lcd_rect.x+lcd_rect.w) && y<(lcd_rect.y+lcd_rect.h)) return true;	
	}	

	return false;
}

void LCD::render(){	
	SDL_SetRenderDrawColor(renderer, 60, 60, 70, 255);
	SDL_RenderFillRect(renderer, &lcd_rect);
	
	SDL_Rect tmpRect=lcd_rect;
	tmpRect.x+=border;
	tmpRect.y+=border;
	tmpRect.w-=2*border;
	tmpRect.h-=2*border;
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderFillRect(renderer, &tmpRect);


	tmpRect.w=pixel_size*5+4;
	tmpRect.h=pixel_size*7+6;
	int i=0;
	for(i=0;i<text_len && i<column;++i){	
		tmpRect.x=lcd_rect.x+border+pixel_size+(tmpRect.w+pixel_size)*i;
		tmpRect.y=lcd_rect.y+border+pixel_size;

		if(text[i]==' ') SDL_RenderCopy(renderer, digit_texture[0], NULL, &tmpRect);
		else SDL_RenderCopy(renderer, digit_texture[(int)(text[i]-64)], NULL, &tmpRect);
	}

	while(i<column){
		tmpRect.x=lcd_rect.x+border+pixel_size+(tmpRect.w+pixel_size)*i;
		tmpRect.y=lcd_rect.y+border+pixel_size;
		SDL_RenderCopy(renderer, digit_texture[0], NULL, &tmpRect);
		++i;
	}
}

void LCD::createTextures(){
    SDL_Rect pixel;
    pixel.w=pixel_size;
    pixel.h=pixel_size;

    for(int i=0;i<27;++i){
	if(digit_texture[i]==NULL) 
		digit_texture[i]=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
						   pixel_size*5+4, pixel_size*7+6);	
	SDL_SetRenderTarget(renderer, digit_texture[i]);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderClear(renderer);

	for(int x=0;x<5;++x){
		for(int y=0;y<7;++y){
			if((character[i][y]>>(4-x)) & 0x01) 
				SDL_SetRenderDrawColor(renderer, digit_color_on.r, digit_color_on.g, digit_color_on.b, 255);
			else 
				SDL_SetRenderDrawColor(renderer, digit_color_off.r, digit_color_off.g, digit_color_off.b, 255);
			pixel.x=x*pixel_size+x;
			pixel.y=y*pixel_size+y;
			SDL_RenderFillRect(renderer, &pixel);
		}
	}
    }
    SDL_SetRenderTarget(renderer, NULL);
}



unsigned char LCD::character[27][7]={
	//Space
	{
	0b00000,
	0b00000,
	0b00000,
   	0b00000,
    	0b00000,
    	0b00000,
   	0b00000},

    	//A
	{
    	0b01110,
   	0b10001,
    	0b10001,
    	0b11111,
    	0b10001,
    	0b10001,
    	0b10001},

    	//B
	{
   	0b11110,
    	0b10001,
    	0b10001,
    	0b11110,
    	0b10001,
    	0b10001,
    	0b11110},


    	//C
	{
    	0b01110,
    	0b10001,
   	0b10000,
    	0b10000,
    	0b10000,
    	0b10001,
    	0b01110},

    	//D
	{
    	0b11100,
    	0b10010,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10010,
    	0b11100},

    	//E
	{
    	0b11111,
    	0b10000,
    	0b10000,
    	0b11110,
    	0b10000,
    	0b10000,
    	0b11111},

    	//F
	{
    	0b11111,
    	0b10000,
    	0b10000,
    	0b11110,
    	0b10000,
    	0b10000,
    	0b10000},

   	//G
	{
    	0b01110,
   	0b10001,
    	0b10000,
    	0b10111,
    	0b10001,
    	0b10001,
    	0b01111},

    	//H
	{
    	0b10001,
    	0b10001,
    	0b10001,
    	0b11111,
    	0b10001,
    	0b10001,
    	0b10001},

    	//I
	{
    	0b01110,
   	0b00100,
    	0b00100,
    	0b00100,
    	0b00100,
    	0b00100,
    	0b01110},

   	//J
	{
    	0b00111,
    	0b00010,
    	0b00010,
    	0b00010,
    	0b00010,
    	0b10010,
    	0b01100},

    	//K
	{
    	0b10001,
    	0b10010,
    	0b10100,
    	0b11000,
    	0b10100,
    	0b10010,
    	0b10001},

    	//L
	{
    	0b10000,
   	0b10000,
    	0b10000,
    	0b10000,
    	0b10000,
    	0b10000,
    	0b11111},

    	//M
	{
    	0b10001,
    	0b11011,
    	0b10101,
    	0b10101,
    	0b10001,
    	0b10001,
    	0b10001},

    	//N
	{
   	0b10001,
    	0b10001,
    	0b11001,
    	0b10101,
    	0b10011,
    	0b10001,
    	0b10001},

    	//O
	{
    	0b01110,
   	0b10001,
   	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b01110},

    	//P
	{
    	0b11110,
    	0b10001,
    	0b10001,
    	0b11110,
    	0b10000,
    	0b10000,
    	0b10000},

    	//Q
	{
    	0b01110,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10101,
    	0b10010,
    	0b01101},

    	//R
	{
    	0b11110,
    	0b10001,
    	0b10001,
    	0b11110,
    	0b10100,
    	0b10010,
    	0b10001},

    	//S
	{
    	0b01111,
    	0b10000,
    	0b10000,
    	0b01110,
    	0b00001,
    	0b00001,
    	0b11110},

    	//T
	{
    	0b11111,
    	0b00100,
    	0b00100,
    	0b00100,
    	0b00100,
    	0b00100,
    	0b00100},

    	//U
	{
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b01110},

    	//V
	{
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10001,
    	0b01010,
    	0b00100},

    	//W
	{
    	0b10001,
    	0b10001,
    	0b10001,
    	0b10101,
    	0b10101,
    	0b10101,
   	0b01010},

    	//X
	{
    	0b10001,
    	0b10001,
    	0b01010,
    	0b00100,
    	0b01010,
    	0b10001,
    	0b10001},

    	//Y
	{
   	0b10001,
   	0b10001,
    	0b10001,
    	0b01010,
    	0b00100,
    	0b00100,
    	0b00100},

    	//Z
	{
    	0b11111,
    	0b00001,
    	0b00010,
    	0b00100,
    	0b01000,
    	0b10000,
    	0b11111}};


