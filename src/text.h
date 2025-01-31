#ifndef TEXT_H
#define TEXT_H
void renderText(const char* text, int x, int y,int fontsize);
void getFontData(char * text, int fontsize, int * textWidth, int * textHeight);
void print_centre_text(char *text, int fontsize, int y);
#endif
