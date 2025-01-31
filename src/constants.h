#include "header.h"
#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int TITLE_FONT_SIZE = 60;
static const int QUOTE_FONT_SIZE = 30;
static const int BUTTON_FONT_SIZE= 22;
static const int DEFAULT_TEXT_SIZE = 25;
static const int MEDIUM_FONT_SIZE = 40;
static const int SMALL_FONT_SIZE = 20;

#define HASH_SIZE 256

static const int HASH_LENGTH = 10;
static const int SCREEN_WIDTH = 864;
static const int SCREEN_HEIGHT = 558;

extern int testvariable;
extern int MUSIC_ON;

static char * gamequote ="Brick by Brick, Breakout's a Blast";
static char * gamequote2 = "Get Ready to Shatter Records!";

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Texture* gBackgroundTexture;
extern SDL_Texture* buttonTexture; /* // Global variable to store the button texture */
extern SDL_Texture* saveButtonTexture;
extern TTF_Font* gFont;
extern FILE *highscoresptr;
extern Mix_Music* globbyMusic;
extern Mix_Music* gIntenseMusic;
extern Mix_Music* gNormalMusic;
extern Mix_Music* gBadumtss;

#define NUM_BRICK_ROWS 5
#define NUM_BRICK_COLS 9
#define BRICK_WIDTH 70
#define BRICK_HEIGHT 25
#define BRICK_GAP_X 8
#define BRICK_GAP_Y 8
#define BRICK_START_Y 60

enum ScreenState { MAIN_MENU, GAME, LOAD_GAME ,QUIT_MENU, GAME_OVER , HIGH_SCORE , SETTINGS , SAVE_GAME, KEYBOARD , KEYBOARD_QUICK};
enum MusicDesc { LOBBY,INTENSEGAME , NORMALGAME, NOTHING,BADUMTSS};

struct MusicState {
    int musicOn;
    enum MusicDesc desc;
}typedef MusicState;
extern MusicState musicState;

extern enum ScreenState prevScreen;
extern enum  ScreenState currentScreen;

extern char name[512];
extern int SELECTED_SLOT;

typedef struct Brick
{
    SDL_Rect rect;
    int health;
} Brick;

typedef struct Game_State {
    char *name;
    SDL_Rect paddleBottom;
    SDL_Rect ball;
    int ballSpeedX;
    int ballSpeedY;
    int gamePaused;
    int score;
    int temp_collision;
    int status;
    Brick bricks[NUM_BRICK_ROWS][NUM_BRICK_COLS]; /* Include the Brick array here */
} Game_State;

typedef struct {
    int autoPlay;
} Settings;

typedef struct Button {
    SDL_Rect rect;
    char *text;
    void (*onClick)(struct Button *); /*Function pointer for button click handler*/
} Button;

extern Button dummy_button;

#define NUM_GAME_STATES 3

extern Game_State localSave[NUM_GAME_STATES];

extern Game_State activeGame;

extern Settings settings;

#endif  
