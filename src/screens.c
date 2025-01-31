#include "screens.h"
#include "constants.h"
#include <stdio.h>
#include "text.h"
#include "buttons.h"
#include "scores.h"
#include "main.h"

int intenseFlag;
char name[512];
Button dummy_button;

void print_slot_file(int slot)
{
    if (!slot || slot > 3 || slot < 1)
    {
        printf("Invalid Slot\n");
    }
    else
    {
        printf("Printing Slot %d Information\n", slot);
        printf("Paddle Bottom x is %d and y is %d\n", localSave[slot - 1].paddleBottom.x, localSave[slot - 1].paddleBottom.y);
        printf("Ball x is %d and y is %d\n", localSave[slot - 1].ball.x, localSave[slot - 1].ball.y);
        printf("Ball Speed x is %d and y is %d\n", localSave[slot - 1].ballSpeedX, localSave[slot - 1].ballSpeedY);
        printf("Game Paused is %d\n", localSave[slot - 1].gamePaused);
        printf("Score is %d\n", localSave[slot - 1].score);

        if (localSave[slot - 1].status == -1)
        {
            printf("Game is lost\n");
        }
        else if (localSave[slot - 1].status == 0)
        {
            printf("Game Savefile is empty\n");
        }
        else
        {
            printf("SaveFile is in use\n");
        }
    }
}

Game_State game_state_array[3];

void loadGameScreen(Button *button)
{
    int i,quit,centreX,numButtons;
    SDL_Event e;
    Button *backButton , *SaveFile1,*SaveFile2,*SaveFile3,*buttons_temp[20];


    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "Load Game");

    numButtons =0;

    backButton = createButton(20, 20, 80, 40, "Back", &returnToMainMenu);
    renderButton(backButton);
    addButton(buttons_temp, backButton, &numButtons);
    centreX = SCREEN_WIDTH / 2;

    SaveFile1 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23, 600, 100, "Slot 1", &print_slot);
    renderSaveButton(SaveFile1, localSave[0]);
    addButton(buttons_temp, SaveFile1, &numButtons);

    SaveFile2 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23 + 110, 600, 100, "Slot 2", &print_slot);
    renderSaveButton(SaveFile2, localSave[1]);
    addButton(buttons_temp, SaveFile2, &numButtons);

    SaveFile3 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23 + 220, 600, 100, "Slot 3", &print_slot);
    renderSaveButton(SaveFile3, localSave[2]);
    addButton(buttons_temp, SaveFile3, &numButtons);

    print_centre_text("Save Files", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);


    SDL_RenderPresent(gRenderer);

    
    quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT ||(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
                quit = 1;
                currentScreen = QUIT_MENU;
                prevScreen = LOAD_GAME;
                handleScreenState();
                printf("back to loadScreen \n");
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for ( i = 0; i < numButtons; i++)
                {
                    if ((x >= buttons_temp[i]->rect.x) && (x <= (buttons_temp[i]->rect.x + buttons_temp[i]->rect.w)) &&
                        (y >= buttons_temp[i]->rect.y) && (y <= (buttons_temp[i]->rect.y + buttons_temp[i]->rect.h)))
                    {
                        buttons_temp[i]->onClick(buttons_temp[i]);
                    }
                }
            }
        }
    }
}

Game_State activeGame;

SDL_Rect left_wall;
SDL_Rect right_wall;
SDL_Rect top_wall;
SDL_Rect bottom_wall;

SDL_Color returnBrickColor(int health)
{
    SDL_Color BLACK = {0, 0, 0, 255};
    SDL_Color BLACK_20 = {0, 0, 0, 51};  
    SDL_Color BLACK_40 = {0, 0, 0, 102}; 
    SDL_Color BLACK_60 = {0, 0, 0, 153}; 
    SDL_Color BLACK_80 = {0, 0, 0, 204};
    SDL_Color color;
    switch (health)
    {
    case 1:
        color = BLACK_20;
        break;
    case 2:
        color = BLACK_40;
        break;
    case 3:
        color = BLACK_60;
        break;
    case 4:
        color = BLACK_80;
        break;
    case 5:
        color = BLACK;
        break;
    default:
        color = BLACK; 
        break;
    }
    return color;
}

Brick createBrick(int x, int y, int w, int h, int type)
{
    Brick newBrick;
    newBrick.health = type;
    newBrick.rect.x = x;
    newBrick.rect.y = y;
    newBrick.rect.w = w;
    newBrick.rect.h = h;
    return newBrick;
}

