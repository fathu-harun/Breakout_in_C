#ifndef SCREENS_H
#define SCREENS_H

#include "header.h"
#include "constants.h"

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_SIZE 20
#define PADDLE_SPEED 5
#define BALL_SPEED_X 3
#define BALL_SPEED_Y 3
#define NUM_BRICKS 10

void renderMain(void);
void loadGameScreen(Button *button);
void handleScreenState(void);
void quitMenu(void);
void my_close(void);
void gameOver(Button *button);
void returnToMainMenu(Button *button);
void loadSaveFile(void);
void Load_Game_State(void);
void Save_Game_State(void);
void reset_default_state(void);
void validate_Game_State(Game_State *gameState);
void initializeGame(void);
void updateGame(void);
void renderGame(void);
void MainGameLoop(Button *button);
void quitMenu(void);
void print_slot_file(int slot);
void set_default_state(Game_State *gameState);
void saveEntriesToFile(void);
void renderHighScores(Button *button);
void initializeBricks(int flag);
void updateKeyboard(void);
void Save_Game_State_toFile(void);
SDL_Rect returnRect(int x, int y, int w, int h);
void HandleSavegame(Button *button);
char *QuickGameKeyboardLoop(void);

#endif
