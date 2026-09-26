/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Crossing Screen Functions Definitions (Init, Update, Draw, Unload)
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

#include "colors.h"
#include "raylib.h"
#include "screens.h"
#include <stdlib.h>
#include <time.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

#define NB_OBSTACLES 30
#define NB_LANES 6
#define NB_OBS_TYPE 7

static int framesCounter = 0;
static int finishScreen = 0;

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    Texture2D texture;
    int lives;
    float speed;
    int row;
    Rectangle rectangle;
    int last_rebirth;
    float alpha;
} Player;

typedef struct Obstacle {
    int id;
    int kind; // 0:greencar; 1:redcar; 2:yellowbus
    int row;
    Vector2 position;
    Texture2D texture;
    float speed;
    bool active;
    Rectangle rectangle;
    Color color;
} Obstacle;

Player player;
Obstacle obstacles[NB_OBSTACLES];
Vector2 player_start_pos = {0};
float time_since_horn = 0.0f;

//----------------------------------------------------------------------------------
// Crossing Screen Functions Definition
//----------------------------------------------------------------------------------
static void UpdatePlayer(Player *player);
static void DrawPlayer(Player *player);
static void UpdateObstacle(Obstacle *obstacle);
static void DrawObstacles();
static void FixCollisionObstacles();

// Crossing Screen Initialization logic
void InitCrossingScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    player_start_pos = (Vector2){((float)GetScreenWidth()/2)+16, 400};
    
    player.position = player_start_pos;
    player.velocity = (Vector2){0, 0};
    player.texture = LoadTexture("./resources/titris_person2.png");
    player.lives = 3;
    player.speed = 100.0f;
    player.row = 0;
    player.rectangle = (Rectangle){player.position.x, player.position.y, 32, 64};
    player.last_rebirth = framesCounter;
    
    srand(time(NULL));

    // Edit image for obstacles
    Texture2D texturas[NB_OBS_TYPE];
    texturas[0] = LoadTexture("./resources/coche_01.png");
    texturas[1] = LoadTexture("./resources/coche_02.png");
    texturas[2] = LoadTexture("./resources/bus_01.png");
    texturas[3] = LoadTexture("./resources/coche_02_amarillo.png");
    texturas[4] = LoadTexture("./resources/coche_02_azul.png");
    texturas[5] = LoadTexture("./resources/coche_01_rosa.png");
    texturas[6] = LoadTexture("./resources/coche_01_beige.png");
    for ( int i=0; i<NB_OBSTACLES; i++ )
    {

        obstacles[i].id = i;
        obstacles[i].kind = i%NB_OBS_TYPE;
        obstacles[i].texture = texturas[obstacles[i].kind];
        obstacles[i].row = i % (NB_LANES - 2);
        obstacles[i].position = (Vector2){ GetScreenWidth() + obstacles[i].texture.width, 90 + (obstacles[i].row * 75) };
        obstacles[i].speed = 100.0f + (rand() % 30);
        obstacles[i].active = false;
        switch (obstacles[i].kind) {
            case 0:
                obstacles[i].rectangle = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, 64, 64};
                break;
            case 1:
                obstacles[i].rectangle = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, 64, 64};
                break;
            case 2:
                obstacles[i].rectangle = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, 128, 64};
                break;
            default:
                obstacles[i].rectangle = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, 64, 64};
                break;
        }
        switch (rand() % 6) {
            case 0: obstacles[i].color = VN_LT_PURPLE; break;
            case 1: obstacles[i].color = VN_GN_YELLOW; break;
            case 2: obstacles[i].color = VN_CHALK; break;
            case 3: obstacles[i].color = VN_BK_WHITE; break;
            case 4: obstacles[i].color = VN_BLUE; break;
            case 5: obstacles[i].color = VN_GN_BLUE; break;
            default: obstacles[i].color = VN_RED; break;
        } 
    }
}

// Crossing Screen Update logic
void UpdateCrossingScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    framesCounter++;

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxKeys);
    }
    // Check collision with player
    if (IsKeyPressed(KEY_UP))
    {
        player.row++;
        if (player.row > NB_LANES -1) {
            player.row = NB_LANES - 1;
            finishScreen = 1;
        } 
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        player.row--;
        if (player.row < 0) player.row = 0;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        player.position.x -= 25;
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        player.position.x += 25;
    }
    UpdatePlayer(&player);

    //Obstacles 
    for (int i = 0; i<NB_OBSTACLES; i++) 
    {
        if (obstacles[i].active)
        {
            UpdateObstacle(&obstacles[i]);
        }

        if (!obstacles[i].active && (framesCounter % 25 == 0)) 
        {
            obstacles[i].active = true;
            UpdateObstacle(&obstacles[i]);
            break;
        }

        if (CheckCollisionRecs(player.rectangle, obstacles[i].rectangle)) 
        {
            player.position = player_start_pos;
            player.row = 0;
            player.lives--;
            player.last_rebirth = framesCounter;

            if (player.lives < 0)
            {
                // GAME OVER
            }
        }
    }
    FixCollisionObstacles();
}