void initializeBricks(int flag)
{
    int BRICK_START_X , x, y ,i,j;
    printf("Initializing Bricks\n");
    BRICK_START_X = (SCREEN_WIDTH - (NUM_BRICK_COLS * BRICK_WIDTH + (NUM_BRICK_COLS - 1) * BRICK_GAP_X)) / 2; 
    x = BRICK_START_X;
    y = BRICK_START_Y;

    for ( i = 0; i < NUM_BRICK_ROWS; i++)
    {
        for ( j = 0; j < NUM_BRICK_COLS; j++)
        {
            if (flag == 1)
            {
                activeGame.bricks[i][j] = createBrick(x, y, BRICK_WIDTH, BRICK_HEIGHT, activeGame.bricks[i][j].health);
            }
            else
            {
                activeGame.bricks[i][j] = createBrick(x, y, BRICK_WIDTH, BRICK_HEIGHT, NUM_BRICK_ROWS - (i));
            }
            x += BRICK_WIDTH + BRICK_GAP_X;
        }
        x = BRICK_START_X;
        y += BRICK_HEIGHT + BRICK_GAP_Y;
    }
}

Game_State game_state;
int gameinProgress = 0;
int keypressed= 0;

void set_default_state(Game_State *gameState)
{
    int i ,j;
    printf("Resetting default\n");
    gameState->paddleBottom = returnRect(SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, 0.83 * SCREEN_HEIGHT, 100, 20);
    gameState->ball = returnRect(SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, 20, 20);

    /*gameState->paddleBottom = (SDL_Rect){SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, 0.83 * SCREEN_HEIGHT, 100, 20};
    gameState->ball = (SDL_Rect){SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, 20, 20};
    */
    gameState->ballSpeedX = BALL_SPEED_X;
    gameState->ballSpeedY = BALL_SPEED_Y;
    gameState->gamePaused = 0;
    gameState->score = 0;
    printf("score has been set to zero\n");
    gameState->temp_collision = 0;
    gameState->status = 0;

    for (i = 0; i < NUM_BRICK_ROWS; i++)
    {
        for (j = 0; j < NUM_BRICK_COLS; j++)
        {
            gameState->bricks[i][j].health = NUM_BRICK_ROWS - i;
        }
    }
}

void OverwriteSlot(Button *button) {
    int slot_num;

    char *temp;
    temp = QuickGameKeyboardLoop();
    activeGame.name = strdup(temp);


    sscanf(button->text, "Slot %d", &slot_num);
    if(slot_num>0 && slot_num<4){
        printf("Slot %d has been clicked\n",slot_num);
        print_slot_file(slot_num);
        if (localSave[slot_num-1].status==1){
            printf("Overwrite Slot %d\n",slot_num); 
        }

        localSave[slot_num-1] = activeGame;
    }
    else{
        printf("Invalid Slot\n");
    }
    prevScreen = MAIN_MENU;
    currentScreen = MAIN_MENU;
    handleScreenState();

}

void Load_Game_State(void)
{
    printf("active game status is %d\n", activeGame.status);

    if (SELECTED_SLOT > 0 && SELECTED_SLOT < 4)
    {
        if(localSave[SELECTED_SLOT-1].status==-1){
            set_default_state(&activeGame);
            initializeBricks(0);
            localSave[SELECTED_SLOT-1] = activeGame;
        }
        else{
            printf("Loading from slot %d\n", SELECTED_SLOT);
            activeGame = localSave[SELECTED_SLOT - 1];
            initializeBricks(1);
        }

    }
    else if (SELECTED_SLOT == 0 && activeGame.status == 1)
    {
        printf("Continuing from local save\n");
        initializeBricks(1);
    }
    else
    {
        printf("GameState is reset\n");
        set_default_state(&activeGame);
        initializeBricks(0);
    }
}

void Save_Game_State(void)
{
    if (activeGame.status == 0)
        activeGame.status = 1;

    if (SELECTED_SLOT == 0)
    {
        printf("Game is not updated with local save\n");
    }
    else
    {
        printf("Saving in local slot %d\n", SELECTED_SLOT);
        localSave[SELECTED_SLOT - 1] = activeGame;
    }
}

void reset_default_state(void)
{
    set_default_state(&activeGame);
}

void validate_Game_State(Game_State *gameState)
{
    if (gameState->ball.x < 66 || gameState->ball.x > SCREEN_WIDTH - 65 || gameState->ball.y < 45 || gameState->ball.y > 485)
    {
        printf("Error : The Ball seems to have disappeared\n");
        
    }
}

SDL_Rect returnRect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

void initializeGame(void)
{
    Load_Game_State();
    left_wall = returnRect(66, 55, 5, 420);
    right_wall = returnRect(SCREEN_WIDTH - 71, 55, 5, 420);
    top_wall = returnRect(83, 45, 700, 5);
    bottom_wall = returnRect(83, 480, 700, 5);

    /*left_wall = (SDL_Rect){66, 55, 5, 420};
    right_wall = (SDL_Rect){SCREEN_WIDTH - 71, 55, 5, 420};
    top_wall = (SDL_Rect){83, 45, 700, 5};
    bottom_wall = (SDL_Rect){83, 480, 700, 5};*/
}

int isMinimized(void)
{
    SDL_WindowFlags flags = SDL_GetWindowFlags(gWindow);
    return flags & SDL_WINDOW_MINIMIZED;
}

