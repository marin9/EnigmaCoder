#ifndef GTEXT_H_INCLUDED
#define GTEXT_H_INCLUDED
#include <SDL2/SDL.h>

class GText{
public:
    static unsigned char character[27][7];
private:
    SDL_Renderer *renderer;
    SDL_Texture *digit_texture[27];
    SDL_Color text_color, background;
    SDL_Rect text_rect;
    
    int pixel_size, text_len, border;
    bool transparent;
    char *text;


public:
    GText(int _x, int _y, int _size, SDL_Renderer *_renderer);
    ~GText();

    void setText(const char* str);
    const char* getText();
    void setLocationX(int x);
    void setLocationY(int y);
    int getLocationX();
    int getLocationY();
    int getWidth();
    int getHeight();
    
    void setColors(int text_r, int text_g, int text_b, int back_r, int back_g, int back_b);
    void setTransparent(bool _transparent);
    void render();

private:
    void createTextures();
};

#endif
