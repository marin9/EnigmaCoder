#include <SDL2/SDL.h>
#include <stdio.h>
#include "lcd.hpp"
#include "keyboard.hpp"
#include "gtext.hpp"
#include "plugboard.hpp"
#include "enigma.hpp"
#define WIDTH 700
#define HEIGHT 500
#define LCD_PIXEL_SIZE 4
#define LCD_COLUMN 21

class Application{
	enum Input {IN, ROTOR1, ROTOR2, ROTOR3, PLUGBOARD};

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	LCD *lcd1, *lcd2, *lcdRotor1, *lcdRotor2, *lcdRotor3;
	Plugboard *plugboard;
	Keyboard *keyboard;
	Enigma *enigma;
	
	Input input;
	bool run;

	char text1[LCD_COLUMN+1], text2[LCD_COLUMN+1];

public:
	Application(int _width, int _height);
	~Application();

	void start();
private:
	void getEvents();
	void render();
};



Application::Application(int _width, int _height){
	window=SDL_CreateWindow("Enigma machine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_OPENGL);
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	lcd1=new LCD(0, 5, LCD_PIXEL_SIZE, LCD_COLUMN, renderer);
	lcd1->setLocationX((WIDTH-lcd1->getWidth())/2);	
	lcd2=new LCD(lcd1->getLocationX(), lcd1->getHeight(), LCD_PIXEL_SIZE, LCD_COLUMN, renderer);	

	lcdRotor3=new LCD(0, lcd2->getHeight()+lcd2->getLocationY()+15, LCD_PIXEL_SIZE, 1, renderer);	
	lcdRotor3->setLocationX(WIDTH/2-(lcdRotor3->getWidth()*3/2));
	lcdRotor2=new LCD(lcdRotor3->getLocationX()+lcdRotor3->getWidth(), 
			lcd2->getHeight()+lcd2->getLocationY()+15, LCD_PIXEL_SIZE, 1, renderer);	
	lcdRotor1=new LCD(lcdRotor2->getLocationX()+lcdRotor2->getWidth(), 
			lcd2->getHeight()+lcd2->getLocationY()+15, LCD_PIXEL_SIZE, 1, renderer);	
 
	lcd1->setBackground(0, 255, 0);
	lcd2->setBackground(50, 120, 255);
	lcdRotor1->setBackground(150, 120, 70);
	lcdRotor2->setBackground(150, 120, 70);
	lcdRotor3->setBackground(150, 120, 70);
	

	plugboard=new Plugboard(WIDTH*0.1/2, lcdRotor1->getLocationY()+lcdRotor1->getHeight()+5, WIDTH*0.9, HEIGHT*0.1, 3, renderer);
	plugboard->setBackground(150, 120, 70);
	
	keyboard=new Keyboard(WIDTH*0.1/2, HEIGHT*0.6, WIDTH*0.9, HEIGHT*0.3, 3, renderer);

	enigma=new Enigma();

	input=IN;
	run=true;

	lcd1->setText(" ");
	lcd2->setText(" ");
	for(int i=0;i<LCD_COLUMN+1;++i) 
		text1[i]=text2[i]=' ';
	text1[LCD_COLUMN]=text2[LCD_COLUMN]='\0';
}

Application::~Application(){
	delete lcd1;
	delete lcd2;
	delete lcdRotor1;
	delete lcdRotor2;
	delete lcdRotor3;
	delete plugboard;
	delete keyboard;
	delete enigma;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Application::start(){
	while(run){
		getEvents();
		render();
		SDL_Delay(20);
	}
}

void Application::getEvents(){
	int key;
 	while(SDL_PollEvent(&event)){
    		if(event.type==SDL_QUIT){
			run=false;
			break;
		}

		key=keyboard->getPressedKey(&event);

		if(input==IN && key!=-1){
			for(int i=0;i<LCD_COLUMN-1;++i){
				text1[i]=text1[i+1];
				text2[i]=text2[i+1];
			}

			text1[LCD_COLUMN-1]=key;
			if(key==' ') text2[LCD_COLUMN-1]=' ';
			else text2[LCD_COLUMN-1]=enigma->encodeChar(key);
			text1[LCD_COLUMN]='\0';
			text2[LCD_COLUMN]='\0';

			lcd1->setText(text1);
			lcd2->setText(text2);

		}else if(input==ROTOR1 && key!=-1){		
			enigma->setRotor3(key);

		}else if(input==ROTOR2 && key!=-1){
			enigma->setRotor2(key);

		}else if(input==ROTOR3 && key!=-1){
			enigma->setRotor1(key);

		}else if(input==PLUGBOARD && key!=-1){
			bool stat;
			if(key==' ') stat=plugboard->removeChar();
			else stat=plugboard->appendChar(key);

			if(stat){
				enigma->clearPlugboard();
				
				char* pdata=plugboard->getData();
				for(int i=0;i<13*2;i=i+2){
					if(pdata[i]==0) break;
					enigma->putInPlugboard(pdata[i], pdata[i+1]);
				}
			}
		}


		if(lcd1->isClicked(&event)){
			input=IN;
			lcd1->setBackground(0, 255, 0);
			lcdRotor1->setBackground(150, 120, 70);
			lcdRotor2->setBackground(150, 120, 70);
			lcdRotor3->setBackground(150, 120, 70);
			plugboard->setBackground(150, 120, 70);
		}else if(lcdRotor1->isClicked(&event)){
			input=ROTOR1;
			lcd1->setBackground(150, 120, 70);
			lcdRotor1->setBackground(255, 190, 0);
			lcdRotor2->setBackground(150, 120, 70);
			lcdRotor3->setBackground(150, 120, 70);
			plugboard->setBackground(150, 120, 70);
		}else if(lcdRotor2->isClicked(&event)){
			input=ROTOR2;
			lcd1->setBackground(150, 120, 70);
			lcdRotor1->setBackground(150, 120, 70);
			lcdRotor2->setBackground(255, 190, 0);
			lcdRotor3->setBackground(150, 120, 70);
			plugboard->setBackground(150, 120, 70);
		}else if(lcdRotor3->isClicked(&event)){
			input=ROTOR3;
			lcd1->setBackground(150, 120, 70);
			lcdRotor1->setBackground(150, 120, 70);
			lcdRotor2->setBackground(150, 120, 70);
			lcdRotor3->setBackground(255, 190, 0);
			plugboard->setBackground(150, 120, 70);
		}else if(plugboard->isClicked(&event)){
			input=PLUGBOARD;
			lcd1->setBackground(150, 120, 70);
			lcdRotor1->setBackground(150, 120, 70);
			lcdRotor2->setBackground(150, 120, 70);
			lcdRotor3->setBackground(150, 120, 70);
			plugboard->setBackground(255, 190, 0);
		}
    	}

	char str[2]={' ', '\0'};
	str[0]=enigma->getRotor3();
	lcdRotor1->setText(str);

	str[0]=enigma->getRotor2();
	lcdRotor2->setText(str);

	str[0]=enigma->getRotor1();
	lcdRotor3->setText(str);
}
 
void Application::render(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
         
	lcd1->render();
	lcd2->render();
	lcdRotor1->render();
	lcdRotor2->render();
	lcdRotor3->render();
	plugboard->render();
	keyboard->render();

        SDL_RenderPresent(renderer);
}



int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO); 
                     
	Application enigmaMachine(WIDTH, HEIGHT);
	enigmaMachine.start();
    
    	SDL_Quit();
    	return 0;
}
