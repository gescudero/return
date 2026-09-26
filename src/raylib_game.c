/*******************************************************************************************
*
*   raylib game template
*
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "colors.h"
#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: rand() y srand()
#include <time.h>                           // Required for: time() in random seed generator
#include <string.h>                         // Required for:

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = LOGO;
Font font = { 0 };
Font big_font = { 0 };
Font small_font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };
Sound fxKeys = { 0 };
Sound fxHorns[3] = {0};

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static int seconds_to_end = 300;
static float elapsed_time = 0.0f; 

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame
static void UpdateGlobalUI(void);
static void DrawGlobalUI(void);
//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice();      // Initialize audio device
    srand(time(NULL));      // Initialize random seed 

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFontEx("./resources/SauceCodeProNerdFont-SemiBold.ttf", 30, NULL, 0);
    big_font = LoadFontEx("./resources/SauceCodeProNerdFont-SemiBold.ttf", 100, NULL, 0);
    small_font = LoadFontEx("./resources/SauceCodeProNerdFont-SemiBold.ttf", 20, NULL, 0);
    //music = LoadMusicStream("resources/ambient.ogg"); // TODO: Load music
    fxKeys = LoadSound("./resources/keys.mp3");
    fxHorns[0] = LoadSound("./resources/car_horn_01.mp3");
    fxHorns[1] = LoadSound("./resources/car_horn_02.mp3");
    fxHorns[2] = LoadSound("./resources/car_horn_03.mp3");
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case INTRO: UnloadIntroScreen(); break;
        case CROSSGAME: UnloadCrossingScreen(); break;
        case FIGHTGAME: UnloadFightingScreen(); break;
        case TITRIS: UnloadTitrisScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);
    UnloadSound(fxKeys);
    for (int i=0; i<3; i++) {
        UnloadSound(fxHorns[i]);
    }

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(int screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case INTRO: UnloadIntroScreen(); break;
        case CROSSGAME: UnloadCrossingScreen(); break;
        case FIGHTGAME: UnloadFightingScreen(); break;
        case TITRIS: UnloadTitrisScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case INTRO: InitIntroScreen(); break;
        case CROSSGAME: InitCrossingScreen(); break;
        case FIGHTGAME: InitFightingScreen(); break;
        case TITRIS: InitTitrisScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    LOG("UpdateTransition\n");
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case INTRO: UnloadIntroScreen(); break;
                case CROSSGAME: UnloadCrossingScreen(); break;
                case FIGHTGAME: UnloadFightingScreen(); break;
                case TITRIS: UnloadTitrisScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case OPTIONS: InitOptionsScreen(); break;
                case INTRO: InitIntroScreen(); break;
                case CROSSGAME: InitCrossingScreen(); break;
                case FIGHTGAME: InitFightingScreen(); break;
                case TITRIS: InitTitrisScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        DrawText("FADEOUT", 200, 150, 20, XT_DK_PURPLE);
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    //UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen()) TransitionToScreen(TITLE);

            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
                else if (FinishTitleScreen() == 2) TransitionToScreen(INTRO);

            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen()) TransitionToScreen(TITLE);

            } break;
            case INTRO:
            {
                UpdateIntroScreen();

                if (FinishIntroScreen() == 1) TransitionToScreen(CROSSGAME);
            } break;
            case CROSSGAME:
            {
                UpdateGlobalUI();
                UpdateCrossingScreen();

                if (FinishCrossingScreen() == 1) TransitionToScreen(FIGHTGAME);
                //else if (FinishCrossingScreen() == 2) TransitionToScreen(TITLE);

            } break;
            case FIGHTGAME:
            {
                UpdateGlobalUI();
                UpdateFightingScreen();

                if (FinishFightingScreen() == 1) TransitionToScreen(TITRIS);
                //else if (FinishCrossingScreen() == 2) TransitionToScreen(TITLE);

            } break;

            case TITRIS:
            {
                UpdateGlobalUI();
                UpdateTitrisScreen();

                if (FinishTitrisScreen() == 1) TransitionToScreen(ENDING);
            }
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

            } break;
            default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(XT_DK_GREY);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case INTRO: DrawIntroScreen(); break;
            case CROSSGAME:
            {
                DrawCrossingScreen(); 
                DrawGlobalUI();
                break;
            }
            case FIGHTGAME:
            {
                DrawFightingScreen(); 
                DrawGlobalUI();
                break;
            }

            case TITRIS: 
            {
                DrawTitrisScreen(); 
                DrawGlobalUI();
                break;
            }
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) 
        {
            DrawTransition();
        }
    EndDrawing();
    //----------------------------------------------------------------------------------
}


static void UpdateGlobalUI(void) {
    elapsed_time += GetFrameTime();

    if (seconds_to_end - (int)elapsed_time <= 0) currentScreen = ENDING;
}
static void DrawGlobalUI(void) {
    Vector2 pos = {50, 10};
    DrawTextEx(small_font, TextFormat("Time to close office: %i", seconds_to_end - (int)elapsed_time), pos, small_font.baseSize, 2, VN_GN_YELLOW);

}
