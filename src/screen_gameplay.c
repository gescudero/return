/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
Vector2 player_start_pos = {368, 380};

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
static void UpdatePlayer(Player *player);
static void DrawPlayer(Player *player);
static void UpdateObstacle(Obstacle *obstacle);
static void DrawObstacles();
static void FixCollisionObstacles();

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    
    // Edit image for player texture
    Image player_img = LoadImage("../resources/DC_1974.png");
    ImageResize(&player_img, 64, 64);

    player.position = player_start_pos;
    player.velocity = (Vector2){0, 0};
    player.texture = LoadTextureFromImage(player_img);
    player.lives = 3;
    player.speed = 100.0f;
    player.row = 0;
    player.rectangle = (Rectangle){player.position.x, player.position.y, 32, 64};
    player.last_rebirth = framesCounter;
    
    srand(time(NULL));

    // Edit image for obstacles
    Image obstacle_img = LoadImage("../resources/DC_1974.png");
    ImageResize(&obstacle_img, 64, 64);
    for ( int i=0; i<NB_OBSTACLES; i++ )
    {
        obstacles[i].id = i;
        obstacles[i].texture = LoadTextureFromImage(obstacle_img);
        obstacles[i].row = i % (NB_LANES - 2);
        obstacles[i].position = (Vector2){ GetScreenWidth(), 90 + (obstacles[i].row * 75) };
        obstacles[i].speed = 100.0f + (rand() % 30);
        obstacles[i].active = false;
        obstacles[i].rectangle = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, 96, 44};
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

    // Unload images
    UnloadImage(player_img);
    UnloadImage(obstacle_img);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    framesCounter++;

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
    // Check collision with player
    if (IsKeyPressed(KEY_W))
    {
        player.row++;
    }
    if (IsKeyPressed(KEY_S))
    {
        player.row--;
    }
    if (IsKeyPressed(KEY_A))
    {
        player.position.x -= 25;
    }
    if (IsKeyPressed(KEY_D))
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

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // Background color
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), VN_MAROON);
    for ( int i=0; i<8; i++ ) 
    {
        DrawRectangle(0, (int)(GetScreenHeight()/NB_LANES)*i, GetScreenWidth(), 4, VN_WHITE);
    }
    DrawText(TextFormat("%i", framesCounter), 10, 10, 10, VN_BLACK);
    DrawPlayer(&player);
    DrawObstacles();

}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadTexture(player.texture);
    for (int i=0; i<NB_OBSTACLES; i++)
    {
        UnloadTexture(obstacles[i].texture);
    }
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
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
    DrawRectangleRec(player->rectangle, Fade(VN_BK_WHITE, alpha));
}
static void UpdateObstacle(Obstacle *obstacle)
{
    if (obstacle->position.x > -132 )
    {
        obstacle->position.x -= obstacle->speed * GetFrameTime();
    } 
    else 
    {
        obstacle->position.x = GetScreenWidth();
        obstacle->active = false;
    }
    obstacle->rectangle.x = obstacle->position.x;

}
static void DrawObstacles()
{
    for (int i=0; i<NB_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            // DrawTextureV(obstacles[i].texture, obstacles[i].position, GRAY);
            // DrawRectangleRec(obstacles[i].rectangle, obstacles[i].color);
            // pinto el rectangulo 2 pixeles mas pequeño de ancho y de alto para que no lleguen a colisionar
            DrawRectangle(obstacles[i].rectangle.x + 1, obstacles[i].rectangle.y + 1, obstacles[i].rectangle.width - 2 , obstacles[i].rectangle.height - 2, obstacles[i].color);
        }
    }
}

static void FixCollisionObstacles()
{
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
                if (obstacles[i].position.x < obstacles[j].position.x)
                {
                    obstacles[j].position.x = obstacles[i].position.x + obstacles[j].rectangle.width + 1;
                }
                else 
                {
                    obstacles[i].position.x = obstacles[j].position.x + obstacles[i].rectangle.width + 1;
                }
            }
        }
    }
}


