#include "gtext.hpp"
#include <stdio.h>

GText::GText(int _x, int _y, int _size, SDL_Renderer *_renderer){
	renderer=_renderer;

	text_rect.x=_x;
	text_rect.y=_y;
	pixel_size=_size;
	border=pixel_size;
	text_rect.w=0;
	text_rect.h=2*border+7*pixel_size;

	text=NULL;
	for(int i=0;i<27;++i) digit_texture[i]=NULL;
	transparent=false;
	setColors(200, 200, 200, 50, 50, 50);
}

GText::~GText(){
	if(text!=NULL) free(text);
	for(int i=0;i<27;++i)
		if(digit_texture[i]!=NULL)
			SDL_DestroyTexture(digit_texture[i]);	
}

void GText::setText(const char* str){
	if(text!=NULL) free(text);
	text_len=0;
	while(str[text_len]!='\0') ++text_len;

	text=(char*)malloc((text_len+1)*sizeof(char));
	for(int i=0;i<text_len;++i){
		if((str[i]>='A' && str[i]<='Z') || str[i]==' ') text[i]=str[i];
		else if(str[i]>='a' && str[i]<='z') text[i]=str[i]-32;
		else text[i]=' ';
	}
	text[text_len]='\0';

	text_rect.w=2*border+5*pixel_size*text_len+(text_len-1)*pixel_size;
}

const char* GText::getText(){
	return text;
}

void GText::setLocationX(int x){
	text_rect.x=x;
}

void GText::setLocationY(int y){
	text_rect.y=y;
}

int GText::getLocationX(){
	return text_rect.x;
}

int GText::getLocationY(){
	return text_rect.y;
}

int GText::getWidth(){
	return text_rect.w;
}

int GText::getHeight(){
	return text_rect.h;
}
    
void GText::setColors(int text_r, int text_g, int text_b, int back_r, int back_g, int back_b){
	text_color.r=text_r;
	text_color.g=text_g;
	text_color.b=text_b;
	text_color.a=255;

	background.r=back_r;
	background.g=back_g;
	background.b=back_b;
	background.a=255;

	createTextures();
}

void GText::setTransparent(bool _transparent){
	transparent=_transparent;
	createTextures();
}

void GText::render(){
	if(!transparent){
		SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
		SDL_RenderFillRect(renderer, &text_rect);
	}	
	
	SDL_Rect tmpRect;
	tmpRect.w=pixel_size*5;
	tmpRect.h=pixel_size*7;
	
	for(int i=0;i<text_len;++i){	
		tmpRect.x=text_rect.x+border+(tmpRect.w+pixel_size)*i;
		tmpRect.y=text_rect.y+border;

		if(text[i]==' ') SDL_RenderCopy(renderer, digit_texture[0], NULL, &tmpRect);
		else SDL_RenderCopy(renderer, digit_texture[(int)(text[i]-64)], NULL, &tmpRect);
	}
}

void GText::createTextures(){
    SDL_Rect pixel;
    pixel.w=pixel_size;
    pixel.h=pixel_size;

    for(int i=0;i<27;++i){
	if(digit_texture[i]==NULL) 
		digit_texture[i]=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
						   pixel_size*5, pixel_size*7);	
	 
	SDL_SetRenderTarget(renderer, digit_texture[i]);

        if(!transparent){
		SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
		SDL_RenderClear(renderer);
	}else{
		SDL_SetTextureBlendMode(digit_texture[i], SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, 0);
		SDL_RenderClear(renderer);
	}
	
		
	SDL_SetRenderDrawColor(renderer, text_color.r, text_color.g, text_color.b, text_color.a);

	for(int x=0;x<5;++x){
		for(int y=0;y<7;++y){
			if((character[i][y]>>(4-x)) & 0x01){				
				pixel.x=x*pixel_size;
				pixel.y=y*pixel_size;
				SDL_RenderFillRect(renderer, &pixel);
			} 
		}
	}
    }
    SDL_SetRenderTarget(renderer, NULL);
}



unsigned char GText::character[27][7]={
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




