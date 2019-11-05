#include <unistd.h>
#include <switch.h>

#include "menu.h"
#include "touch.h"
#include "util.h"

#define APP_VERSION "Kefir Updater: 0.5.0"

void refreshScreen()
{
    clearRenderer();
    
    // app version.
    drawText(fntMedium, 40, 40, SDL_GetColour(black), APP_VERSION);

    // system version.
    drawText(fntSmall, 25, 150, SDL_GetColour(black), getSysVersion());

    // atmosphere version.
    //drawText(fntSmall, 25, 230, SDL_GetColour(black), getAmsVersion());

    //drawText(fntMedium, 120, 225, SDL_GetColour(black), "Menu Here"); // menu options
    drawButton(fntButton, BUTTON_A, 970, 672, SDL_GetColour(black));
    drawText(fntSmall, 1010, 675, SDL_GetColour(black), "Select");
    drawButton(fntButton, BUTTON_PLUS, 1145, 672, SDL_GetColour(black));
    drawText(fntSmall, 1185, 675, SDL_GetColour(black), "Exit");
}

void printOptionList(int cursor)
{
    refreshScreen();

    char *option_list[]      = {    "Update Atmosphere", \
                                    "Update SXOS", \
                                    "Reboot (reboot to payload)" };

    char *description_list[] = {    "Update kefir on Atmosphere", \
                                    "Update kefir on SXOS", \
                                    "Reboots switch (recommended after updating)" };

    SDL_Texture *textureArray[] = { ams_icon, app_icon, reboot_icon };

    for (int i=0, nl=0; i < (CURSOR_LIST_MAX+1); i++, nl+=NEWLINE)
    {
        if (cursor != i) drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(black), option_list[i]);
        else
        {
            // icon for the option selected.
            drawImage(textureArray[i], 125, 350);
            // highlight box.
            drawShape(SDL_GetColour(dark_grey), 530, (FIRST_LINE + nl - HIGHLIGHT_BOX_MIN), 700, HIGHLIGHT_BOX_MAX);
            // option text.
            drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(grey), option_list[i]);
            // description.
            drawText(fntSmall, 25, 675, SDL_GetColour(black), description_list[i]);
        }
    }
}

void popUpBox(TTF_Font *font, int x, int y, SDL_Colour colour, char *text)
{
    // outline. box
    drawShape(SDL_GetColour(black), (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    drawShape(SDL_GetColour(not_so_dark_grey), SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    drawText(font, x, y, colour, text);
}

int yesNoBox(int mode, int x, int y, char *question)
{
    printOptionList(mode);
    popUpBox(fntMedium, x, y, SDL_GetColour(black), question);
    // highlight box.
    drawShape(SDL_GetColour(dark_grey), 380, 410, 175, 65);
    drawShape(SDL_GetColour(dark_grey), 705, 410, 195, 65);
    // option text.
    drawButton(fntButtonBig, BUTTON_B, 415, 425, SDL_GetColour(grey));
    drawText(fntMedium, 460, 425, SDL_GetColour(grey), "No");
    drawButton(fntButtonBig, BUTTON_A, 725, 425, SDL_GetColour(grey));
    drawText(fntMedium, 770, 425, SDL_GetColour(grey), "Yes");

    updateRenderer();

    int res = 0;
    int touch_lock = OFF;
    touchPosition touch;
    u32 tch = 0;
    u32 touch_count = hidTouchCount();

    // check if the user is already touching the screen.
    if (touch_count > 0) touch_lock = ON;

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        touch_count = hidTouchCount();

        if (touch_count == 0) touch_lock = OFF;

        if (touch_count > 0 && touch_lock == OFF)
            res = touch_yes_no_option(touch.px, touch.py);

        if (kDown & KEY_A || res == YES)
            return YES;

        if (kDown & KEY_B || res == NO)
            return NO;
    }
}

int yesNoBox1(int x, int y, char *question)
{
    popUpBox(fntMedium, x, y, SDL_GetColour(black), question);
    // highlight box.
    drawShape(SDL_GetColour(dark_grey), 380, 410, 175, 65);
    drawShape(SDL_GetColour(dark_grey), 705, 410, 195, 65);
    // option text.
    drawButton(fntButtonBig, BUTTON_B, 415, 425, SDL_GetColour(grey));
    drawText(fntMedium, 460, 425, SDL_GetColour(grey), "No");
    drawButton(fntButtonBig, BUTTON_A, 725, 425, SDL_GetColour(grey));
    drawText(fntMedium, 770, 425, SDL_GetColour(grey), "Yes");

    updateRenderer();

    int res = 0;
    int touch_lock = OFF;
    touchPosition touch;
    u32 tch = 0;
    u32 touch_count = hidTouchCount();

    // check if the user is already touching the screen.
    if (touch_count > 0) touch_lock = ON;

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        touch_count = hidTouchCount();

        if (touch_count == 0) touch_lock = OFF;

        if (touch_count > 0 && touch_lock == OFF)
            res = touch_yes_no_option(touch.px, touch.py);

        if (kDown & KEY_A || res == YES)
            return YES;

        if (kDown & KEY_B || res == NO)
            return NO;
    }
}

void errorBox(int x, int y, char *errorText)
{
    popUpBox(fntMedium, x, y, SDL_GetColour(black), errorText);
    drawImageScale(error_icon, 570, 340, 128, 128);
    updateRenderer();

    sleep(3);
}