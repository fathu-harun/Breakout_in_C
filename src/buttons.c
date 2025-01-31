#include "buttons.h"
#include "constants.h"
#include <stdio.h>
#include "screens.h"
#include "text.h"

int SELECTED_SLOT=0;

void renderButton(const Button *button) {
    int textX,textY,textWidth, textHeight;

    /*Assuming getFontData returns the dimensions of the text*/
    getFontData(button->text, BUTTON_FONT_SIZE, &textWidth, &textHeight);

    /* Calculate the position to render the text within the button*/
    textX = button->rect.x + (button->rect.w - textWidth) / 2;
    textY = button->rect.y + (button->rect.h - textHeight) / 2;

    SDL_RenderCopy(gRenderer, buttonTexture, NULL, &button->rect);

    renderText(button->text, textX, textY, BUTTON_FONT_SIZE);
}

void renderSaveButton(const Button *button , Game_State saveFile) {
    /* if status = -1 , game lost 
     if status =0 , empty game
     if status = 1 , game saved*/
    /*Get text dimensions using the specified font size*/
    int textWidth, textHeight,textX,textY;
    char temp[100];
    /*Assuming getFontData returns the dimensions of the text*/
    getFontData(button->text, MEDIUM_FONT_SIZE, &textWidth, &textHeight);

    /* Calculate the position to render the text within the button*/
    textX = button->rect.x + ((button->rect.w - textWidth) / 2)*.1;
    textY = button->rect.y + (button->rect.h - textHeight) / 2;

    SDL_RenderCopy(gRenderer, saveButtonTexture, NULL, &button->rect);

    renderText(button->text, textX, textY, MEDIUM_FONT_SIZE);

    if(saveFile.status==0){
        getFontData("Empty", DEFAULT_TEXT_SIZE, &textWidth, &textHeight);
        textX = button->rect.x + ((button->rect.w - textWidth) / 2);
        textY = button->rect.y + (button->rect.h - textHeight) / 2;
        renderText("Empty", textX, textY, DEFAULT_TEXT_SIZE);
    }
    else if(saveFile.status==1){
        getFontData("Saved", DEFAULT_TEXT_SIZE, &textWidth, &textHeight);
        textX = (button->rect.x + ((button->rect.w - textWidth) / 2))*.8;
        textY = button->rect.y + (button->rect.h - textHeight) / 2;
        renderText("Saved", textX, textY, DEFAULT_TEXT_SIZE);

        sprintf(temp,"Name : %s",saveFile.name);
        getFontData(temp,SMALL_FONT_SIZE, &textWidth, &textHeight);
        textX = (button->rect.x + (button->rect.w - textWidth) / 2)*1.2;
        textY = (button->rect.y + (button->rect.h - textHeight) / 2) - textHeight/2;
        renderText(temp, textX, textY,SMALL_FONT_SIZE);


        sprintf(temp,"Score : %d",saveFile.score);
        getFontData(temp,SMALL_FONT_SIZE, &textWidth, &textHeight);
        textX = (button->rect.x + (button->rect.w - textWidth) / 2)*1.2;
        textY = (button->rect.y + (button->rect.h - textHeight) / 2)+ textHeight/2;
        renderText(temp, textX, textY,SMALL_FONT_SIZE);
    }
    else{
        getFontData("Lost", DEFAULT_TEXT_SIZE, &textWidth, &textHeight);
        textX = button->rect.x + ((button->rect.w - textWidth) / 2);
        textY = button->rect.y + (button->rect.h - textHeight) / 2;
        renderText("Lost", textX, textY, DEFAULT_TEXT_SIZE);   
    }
}

void print_slot(Button *button) {
    int slot_num;

    sscanf(button->text, "Slot %d", &slot_num);
    if(slot_num>0 && slot_num<4){
        printf("Slot %d has been clicked\n",slot_num);
        print_slot_file(slot_num);
        SELECTED_SLOT = slot_num;
        printf("Slot is %d\n",SELECTED_SLOT);

        if(localSave[slot_num-1].status==0){
            printf("Empty Slot\n");
            /* maybe set slot status to 1 here*/
            currentScreen = KEYBOARD;
            handleScreenState();
        }
        else{
        currentScreen = GAME;
        handleScreenState();
        }
    }
    else{
        printf("Invalid Slot\n");
    }
}

void quitGame(Button *button) {
    
    printf("\nYou have Decided to quit the game\n");
}

void return_back(Button *button) {
    printf("Implement Go Back \n");
}

void Redirect_to_Game(Button *button) {
    currentScreen = LOAD_GAME; 
    prevScreen=MAIN_MENU;
    handleScreenState(); 
}

void returnToMainMenu(Button *button) {
    currentScreen = MAIN_MENU;
    handleScreenState();
}

void Redirect_to_HighScore(Button *button)
{
    prevScreen=currentScreen;
    currentScreen = HIGH_SCORE;
    handleScreenState(); 
}

void Redirect_to_Settings(Button *button){
    prevScreen = currentScreen;
    currentScreen = SETTINGS;
    handleScreenState();
}

void Redirect_to_SaveGameScreen(Button *button) {
    prevScreen = currentScreen;   
    currentScreen = SAVE_GAME;
    handleScreenState();
}

void startGame(Button *button) {
    set_default_state(&activeGame);
    currentScreen = GAME;
    prevScreen=MAIN_MENU; 
    handleScreenState();
} 

void buttonClicked(Button *button) {
    prevScreen = currentScreen;
    currentScreen = KEYBOARD;
    handleScreenState();
}

void DummyFunction_Empty(void){
    if(SELECTED_SLOT>0 && SELECTED_SLOT<4){
        strcpy(localSave[SELECTED_SLOT-1].name,name);
        localSave[SELECTED_SLOT-1].status = 1;
        currentScreen = GAME;
        handleScreenState();
    }
    else{
        printf("Invalid Slot\n");
    }
}

void DummyFunction(Button *button){
    if(SELECTED_SLOT>0 && SELECTED_SLOT<4){
        strcpy(localSave[SELECTED_SLOT-1].name,name);
        localSave[SELECTED_SLOT-1].status = 1;

        name[0] = '\0';
        currentScreen = GAME;
        handleScreenState();
    }
    else{
        printf("Invalid Slot\n");
    }
}


Button* createButton(int x, int y, int width, int height, const char* buttonText , void (*clickHandler)(Button*)) {
    Button* button = (Button*)malloc(sizeof(Button));
    if (button == NULL) {
        return NULL;
    }
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = width;
    button->rect.h = height;
    /*  Allocate memory for the text and copy the buttonText */
    button->text = (char*)malloc(strlen(buttonText) + 1);
    if (button->text == NULL) {
       
        free(button);
        return NULL;
    }
    strcpy(button->text, buttonText);
    /*  Assign the click handler functionz */
    button->onClick = clickHandler;

    return button;
}

void addButton(Button * buttons[], Button *button, int * numButtons) {
    if (*numButtons < 10) {
        buttons[*numButtons] = button;
        *numButtons += 1;
    }
}

void toggleAutomaticMode(Button *button){
    settings.autoPlay = !settings.autoPlay;
    printf("Auto Play Mode : %d\n",settings.autoPlay);
    currentScreen = prevScreen;
    handleScreenState();
}

void ResetSaveFile(Button *button){
    int i;
    for (i = 0; i < NUM_GAME_STATES; i++) {
        localSave[i].status = -1;
    }
    printf("Save File Reset\n");
    activeGame.status =-1;
    Save_Game_State_toFile();
    loadSaveFile();
    currentScreen = prevScreen;
    handleScreenState();
}