int isFocused(void)
{
    SDL_WindowFlags flags = SDL_GetWindowFlags(gWindow);
    return flags & SDL_WINDOW_INPUT_FOCUS;
}

void updateGame(void)
{
    int i, j,allBlocksDestroyed;
    int expectedPaddlePosition;
    const int padding_x = 70;
    const Uint8 *currentKeyStates ;
    if (isMinimized() || !isFocused())
    {
        return;
    }

    if(intenseFlag==0 && activeGame.score>20){
        intenseFlag=1;
        PlayMusic(&musicState, INTENSEGAME);
    }

    if(activeGame.status==0) activeGame.status = 1;
    
    currentKeyStates= SDL_GetKeyboardState(NULL);
    

    if (!activeGame.gamePaused)
    {
        if (currentKeyStates[SDL_SCANCODE_LEFT] && activeGame.paddleBottom.x > padding_x)
        {
            if (currentKeyStates[SDL_SCANCODE_LALT])
            {
                activeGame.paddleBottom.x -= PADDLE_SPEED;
            }
            activeGame.paddleBottom.x -= PADDLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] && activeGame.paddleBottom.x < SCREEN_WIDTH - PADDLE_WIDTH - padding_x)
        {
            if (currentKeyStates[SDL_SCANCODE_LALT])
            {
                activeGame.paddleBottom.x += PADDLE_SPEED;
            }
            activeGame.paddleBottom.x += PADDLE_SPEED;
        }
        activeGame.ball.x -= activeGame.ballSpeedX;
        activeGame.ball.y -= activeGame.ballSpeedY;

        if (SDL_HasIntersection(&activeGame.ball, &left_wall))
        {
            activeGame.ballSpeedX = -activeGame.ballSpeedX; 
        }

        if (SDL_HasIntersection(&activeGame.ball, &right_wall))
        {
            activeGame.ballSpeedX = -activeGame.ballSpeedX; 
        }

        if (SDL_HasIntersection(&activeGame.ball, &top_wall))
        {
            activeGame.ballSpeedY = -activeGame.ballSpeedY; 
        }
        allBlocksDestroyed = 1;
        
        for (i = 0; i < NUM_BRICK_ROWS; i++)
        {
            for ( j = 0; j < NUM_BRICK_COLS; j++)
            {
                if (!(activeGame.bricks[i][j].health == 0) && SDL_HasIntersection(&activeGame.ball, &activeGame.bricks[i][j].rect))
                {

                    activeGame.bricks[i][j].health -= 1;
                    activeGame.ballSpeedY = -activeGame.ballSpeedY; 
                    activeGame.ball.y -= activeGame.ballSpeedY - 5;
                    activeGame.score++;
                }
                if (activeGame.bricks[i][j].health > 0)
                {
                    allBlocksDestroyed = 0;
                }
            }
        }
        if (allBlocksDestroyed)
        {
            activeGame.gamePaused = 1;
        }

        if (SDL_HasIntersection(&activeGame.ball, &activeGame.paddleBottom))
        {
            activeGame.ballSpeedY = -activeGame.ballSpeedY;
            activeGame.ball.y -= activeGame.ballSpeedY + 5;
        }

        if (SDL_HasIntersection(&activeGame.ball, &bottom_wall))
        {
            activeGame.gamePaused = 1;
        }

        if (settings.autoPlay)
        {
            expectedPaddlePosition = activeGame.ball.x - (activeGame.paddleBottom.w / 2);
            activeGame.paddleBottom.x = expectedPaddlePosition;
            if (expectedPaddlePosition < 0)
            {
                expectedPaddlePosition = 0;
            }
            else if (expectedPaddlePosition > SCREEN_WIDTH - activeGame.paddleBottom.w)
            {
                expectedPaddlePosition = SCREEN_WIDTH - activeGame.paddleBottom.w;
            }
        }
    }
    else
    {
        SDL_Delay(2000);

        activeGame.status = -1;

        Save_Game_State();

        currentScreen = GAME_OVER;
        handleScreenState();
    }

}

void renderGame(void)
{
    char scoreText[20];
    int  i, j;
    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
    sprintf(scoreText, "[Score: %d]", activeGame.score);
    print_centre_text(scoreText, QUOTE_FONT_SIZE, 0.55 * SCREEN_HEIGHT);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &activeGame.paddleBottom);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &left_wall);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &top_wall);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &right_wall);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &bottom_wall);

    SDL_RenderFillRect(gRenderer, &activeGame.ball);

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (i = 0; i < NUM_BRICK_ROWS; i++)
    {
        for ( j = 0; j < NUM_BRICK_COLS; j++)
        {
            if (!(activeGame.bricks[i][j].health == 0))
            {
                SDL_Color temp = returnBrickColor(activeGame.bricks[i][j].health);
                SDL_SetRenderDrawColor(gRenderer, temp.r, temp.g, temp.b, temp.a);
                SDL_RenderFillRect(gRenderer, &activeGame.bricks[i][j].rect);
            }
        }
    }
    SDL_RenderPresent(gRenderer);
}

