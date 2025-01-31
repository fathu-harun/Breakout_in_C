#include "main.h"
#include "text.h"
#include "buttons.h"
#include "constants.h"
#include "scores.h"
#include "header.h"
#include "screens.h"

FILE *highscoresptr;
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Texture *gBackgroundTexture;
SDL_Texture *buttonTexture;
TTF_Font *gFont;
SDL_Texture *saveButtonTexture;
enum ScreenState prevScreen;
enum ScreenState currentScreen;
enum MusicDesc currentMusic;
Game_State localSave[NUM_GAME_STATES];
entry localentries[NUM_SCORES];
Settings settings;
char temp_name[512];
enum ScreenState currentScreen;
enum ScreenState prevScreen;
Mix_Music *globbyMusic;
Mix_Music *gIntenseMusic;
Mix_Music *gNormalMusic;
Mix_Music *gBadumtss;
MusicState musicState;


int init(void)
{
    int imgFlags ;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    gWindow = SDL_CreateWindow("Background Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    
    imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }
    
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return 0;
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 0;
    }

    musicState.musicOn = 0;
    musicState.desc = LOBBY;

    currentScreen = MAIN_MENU;
    prevScreen = MAIN_MENU;

    printf("init done\n");
    return 1;
}

SDL_Texture *loadTexture(const char *path)
{
    SDL_Texture *texture;
    SDL_Surface *loadedSurface;
    
    loadedSurface = IMG_Load(path);
    
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    texture  = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    return texture;
}

int loadMedia(void)
{
    gBackgroundTexture = loadTexture("resources/background-panel.png");
    buttonTexture = loadTexture("resources/button-small.png");
    saveButtonTexture = loadTexture("resources/save_file_button.png");

    if (gBackgroundTexture == NULL || buttonTexture == NULL || saveButtonTexture == NULL)
    {
        printf("Failed to load texture images !\n");
        return 0;
    }

    gFont = TTF_OpenFont("resources/LondrinaSolid-Light.ttf", 60);

    if (gFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());

        return 0;
    }

    globbyMusic = Mix_LoadMUS("resources/music/LobbyMusic_MrBean.wav");
    gIntenseMusic = Mix_LoadMUS("resources/music/RipTear.wav");
    gNormalMusic = Mix_LoadMUS("resources/music/Game_Youareamemory.wav");
    gBadumtss = Mix_LoadMUS("resources/music/ba-dum-tss.wav");
    if (globbyMusic == NULL || gIntenseMusic == NULL || gNormalMusic == NULL || gBadumtss == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }

    loadSaveFile();

    loadEntries();

    loadSettings();

    return 1;
}

