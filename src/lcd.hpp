#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED
#include <SDL2/SDL.h>

class LCD{
public:
    static unsigned char character[27][7];
private:
    SDL_Renderer *renderer;
    SDL_Texture *digit_texture[27];
    SDL_Color digit_color_on, digit_color_off, background;
    SDL_Rect lcd_rect;
    
    int pixel_size, column, text_len, border;
    char *text;


public:
    LCD(int _x, int _y, int _pixel_size, int _column, SDL_Renderer *_renderer);
    ~LCD();

    void setText(const char* str);
    const char* getText();
    void clr();
    void setLocationX(int x);
    void setLocationY(int y);
    int getLocationX();
    int getLocationY();
    int getWidth();
    int getHeight();
    bool isClicked(SDL_Event *event);
    
    void setBackground(int r, int g, int b);
    void render();

private:
    void createTextures();
};

#endif
