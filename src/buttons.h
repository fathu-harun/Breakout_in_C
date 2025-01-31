#ifndef BUTTONS_H
#define BUTTONS_H

#include "header.h"
#include "constants.h"


void renderButton(const Button *button);
Button *createButton(int x, int y, int width, int height, const char *buttonText, void (*clickHandler)(Button *b));
void buttonClicked(Button *button);
void addButton(Button *buttons[], Button *button, int *numButtons);
void Redirect_to_Game(Button *button);
void quitGame(Button *button);
void return_back(Button *button);
void startGame(Button *button);
void renderSaveButton(const Button *button, Game_State gameState);
void print_slot(Button *button);
void Redirect_to_HighScore(Button *button);
void returnToMainMenu(Button *button);
void Redirect_to_Settings(Button *button);
void Redirect_to_SaveGameScreen(Button *button);
void ResetSaveFile(Button *button);
void toggleAutomaticMode(Button *button);
void DummyFunction(Button *button);
void DummyFunction_Empty(void);

#endif