// Crossing Screen Draw logic
void DrawCrossingScreen(void)
{
    // Background color
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), VN_GREEN);
    // lanes 
    for ( int i=1; i<5; i++ ) 
    {
        DrawRectangle(0, (int)(GetScreenHeight()/NB_LANES)*i, GetScreenWidth(), GetScreenHeight()/NB_LANES, XT_DK_GREY);
        DrawRectangle(0, (int)(GetScreenHeight()/NB_LANES)*i, GetScreenWidth(), 4, VN_WHITE);
    }
    // player 
    DrawPlayer(&player);
    // obstacles 
    DrawObstacles();

}

// Crossing Screen Unload logic
void UnloadCrossingScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadTexture(player.texture);
    for (int i=0; i<NB_OBSTACLES; i++)
    {
        UnloadTexture(obstacles[i].texture);
    }
}

// Crossing Screen should finish?
int FinishCrossingScreen(void)
{
    return finishScreen;
}

// private Functions
static void UpdatePlayer(Player *player)
{
    player->position.y = player_start_pos.y - (75 * player->row);
    player->rectangle.x = player->position.x;
    player->rectangle.y = player->position.y;
}
static void DrawPlayer(Player *player)
{
    float alpha = framesCounter - player->last_rebirth;
    if (alpha > 100)
    {
        alpha = 100.0f;
    }
    alpha = alpha/100.0f;
    // DrawRectangleRec(player->rectangle, Fade(VN_BK_WHITE, alpha));
    DrawTextureV(player->texture, player->position, RAYWHITE);
}
static void UpdateObstacle(Obstacle *obstacle)
{
    switch (obstacle->row) {
        case 0:
        case 1:
            if (obstacle->position.x > -132 )
            {
                obstacle->position.x -= obstacle->speed * GetFrameTime();
            } 
            else 
            {
                obstacle->position.x = GetScreenWidth();
                obstacle->active = false;
            }

            break;
        case 2:
        case 3:
            if (obstacle->position.x < GetScreenWidth() + obstacle->rectangle.width )
            {
                obstacle->position.x += obstacle->speed * GetFrameTime();
            } 
            else 
            {
                obstacle->position.x = obstacle->rectangle.width * -1;
                obstacle->active = false;
            }
            break;
        default:
            break;
    }
    obstacle->rectangle.x = obstacle->position.x;

}
static void DrawObstacles()
{
    for (int i=0; i<NB_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            if (obstacles[i].row < 2) {
                DrawTextureV(obstacles[i].texture, obstacles[i].position, RAYWHITE);
            } else {
                Vector2 pos = obstacles[i].position;
                pos.x += obstacles[i].texture.width;
                pos.y += (obstacles[i].texture.height - 10);
                DrawTextureEx(obstacles[i].texture, pos, 180, 1.0f, RAYWHITE);
            }
        }
    }
}

static void FixCollisionObstacles()
{
    bool caravana = false;
    for (int i=0; i<NB_OBSTACLES; i++)
    {
        if (!obstacles[i].active)
        {
            continue;
        }
        for (int j=0; j<NB_OBSTACLES; j++)
        {
            if (!obstacles[j].active || obstacles[i].id == obstacles[j].id)
            {
                continue;
            }
            if (CheckCollisionRecs(obstacles[i].rectangle, obstacles[j].rectangle))
            {
                caravana = true;
                if (obstacles[i].position.x < obstacles[j].position.x)
                {
                    obstacles[j].position.x = obstacles[i].position.x + obstacles[i].rectangle.width + 1;
                }
                else 
                {
                    obstacles[i].position.x = obstacles[j].position.x + obstacles[j].rectangle.width + 1;
                }
            }
        }
    }
    time_since_horn += GetFrameTime();
    if (caravana && time_since_horn > 2.0f) {
        time_since_horn = 0.0f;
        PlaySound(fxHorns[rand()%3]);
    }

}