void MainGameLoop(Button *button)
{   

    PlayMusic(&musicState, NORMALGAME);
    intenseFlag= 0;
    const Uint32 targetFrameDuration = 1000 / 2000; 
    SDL_Event e;
    int quit;
    Uint32 lastRenderTime;


    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    print_centre_text("Press [SPACE] to StartGame", QUOTE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);
    print_centre_text("Use <- and -> to move around", QUOTE_FONT_SIZE, 0.36 * SCREEN_HEIGHT);

    SDL_SetWindowTitle(gWindow, "Instructions Panel");

    initializeGame();

    SDL_RenderPresent(gRenderer);

    lastRenderTime = 0;
    quit = 0;

    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)  || e.type == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                Save_Game_State();
                quit = 1;
                gameinProgress = 1;
                activeGame.status = 1;
                currentScreen = QUIT_MENU;
                prevScreen = GAME;
                handleScreenState();
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("x is %d and y is %d\n", x, y);
            }
            else if (!gameinProgress)
            {
                switch (e.type)
                {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        gameinProgress = 1;
                        SDL_SetWindowTitle(gWindow, "Game in Progress");
                        break;
                    }
                }
            }
        }

        if (gameinProgress)
        {

            Uint32 currentTime = SDL_GetTicks();
            Uint32 elapsedTime = currentTime - lastRenderTime;

            if (elapsedTime >= targetFrameDuration)
            {
                renderGame();
                lastRenderTime = currentTime;
            }
            updateGame();
        }
    }
}

void renderKeyboard(void)
{
    int numButtons, text_width , text_height;
    Button *buttons_main[10],*saveName;
    SDL_Rect rect;

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
    SDL_SetWindowTitle(gWindow, "Enter Name");

    numButtons = 0 ;

    saveName = createButton(375, 240, 120, 50, "Save Name", &DummyFunction);
    renderButton(saveName);
    addButton(buttons_main, saveName, &numButtons);

    print_centre_text("[Enter Name from Keyboard]", SMALL_FONT_SIZE, 0.80 * SCREEN_HEIGHT);
    
    if(strlen(name)>0){
        print_centre_text(name, MEDIUM_FONT_SIZE, 0.20 * SCREEN_HEIGHT);
        getFontData(name, MEDIUM_FONT_SIZE, &text_width, &text_height);
        text_width+=10;

        rect.x = (SCREEN_WIDTH - text_width)/2;
        rect.y = 0.24*SCREEN_HEIGHT;
        rect.w = text_width;
        rect.h = 10;

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &rect);
    }

    SDL_RenderPresent(gRenderer);
}

void updateKeyboard(void)
{
    int key,WORD_LIMIT;

    const Uint8 *currentKeyStates;
    
    currentKeyStates = SDL_GetKeyboardState(NULL);
    WORD_LIMIT = 10;
    
    for (key = SDL_SCANCODE_A; key <= SDL_SCANCODE_Z; ++key)
    {
        if (currentKeyStates[key])
        {
            char pressedKey = (char)(key - SDL_SCANCODE_A + 'A');

            if (currentKeyStates[SDL_SCANCODE_LSHIFT] || currentKeyStates[SDL_SCANCODE_RSHIFT]) {
                pressedKey += 'a' - 'A';
            }

            if (strlen(name) >= WORD_LIMIT)
            {   
                printf("Name is too long\n");
                return;
            }
            
            strncat(name, &pressedKey, 1);

            printf("%c is pressed\n", pressedKey);
            /*
            SDL_Delay(1); Delay to avoid rapid key press detection*/
        }
    }

/*    if (currentKeyStates[SDL_SCANCODE_SPACE])
    {

        strncat(name, " ", 1);
        printf("Space is pressed\n");
        SDL_Delay(200); 
    } 
*/

    if (currentKeyStates[SDL_SCANCODE_BACKSPACE])
    {


        if (strlen(name) > 0) {
            name[strlen(name) - 1] = '\0';
        }
        printf("Delete is pressed\n");
        SDL_Delay(200); 
    }

    if (currentKeyStates[SDL_SCANCODE_RETURN])
    {
        printf("Enter is pressed\n");
        DummyFunction_Empty();
    }

    printf("Name is %s\n", name);
}

