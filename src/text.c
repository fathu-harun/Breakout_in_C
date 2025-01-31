
#include "text.h"
#include "header.h"
#include "constants.h"

void renderText(const char* text, int x, int y,int fontsize)
{
    SDL_Surface* textSurface;
    SDL_Rect renderQuad;
    SDL_Color textColor;
    /* TTF_Font* font = TTF_OpenFont("resources/PatrickHand-Regular.ttf", fontsize); */

    if(TTF_SetFontSize(gFont, fontsize) == -1){
        printf("Failed to set font size! SDL_ttf Error: %s\n", TTF_GetError());
    }

    textColor.r = 0;
    textColor.g = 0;
    textColor.b = 0;
    textColor.a= 0;

    textSurface = TTF_RenderText_Solid(gFont, text, textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (textTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            renderQuad.x = x;
            renderQuad.y = y;
            renderQuad.w = textSurface->w;
            renderQuad.h = textSurface->h;
            SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);
        }

        SDL_FreeSurface(textSurface);
    }
}


/*Since to calculate the x and y positions before sending it off to render text , 
we need a font pointer to pass to TTF_SIZEText(), geetFondData , takes a pointer to text ,
 preffered font size and pointers to where it should return the dimensions*/
void getFontData(char * text, int fontsize, int * textWidth, int * textHeight){

    if(TTF_SetFontSize(gFont, fontsize) == -1){
        printf("Failed to set font size! SDL_ttf Error: %s\n", TTF_GetError());
    }
    /* temp_Font = TTF_OpenFont("resources/PatrickHand-Regular-temp.ttf", fontsize); */
    if (gFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }
    if(TTF_SizeText(gFont, text,textWidth,textHeight)){
        printf("Unable to get text size! SDL_ttf Error: %s\n", TTF_GetError());}

}

void print_centre_text(char *text, int fontsize, int y) {

    int textWidth, textHeight;

    getFontData(text, fontsize, &textWidth, &textHeight);

    if (!(textWidth > SCREEN_WIDTH / 2)) {
        /* Existing code for centering short lines */
        int textX = (SCREEN_WIDTH - textWidth) / 2;
        int textY = y - textHeight / 2;
        renderText(text, textX, textY, fontsize);
    } else {
        /* TODO Write code to wrap around long text*/
        printf("Text %s is too long to fit in one line\n", text); 

        printf("width is %d and height is %d\n", textWidth, textHeight);
    }
}
