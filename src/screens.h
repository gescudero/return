/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef SCREENS_H
#define SCREENS_H

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, INTRO, CROSSGAME, FIGHTGAME, TITRIS, GAMEPLAY, ENDING } GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Font big_font;
extern Font small_font;
extern Music music;
extern Sound fxCoin;
extern Sound fxKeys;
extern Sound fxHorns[3];

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Crossing Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitCrossingScreen(void);
void UpdateCrossingScreen(void);
void DrawCrossingScreen(void);
void UnloadCrossingScreen(void);
int FinishCrossingScreen(void);

//----------------------------------------------------------------------------------
// Fight Screen Functions Declaration
// ---------------------------------------------------------------------------------
void InitFightingScreen(void);
void UpdateFightingScreen(void);
void DrawFightingScreen(void);
void UnloadFightingScreen(void);
int FinishFightingScreen(void);

//----------------------------------------------------------------------------------
// Titris Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitrisScreen(void);
void UpdateTitrisScreen(void);
void DrawTitrisScreen(void);
void UnloadTitrisScreen(void);
int FinishTitrisScreen(void);

//----------------------------------------------------------------------------------
// Intro Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitIntroScreen(void);
void UpdateIntroScreen(void);
void DrawIntroScreen(void);
void UnloadIntroScreen(void);
int FinishIntroScreen(void);

//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