void  MainKeyboardLoop(void)
{

    SDL_Event e;
    int quit;
    int i;
    const Uint32 keyPressDelay = 200; 
    Button *buttons_main[10];
    Button *saveName;
    int numButtons;
    Uint32 lastKeyPressTime;

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "Keyboard Instructions");

    numButtons = 0;

    saveName = createButton(375, 240, 120, 50, "Save Name", &DummyFunction);
    renderButton(saveName);
    addButton(buttons_main, saveName, &numButtons);
    
    print_centre_text("[10 character input limit]", SMALL_FONT_SIZE, 0.75 * SCREEN_HEIGHT);
    print_centre_text("[Enter Name from Keyboard]", SMALL_FONT_SIZE, 0.80 * SCREEN_HEIGHT);

    SDL_RenderPresent(gRenderer);

    lastKeyPressTime = 0;
    
    quit = 0;
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT ||(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)  || e.type == SDL_WINDOWEVENT_FOCUS_LOST)
            {

                currentScreen = QUIT_MENU;
                prevScreen = KEYBOARD;
                handleScreenState();
            }
            else if(e.type==SDL_MOUSEBUTTONDOWN){
                int x,y;
                SDL_GetMouseState(&x,&y);
                for(i=0;i<numButtons;i++){
                    if((x>=buttons_main[i]->rect.x) && (x<=(buttons_main[i]->rect.x+buttons_main[i]->rect.w)) && (y>=buttons_main[i]->rect.y) && (y<=(buttons_main[i]->rect.y+buttons_main[i]->rect.h))){
                        if(name[0]=='\0'){
                            /* name = (char *)malloc(8 * sizeof(char)); */
                            strcpy(name,"Empty");
                        }
                            buttons_main[i]->onClick(buttons_main[i]);
                    }
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {

                keypressed = 1;
            }
        }

        if (keypressed)
        {
            Uint32 currentTime = SDL_GetTicks();
            Uint32 elapsedTime = currentTime - lastKeyPressTime;

            /* Check if enough time has passed since the last key press*/
            if (elapsedTime >= keyPressDelay)
            {
                updateKeyboard();

                lastKeyPressTime = currentTime;

                keypressed = 0 ;
                
                renderKeyboard();
            }
        }
    }
}

void Save_Game_State_toFile(void)
{
    int i,j,k;
    char tempbuffer[4096];
    FILE *file = fopen("resources/game_load_files/savefile.txt", "w");
    if (file)
    {
        for (i = 0; i < NUM_GAME_STATES; i++)
        {
            tempbuffer[0] = '\0';
            snprintf(tempbuffer + strlen(tempbuffer), sizeof(tempbuffer), "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", localSave[i].name, localSave[i].paddleBottom.x, localSave[i].paddleBottom.y, localSave[i].paddleBottom.w, localSave[i].paddleBottom.h, localSave[i].ball.x, localSave[i].ball.y, localSave[i].ball.w, localSave[i].ball.h, localSave[i].ballSpeedX, localSave[i].ballSpeedY, localSave[i].gamePaused, localSave[i].score, localSave[i].temp_collision, localSave[i].status);
            for (j = 0; j < NUM_BRICK_ROWS; j++)
            {
                for (k = 0; k < NUM_BRICK_COLS; k++)
                {
                    snprintf(tempbuffer + strlen(tempbuffer), sizeof(tempbuffer), "%d ", localSave[i].bricks[j][k].health);
                }
            }
            fprintf(file, "%s\n", tempbuffer);
            /*fprintf(file, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", localSave[i].name, localSave[i].paddleBottom.x, localSave[i].paddleBottom.y, localSave[i].paddleBottom.w, localSave[i].paddleBottom.h, localSave[i].ball.x, localSave[i].ball.y, localSave[i].ball.w, localSave[i].ball.h, localSave[i].ballSpeedX, localSave[i].ballSpeedY, localSave[i].gamePaused, localSave[i].score, localSave[i].temp_collision, localSave[i].status);
            */
        }
    }
    else
    {
        printf("Error opening file: %s\n", "resources/game_load_files/savefile.txt");
    }
}

void HandleSavegame(Button *button)
{
    int i,empty;
    if (activeGame.status == 1)
    {
        printf("Score is %d\n", activeGame.score);
        empty = 0;
        for ( i = 0; i < NUM_GAME_STATES; i++)
        {
            if (localSave[i].status == 0 || localSave[i].status == -1)
            {
                char *temp;
                temp = QuickGameKeyboardLoop();
                printf("temp is %s\n",temp);
           
                printf("Slot is empty %d\n", i + 1);
                printf("Writing to slot %d\n", i + 1);
                activeGame.name = strdup(temp);
                localSave[i] = activeGame;
                
                name[0] = '\0';

                /*find a way somehow to save the name*/
                /*write code here that calls the keyboardloop shit and gets a name string as return without going through the handleScreenState ordeal*/ 
                empty = 1;
                break;
            }
        }
        if (!empty)
        {
            printf("No slots are empty\n");
            
            prevScreen = currentScreen;
            currentScreen = SAVE_GAME;
            handleScreenState();

        }
        else{
            prevScreen = MAIN_MENU;
            currentScreen=MAIN_MENU;
            handleScreenState();
        }
    }
}