void loadSettings(void)
{
    FILE *settings_file = fopen("resources/game_load_files/settings.txt", "r");

    if (settings_file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fscanf(settings_file, "%d\n", &settings.autoPlay);

    fclose(settings_file);
}

void loadSaveFile(void)
{
    int i , j,k;
    FILE *slot_file = fopen("resources/game_load_files/savefile.txt", "r");

    if (slot_file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    for (i=0; i < NUM_GAME_STATES; i++)
    {
        char temp[512];
        fscanf(slot_file, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", temp, &localSave[i].paddleBottom.x, &localSave[i].paddleBottom.y, &localSave[i].paddleBottom.w, &localSave[i].paddleBottom.h, &localSave[i].ball.x, &localSave[i].ball.y, &localSave[i].ball.w, &localSave[i].ball.h, &localSave[i].ballSpeedX, &localSave[i].ballSpeedY, &localSave[i].gamePaused, &localSave[i].score, &localSave[i].temp_collision, &localSave[i].status);
        localSave[i].name = strdup(temp);


        for (j = 0; j < NUM_BRICK_ROWS; j++)
        {
            for ( k = 0; k < NUM_BRICK_COLS; k++)
            {
                fscanf(slot_file, "%d ", &localSave[i].bricks[j][k].health);
            }
        }

        if (localSave[i].status == -1)
        {
            set_default_state(&localSave[i]);
        }
    }

    fclose(slot_file);
}

void my_close(void)
{
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    Mix_FreeMusic( globbyMusic );
    globbyMusic = NULL;

    TTF_CloseFont(gFont);
    gFont = NULL;
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    saveEntriesToFile();
    Save_Game_State_toFile();
    SaveSettingsToFile();

    printf("\nEverything Cleared , Quitting Game \n");

    exit(0);
}

void SaveSettingsToFile(void)
{
    FILE *settings_file = fopen("resources/game_load_files/settings.txt", "w");

    if (settings_file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(settings_file, "%d\n", settings.autoPlay);

    fclose(settings_file);
}

void PlayMusic(MusicState *musicState, int music)
{
    if (musicState->musicOn ==1)
    {
        if (musicState->desc == music)
        {
            return;
        }
        else
        {

            switch(music)
            {
                case LOBBY:
                    musicState->desc = LOBBY;
                    Mix_FadeInMusic(globbyMusic,-1,1000);
                    /* Mix_PlayMusic(globbyMusic,-1); */ 
                    break;
                case INTENSEGAME:
                    musicState->desc = INTENSEGAME;
                    Mix_FadeInMusic(gIntenseMusic,-1,2000);
                    /* Mix_PlayMusic(gIntenseMusic,-1); */ 
                    break;
                case NORMALGAME:
                    musicState->desc = NORMALGAME;
                    Mix_PlayMusic(gNormalMusic,-1);
                    break;
                case NOTHING:
                    musicState->desc = NOTHING;
                    Mix_HaltMusic();
                    break;
                case BADUMTSS:
                    musicState->desc = BADUMTSS;
                    Mix_PlayMusic(gBadumtss,1);
                    break;
            }  
        }
    }
    else if(musicState->musicOn == 0)
    {
        switch(music)
        {
            case LOBBY:
                musicState->desc = LOBBY;   
                musicState->musicOn = 1;
                Mix_PlayMusic(globbyMusic,-1);
                break;
            case INTENSEGAME:
                musicState->desc = INTENSEGAME;
                musicState->musicOn = 1;
                Mix_PlayMusic(gIntenseMusic,-1);
                break;
            case NORMALGAME:
                musicState->desc = NORMALGAME;
                musicState->musicOn = 1;
                Mix_PlayMusic(gNormalMusic,-1);
                break;
            case NOTHING:
                musicState->desc = NOTHING;
                Mix_HaltMusic();
                break;
            case BADUMTSS:
                musicState->desc = BADUMTSS;
                Mix_PlayMusic(gBadumtss,1);
                break;
        }
    }
    else if(musicState->musicOn == 2)
    {
        Mix_ResumeMusic();
        musicState->musicOn = 1;
    }
}

void PauseMusic(MusicState *state)
{
    if (state->musicOn == 1)
    {
        state->musicOn = 2;
        Mix_PauseMusic();
    }
} 

void renderMain(void)
{
    int i; 
    int textWidth; 
    int textHeight; 
    int numButtons; 
    int quit;
    Button *StartGame, *LoadGame,*HighScore , *Settings;
    SDL_Event e;
    Button *buttons_main[10];

    PlayMusic(&musicState,0);

    numButtons = 0;
    
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor (gRenderer, 0x00, 0x00, 0x00, 0x00);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
    SDL_SetWindowTitle(gWindow, "Main Menu");

    print_centre_text("Breakout", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);

    getFontData(gamequote, QUOTE_FONT_SIZE, &textWidth, &textHeight);

    print_centre_text(gamequote, QUOTE_FONT_SIZE, 0.25 * SCREEN_HEIGHT);
    print_centre_text(gamequote2, QUOTE_FONT_SIZE, 0.25 * SCREEN_HEIGHT + textHeight);

    StartGame = createButton(295, 240, 120, 50, "Quick Game", &startGame);
    renderButton(StartGame);
    addButton(buttons_main, StartGame, &numButtons);

    LoadGame = createButton(425, 240, 120, 50, "Load Game", &Redirect_to_Game);
    renderButton(LoadGame);
    addButton(buttons_main, LoadGame, &numButtons);

    HighScore = createButton(295, 300, 120, 50, "High Score", &Redirect_to_HighScore);
    renderButton(HighScore);
    addButton(buttons_main, HighScore, &numButtons);

    Settings = createButton(425, 300, 120, 50, "Settings", &Redirect_to_Settings);
    renderButton(Settings);
    addButton(buttons_main, Settings, &numButtons);
    
 /*   */
    SDL_RenderPresent(gRenderer);
    quit = 0;

    while (!quit)
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
                printf("Key pressed: %d\n", e.key.keysym.sym);
                quit = 1;
                currentScreen = QUIT_MENU; 
                prevScreen = MAIN_MENU;
                handleScreenState();
                printf("Load Quit Game Screen\n");
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (i = 0; i < numButtons; i++)
                {
                    if ((x >= buttons_main[i]->rect.x) && (x <= (buttons_main[i]->rect.x + buttons_main[i]->rect.w)) &&
                        (y >= buttons_main[i]->rect.y) && (y <= (buttons_main[i]->rect.y + buttons_main[i]->rect.h)))
                    {
                        buttons_main[i]->onClick(buttons_main[i]);
                    }
                }
            }
        }
    }


int mainLoop(void)
{
    if (!init())
    {
        printf("Failed to initialize!\n");
        return 1;
    }

    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        return 2;
    }

    currentScreen = MAIN_MENU;
    
    handleScreenState();

    printf("back to mainLoop\n");
    my_close();
    return 0;
}



#ifdef _WIN32

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int result = mainLoop();
    return result;
}

#else
int main(int argc, char *argv[])
{
    int result = mainLoop();
    return result;
}
#endif
