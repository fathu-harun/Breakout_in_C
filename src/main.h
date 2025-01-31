#ifndef MAIN_H
#define MAIN_H
#include "header.h"
#include "constants.h"
int init(void);
SDL_Texture* loadTexture(const char* path);
int loadMedia(void);
void my_close(void);
int mainLoop(void);
void loadSettings(void);
void SaveSettingsToFile(void);
void PlayMusic(MusicState *state,int number);
void PauseMusic(MusicState *state);

#endif