void quitMenu(void)
{
    int i, numButtons;

    SDL_Event e;
    int quit;
    Button *buttons_quit[10],*StartGame,*LoadGame,*SaveGame;
    void (*returnfuncptr)(Button *button) = NULL;

    printf("prevscreen is %d\n", prevScreen);

    switch (prevScreen)
    {
    case MAIN_MENU:
        returnfuncptr = &returnToMainMenu;
        break;
    case GAME:
        returnfuncptr = &MainGameLoop;
        break;
    case LOAD_GAME:
        returnfuncptr = &loadGameScreen;
        break;
    case HIGH_SCORE:
        returnfuncptr = &renderHighScores;
        break;
    default:
        returnfuncptr = &returnToMainMenu;
        break;
    }


    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
    SDL_SetWindowTitle(gWindow, "Quit Game ?");
    
    numButtons = 0;

    StartGame = createButton(300, 220, 120, 50, "Yes", &quitGame);
    renderButton(StartGame);
    addButton(buttons_quit, StartGame, &numButtons);

    LoadGame = createButton(450, 220, 120, 50, "Go Back", returnfuncptr);
    renderButton(LoadGame);
    addButton(buttons_quit, LoadGame, &numButtons);

    if (prevScreen == GAME && SELECTED_SLOT==0)
    {
        SaveGame = createButton(375, 280, 120, 50, "Save Game", &HandleSavegame);
        renderButton(SaveGame);
        addButton(buttons_quit, SaveGame, &numButtons);
    }

    print_centre_text("Quit Game", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);


    SDL_RenderPresent(gRenderer);

    quit = 0 ;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (i = 0; i < numButtons; i++)
                {
                    if ((x >= buttons_quit[i]->rect.x) && (x <= (buttons_quit[i]->rect.x + buttons_quit[i]->rect.w)) &&
                        (y >= buttons_quit[i]->rect.y) && (y <= (buttons_quit[i]->rect.y + buttons_quit[i]->rect.h)))
                    {
                        if (strcmp(buttons_quit[i]->text, "Yes") == 0)
                        {
                            buttons_quit[i]->onClick(buttons_quit[i]);
                            quit = 1;
                            break;

                        }
                        else
                        {
                            buttons_quit[i]->onClick(buttons_quit[i]);
                        }
                    }
                }
            }
        }
    }

    my_close();
}

void gameOver(Button *button)
{   
    SDL_Event e;

    PlayMusic(&musicState, BADUMTSS);

    int quit,i,numButtons;
    char scoreText[20];
    entry temp;
    Button *buttons_main[10],*StartGame,*HighScore;
    /* seperated from mainLoop to manage different screens
    Clear the render
    clear slot*/

    SDL_RenderClear(gRenderer);

    SDL_SetWindowTitle(gWindow, "Game Over :(");

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    numButtons = 0;

    StartGame = createButton(295, 280, 120, 50, "Main Menu", &returnToMainMenu);
    renderButton(StartGame);
    addButton(buttons_main, StartGame, &numButtons);

    HighScore = createButton(435, 280, 120, 50, "High Score", &Redirect_to_HighScore);
    renderButton(HighScore);
    addButton(buttons_main, HighScore, &numButtons);

    print_centre_text("Game Over", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);

    if (SELECTED_SLOT)
    {
        print_centre_text("Game Saved", QUOTE_FONT_SIZE, 0.25 * SCREEN_HEIGHT);

        sprintf(scoreText, "Score: %d", activeGame.score);
        print_centre_text(scoreText, QUOTE_FONT_SIZE, 0.45 * SCREEN_HEIGHT);
        temp = createEntry(activeGame.name, activeGame.score);
        printf("Player Score is %d\n", temp.score);
        updateHighScores(&temp);
    }
    else
    {
        print_centre_text("Game Not Saved", QUOTE_FONT_SIZE, 0.25 * SCREEN_HEIGHT);
        
        sprintf(scoreText, "Score: %d", activeGame.score);

        print_centre_text(scoreText, QUOTE_FONT_SIZE, 0.45 * SCREEN_HEIGHT);
    }

    SELECTED_SLOT = 0; /* this is a massive time bomb waiting to happen*/
    
    SDL_RenderPresent(gRenderer);
    
    quit =0;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
                quit =1;
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
}

void renderHighScores(Button *button)
{
    SDL_Event e;
    int i,numButtons,quit,x,y;
    Button *buttons_main[10],*backButton;
    void (*returnfuncptr)(Button *button) = NULL;
    
    switch (prevScreen)
    {
    case MAIN_MENU:
        returnfuncptr = &returnToMainMenu;
        break;
    case GAME:
        returnfuncptr = &MainGameLoop;
        break;
    case LOAD_GAME:
        returnfuncptr = &loadGameScreen;
        break;
    case GAME_OVER:
        returnfuncptr = &gameOver;
        break;
    default:
        returnfuncptr = &returnToMainMenu;
        break;
    }
    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "High Scores");

    print_centre_text("High Scores", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);

    for (i = 0; i < NUM_SCORES; i++)
    {
        char buffer[100];
        sprintf(buffer, "%s : %d", localentries[i].name, localentries[i].score);
        print_centre_text(buffer, QUOTE_FONT_SIZE, 0.20 * SCREEN_HEIGHT + (i + 1) * 40);
    }

    numButtons=0;

    backButton = createButton(20, 20, 80, 40, "Back", returnfuncptr);
    renderButton(backButton);
    addButton(buttons_main, backButton, &numButtons);

    SDL_RenderPresent(gRenderer);
    
    quit =0;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
                quit = 1;
                currentScreen = QUIT_MENU; 
                prevScreen = MAIN_MENU;
                handleScreenState();
                printf("Load Quit Game Screen\n");
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                
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
}


