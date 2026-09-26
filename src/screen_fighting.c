#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

typedef enum { FS_IDLE, FS_HIT, FS_AVOID } fighter_state;

typedef struct {
    fighter_state state;
    float cooldown;
    int health;
} fighter_t;

//----------------------------------------------------------------------------------
// Fighting Screen Functions Definition
//----------------------------------------------------------------------------------

// Fighting Screen Initialization logic
void InitFightingScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
}

// Fighting Screen Update logic
void UpdateFightingScreen(void)
{
    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxKeys);
    }
}

// Fighting Screen Draw logic
void DrawFightingScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
}

// Fighting Screen Unload logic
void UnloadFightingScreen(void)
{
}

// Fighting Screen should finish?
int FinishFightingScreen(void)
{
    return finishScreen;
}