void QuickrenderKeyboard(void)
{
    Button *buttons_main[10],*saveName;

    int numButtons;
    SDL_Rect rect;
    int text_width, text_height;
    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
    SDL_SetWindowTitle(gWindow, "Enter Name");

    numButtons = 10;

    saveName = createButton(375, 240, 120, 50, "Save Name", &DummyFunction);
    renderButton(saveName);
    addButton(buttons_main, saveName, &numButtons);

    print_centre_text("[Enter Name from Keyboard]", SMALL_FONT_SIZE, 0.80 * SCREEN_HEIGHT);
    
    if(strlen(name)>0){
        print_centre_text(name, MEDIUM_FONT_SIZE, 0.20 * SCREEN_HEIGHT);
        getFontData(name, MEDIUM_FONT_SIZE, &text_width, &text_height);
        text_width+=10;
        rect.x =  (SCREEN_WIDTH - text_width)/2;
        rect.y = 0.24*SCREEN_HEIGHT;
        rect.w = text_width;
        rect.h = 10;
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &rect);
    }
    SDL_RenderPresent(gRenderer);
}

void QuickupdateKeyboard(void)
{
    int key,WORD_LIMIT;
    const Uint8 *currentKeyStates;
    
    WORD_LIMIT = 10;

    currentKeyStates = SDL_GetKeyboardState(NULL);

    for (key = SDL_SCANCODE_A; key <= SDL_SCANCODE_Z; ++key)
    {
        if (currentKeyStates[key])
        {
            char pressedKey = (char)(key - SDL_SCANCODE_A + 'A');

            if (currentKeyStates[SDL_SCANCODE_LSHIFT] || currentKeyStates[SDL_SCANCODE_RSHIFT]) {
                pressedKey += 'a' - 'A';
            }

            if (strlen(name) >= WORD_LIMIT)
            {   
                printf("Name is too long\n");
                return;
            }
            
            strncat(name, &pressedKey, 1);

            printf("%c is pressed\n", pressedKey);
            /* SDL_Delay(1);  Delay to avoid rapid key press detection*/
        }
    }

/*    if (currentKeyStates[SDL_SCANCODE_SPACE])
    {

        strncat(name, " ", 1);
        printf("Space is pressed\n");
        SDL_Delay(200); 
    } 
*/

    if (currentKeyStates[SDL_SCANCODE_RETURN])
    {
        printf("Enter is pressed\n");
        DummyFunction_Empty();
    }

    printf("Name is %s\n", name);
}

char*  QuickGameKeyboardLoop(void)
{
    SDL_Event e;
    int quit,i;
    const Uint32 keyPressDelay = 200; 
    int numButtons ;
    Button *buttons_main[10],*saveName;
    Uint32 lastKeyPressTime;
    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "Keyboard Instructions");

    numButtons = 0 ;

    saveName = createButton(375, 240, 120, 50, "Save Name", &DummyFunction);
    renderButton(saveName);
    addButton(buttons_main, saveName, &numButtons);
    
    print_centre_text("[10 character input limit]", SMALL_FONT_SIZE, 0.75 * SCREEN_HEIGHT);
    print_centre_text("[Enter Name from Keyboard]", SMALL_FONT_SIZE, 0.80 * SCREEN_HEIGHT);

    SDL_RenderPresent(gRenderer);
    
    lastKeyPressTime= 0;
    quit = 0;

    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_WINDOWEVENT_FOCUS_LOST)
            {

                currentScreen = QUIT_MENU;
                prevScreen = KEYBOARD;
                handleScreenState();
            }
            else if(e.type==SDL_MOUSEBUTTONDOWN){
                int x,y;
                SDL_GetMouseState(&x,&y);
                for(i=0;i<numButtons;i++){
                    if((x>=buttons_main[i]->rect.x) && (x<=(buttons_main[i]->rect.x+buttons_main[i]->rect.w)) && (y>=buttons_main[i]->rect.y) && (y<=(buttons_main[i]->rect.y+buttons_main[i]->rect.h))){
                        /* buttons_main[i]->onClick(buttons_main[i]); */ 
                        printf("The name is %s\n",name);
                        return name;
                    }
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {

                keypressed = 1;
            }
        }

        if (keypressed)
        {
            Uint32 currentTime = SDL_GetTicks();
            Uint32 elapsedTime = currentTime - lastKeyPressTime;

            if (elapsedTime >= keyPressDelay)
            {
                QuickupdateKeyboard();

                lastKeyPressTime = currentTime;

                keypressed = 0;
                
                QuickrenderKeyboard();
            }
        }
    }
    return name;
}

void renderSettings(void)
{
    int i,x,y;
    SDL_Event e;
    int quit;
    char temp[100];
    int textWidth, textHeight,textY,textX;
    Button *buttons_main[10];
    Button *backButton ,  *Automatic , *ResetGameSlots;
    int numButtons;


    void (*returnfuncptr)(Button *button) = NULL;

    switch (prevScreen)
    {
    case MAIN_MENU:
        returnfuncptr = &returnToMainMenu;
        break;
    case GAME:
        returnfuncptr = &MainGameLoop;
        break;
    case LOAD_GAME:
        returnfuncptr = &loadGameScreen;
        break;
    case GAME_OVER:
        returnfuncptr = &gameOver;
        break;
    default:
        returnfuncptr = &returnToMainMenu;
        break;
    }

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "Settings");


    print_centre_text("Settings", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);

    numButtons = 0;

    backButton = createButton(20, 20, 80, 40, "Back", returnfuncptr);
    renderButton(backButton);
    addButton(buttons_main, backButton, &numButtons);

    getFontData("Automatic Mode :", DEFAULT_TEXT_SIZE, &textWidth, &textHeight);

    /* Existing code for centering short lines */
    textX = ((SCREEN_WIDTH - textWidth) / 2)*.70;
    textY = 0.35*SCREEN_HEIGHT - textHeight / 2;
    renderText("Automatic Mode :", textX, textY, DEFAULT_TEXT_SIZE);

    if (settings.autoPlay)
    {
        strcpy(temp, "Disable");
    }
    else
    {
        strcpy(temp, "Enable");
    }

    Automatic = createButton(425, 175, 120, 50, temp, &toggleAutomaticMode);
    renderButton(Automatic);
    addButton(buttons_main, Automatic, &numButtons);

    ResetGameSlots = createButton(360, 250, 150, 50, "Reset Save File", &ResetSaveFile);
    renderButton(ResetGameSlots);
    addButton(buttons_main, ResetGameSlots, &numButtons);
    
    SDL_RenderPresent(gRenderer);
    quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
                quit = 1;
                currentScreen = QUIT_MENU; 
                prevScreen = MAIN_MENU;
                handleScreenState();
                printf("Load Quit Game Screen\n");
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
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
}

void SaveGameScreen(void)
{   
    SDL_Event e;
    int i,centreX;
    int quit;
    int numButtons;

    Button *buttons_main[10];
    Button *backButton , *SaveFile1,*SaveFile2,*SaveFile3;

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

    SDL_SetWindowTitle(gWindow, "Save Game");

    numButtons= 0;

    backButton = createButton(20, 20, 80, 40, "Back", &returnToMainMenu);
    renderButton(backButton);
    addButton(buttons_main, backButton, &numButtons);
    centreX = SCREEN_WIDTH / 2;

    SaveFile1 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23, 600, 100, "Slot 1", &OverwriteSlot);
    renderSaveButton(SaveFile1, localSave[0]);
    addButton(buttons_main, SaveFile1, &numButtons);

    SaveFile2 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23 + 110, 600, 100, "Slot 2", &OverwriteSlot);
    renderSaveButton(SaveFile2, localSave[1]);
    addButton(buttons_main, SaveFile2, &numButtons);

    SaveFile3 = createButton(centreX - 600 / 2, SCREEN_HEIGHT * 0.23 + 220, 600, 100, "Slot 3", &OverwriteSlot);
    renderSaveButton(SaveFile3, localSave[2]);
    addButton(buttons_main, SaveFile3, &numButtons);

    print_centre_text("Save Files", TITLE_FONT_SIZE, 0.16 * SCREEN_HEIGHT);

    SDL_RenderPresent(gRenderer);

    quit = 0;
    
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT ||(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) )
            {
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

                for  (i = 0; i < numButtons; i++)
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
}

void handleScreenState(void)
{
    
    switch (currentScreen)
    {
    case MAIN_MENU:
        PlayMusic(&musicState,0);
        renderMain();
        break;
    case GAME:
        PlayMusic(&musicState,2);
        MainGameLoop(&dummy_button);
        break;
    case LOAD_GAME:
        loadGameScreen(&dummy_button);
        break;
    case QUIT_MENU:
        /* PlayMusic(&musicState,3); */
        PauseMusic(&musicState);
        quitMenu();
        break;
    case GAME_OVER:
        gameOver(&dummy_button);
        break;
    case HIGH_SCORE:
        renderHighScores(&dummy_button);
        break;
    case SETTINGS:
        PlayMusic(&musicState,0);
        renderSettings();
        break;
    case SAVE_GAME:
        SaveGameScreen();
        break;
    case KEYBOARD:
        MainKeyboardLoop();
        break;  
    default:
        break;
    }
}
