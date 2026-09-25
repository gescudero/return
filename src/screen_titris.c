/***
 *
 */

#include "colors.h"
#include "models_titris.h"
#include "raylib.h"
#include "screens.h"
#include <stdio.h>
#include <stdlib.h>         // Required for: rand()
#include <stdbool.h>        // Required for bool types

/***
 * Variable Definitions
 */
int grid[NB_ROWS][NB_COLS] = {0};
queue_t piezas;
Color colores[9] = {VN_BLACK, VN_BLUE, VN_LT_PURPLE, VN_LT_GREEN, VN_GN_YELLOW, VN_ORANGE, VN_PINK, VN_RED, VN_WHITE};
Texture2D textures[NB_TEXTURES] = {0};
screenpiece_t current_piece = {0};
float time_accum = 0.0f;
float update_interval = 0.5f;
bool is_cleaning = false;
int completed_col = -1;
titris_states_t game_state = TITRIS_ACTIVE;
float score = 0.0f;
int total_passengers = 35;

/***
 * Functions declarations
 */
static void InitTextures();
static void UnloadTextures();
static pieza_t GetNextPieza(queue_t *cola);
static void AddPiezaToList(queue_t *cola, pieza_t p);
static void FillPiezasList(queue_t *cola);
static void TitrisGameOver(void);
static bool PieceCanMoveDown(Vector2 prev_pos, const pieza_t *pieza);
static bool PieceCanMoveUp(Vector2 prev_pos, const pieza_t *pieza);
static bool PieceCanMoveLeft(Vector2 prev_pos, const pieza_t *pieza);
static bool PieceCanRotate(Vector2 pos, pieza_t *pieza);
static void UpdatePiece(bool isDrawing);
static int CheckColsCompleted(void);
static void CleanColCompleted(int completed_col);
static void SetRowWhite(int row_number);
static void UpdateLoop(void);
/***
 * Public Function Definitions
 */
void InitTitrisScreen(void) {
    // Init vars, load textures and all stuff
    // needed before main loop.
    InitTitrisModels();
    InitTextures();
    FillPiezasList(&piezas);
    game_state = TITRIS_ACTIVE;
    current_piece.active = false;
}
void UpdateTitrisScreen(void) {
    // Update data in the main loop
    time_accum += GetFrameTime();
    if (game_state == TITRIS_ACTIVE) UpdateLoop();

}
void DrawTitrisScreen(void) {
    // Draw screen 
    ClearBackground(XT_DK_GREY);
    // GRID 
    for (int col=NB_COLS-1; col>=0; col--) {
        for (int row=0; row<NB_ROWS; row++) {
            int cell_value = grid[row][col];
            if (cell_value == 0) {
                DrawRectangle(CELL_SIZE*col+1, CELL_SIZE*row+1, CELL_SIZE-1, CELL_SIZE-1, colores[cell_value]);
            } else {
                DrawTexture(textures[cell_value % NB_TEXTURES], CELL_SIZE*col, CELL_SIZE*row, RAYWHITE);
            }
        }
    }
    // NEXT PIECES
    for (int i=0; i<NB_PIEZAS; i++) {
        int index = (piezas.head + i) % NB_PIEZAS;
        const char **p = piezas.list[index].formas[3];
        Color c = colores[piezas.list[index].color];
        int start_posx = (450 - (100 * i)) - 2;
        int start_posy = 300;

        for (int row=0; row<4; row++) {
            for (int col=0; col<4; col++) {
                if (p[row][col] != '0') {
                    DrawTexture(textures[piezas.list[index].color % NB_TEXTURES], start_posx + CELL_SIZE*col, start_posy + CELL_SIZE*row, RAYWHITE);
                    DrawRectangle(start_posx + (CELL_SIZE*col+1), start_posy + (CELL_SIZE*row+1), CELL_SIZE-1, CELL_SIZE-1, c);
                }
            }
        }
    }

    // PASSENGERS
    DrawText("Passengers Remaining:", 550, 30, 20, VN_PINK);
    DrawText(TextFormat("%i", total_passengers), 550, 60, 30, VN_PINK);

    
    // GAME OVER
    if (game_state == TITRIS_GAME_OVER) {
        DrawText("AUTOBUS LLENO", 50, 100, 40, VN_LT_GREEN);
        DrawText("TENDRÁS QUE ESPERAR", 50, 150, 40, VN_LT_GREEN);
        DrawText("AL SIGUIENTE", 50, 200, 40, VN_LT_GREEN);
    }
    // WIN GAME
    if (game_state == TITRIS_WIN_GAME) {
        DrawText("HAS CONSEGUIDO", 50, 100, 40, VN_LT_GREEN);
        DrawText("ESPACIO EN EL ", 50, 150, 40, VN_LT_GREEN);
        DrawText("AUTOBUS...", 50, 200, 40, VN_LT_GREEN);
    }
}
void UnloadTitrisScreen(void) {
    // Unload textures, free data, etc..
    UnloadTextures();
}
int FinishTitrisScreen(void) {
    return 0;
}

/***
 * Private functions
 */
static void InitTextures() {
    textures[0] = LoadTexture("./resources/titris_person1.png");
    textures[1] = LoadTexture("./resources/titris_person2.png");
    textures[2] = LoadTexture("./resources/titris_person3.png");
    textures[3] = LoadTexture("./resources/titris_person4.png");
    textures[4] = LoadTexture("./resources/titris_person5.png");
}
static void UnloadTextures() {
    for (int i=0; i<NB_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
}
static pieza_t GetNextPieza(queue_t *cola) {
    pieza_t p = cola->list[cola->head];
    cola->head = (cola->head + 1) % NB_PIEZAS;
    cola->count--;
    return p;
}
static void AddPiezaToList(queue_t *cola, pieza_t p) {
    if (cola->count >= NB_PIEZAS) {
        return;
    }
    cola->list[cola->tail] = p;
    cola->tail = (cola->tail + 1) % NB_PIEZAS;
    cola->count++;
}
static void FillPiezasList(queue_t *cola_piezas) {
    for (int i=0; i<NB_PIEZAS; i++) {
        AddPiezaToList(cola_piezas, GetPieza(rand() % 7));
    }
}
static void TitrisGameOver(void) {
    game_state = TITRIS_GAME_OVER;
}
static bool PieceCanMoveDown(Vector2 prev_pos, const pieza_t *pieza) {
    int final_row = prev_pos.y + 1;
    int final_col = prev_pos.x;
    const char **forma = pieza->formas[pieza->rot_state];
    for (int col=0; col<4; col++) {
        for (int row=0; row<4; row++) {
            // comprobamos si alguno de los '1' se sale por abajo
            if (forma[row][col] != '0' && final_row+row >= NB_ROWS) {
                return false;
            }
            // comprobamos si cada uno de los '1' de nuestra forma coincide con 
            // alguna celda ya pintada
            if (forma[row][col] != '0' && grid[final_row+row][final_col+col] != 0) {
                return false;
            }
        }
    }
    return true;
}
static bool PieceCanMoveUp(Vector2 prev_pos, const pieza_t *pieza) {
    int final_row = prev_pos.y - 1;
    int final_col = prev_pos.x;
    const char **forma = pieza->formas[pieza->rot_state];
    for (int col=0; col<4; col++) {
        for (int row=0; row<4; row++) {
            // comprobamos si alguno de los '1' se sale por arriba
            if (forma[row][col] != '0' && final_row+row < 0) {
                return false;
            }
            // comprobamos si cada uno de los '1' de nuestra forma coincide con 
            // alguna celda ya pintada
            if (forma[row][col] != '0' && grid[final_row+row][final_col+col] != 0) {
                return false;
            }
        }
    }
    return true;
}
static bool PieceCanMoveLeft(Vector2 prev_pos, const pieza_t *pieza) {
    int final_row = prev_pos.y;
    int final_col = prev_pos.x - 1;
    const char **forma = pieza->formas[pieza->rot_state];
    for (int col=0; col<4; col++) {
        for (int row=0; row<4; row++) {
            // si nos salimos de la grid por arriba o por debajo, lo ignoramos
            // para evitar overflows
            if (final_row+row >= NB_ROWS || final_col+col >= NB_COLS) {
                continue;
            }
            // comprobamos si alguno de los '1' toca la pared izquierda
            if (forma[row][col] != '0' && final_col + col < 0 ) {
                return false;
            }
            // comprobamos si cada uno de los '1' de nuestra forma coincide con 
            // alguna celda ya pintada, si coincide y la posicion es la inicial
            // entonces se acabo el juego
            if (forma[row][col] != '0' && grid[final_row+row][final_col+col] != 0) {
                if (prev_pos.x >= NB_COLS-1) TitrisGameOver();
                return false;
            }
        }
    }
    return true;
}
static bool PieceCanRotate(Vector2 pos, pieza_t *pieza) {
    int final_row = pos.y;
    int final_col = pos.x;
    int prev_rotation = pieza->rot_state;
    RotatePieceRight(pieza);
    const char **forma = pieza->formas[pieza->rot_state];
    for (int row=0; row<4; row++) {
        for (int col=0; col<4; col++) {
            if (forma[row][col] != '0' && final_row + row >= NB_ROWS ) {
                pieza->rot_state = prev_rotation;
                return false;
            }
            if (forma[row][col] != '0' && final_col + col < 0 ) {
                pieza->rot_state = prev_rotation;
                return false;
            }
            if (forma[row][col] != '0' && final_col + col >= NB_COLS ) {
                pieza->rot_state = prev_rotation;
                return false;
            }
            int cell_value = grid[final_row+row][final_col+col];
            if (forma[row][col] != '0' && cell_value != 0) {
                pieza->rot_state = prev_rotation;
                return false;
            }
        }
    }
    return true;
}
static void UpdatePiece(bool isDrawing) {
    int posx = 0;
    int posy = 0;
    pieza_t *p = &current_piece.pieza;
    for (int row=0; row<4; row++) {
        for (int col=0; col<4; col++) {
            int value = p->formas[p->rot_state][row][col] - '0';
            if (value == 1) {
                posx = current_piece.pos.x + col;
                posy = current_piece.pos.y + row;
                if (posy < 0 || posy >= NB_ROWS || posx < 0 || posx >= NB_COLS) {
                    continue;
                }
                if (isDrawing) {
                    grid[posy][posx] = p->color;
                } else {
                    grid[posy][posx] = 0;
                }
            }
        }
    }
}
static int CheckColsCompleted(void) {
    bool completed = false;
    for (int col=NB_COLS-1; col>=0; col--) {
        for (int row=0; row<NB_ROWS; row++) {
            if (grid[row][col] != 0) {
                completed = true;
            } else {
                completed = false;
                // saltamos a la siguiente columna
                break;
            }
        }

        if (completed) {
            if (!is_cleaning) time_accum = 0.0f;
            is_cleaning = true;
            return col;
        }
    }
    return -1;
}
static void CleanColCompleted(int completed_col) {
    current_piece.pos.x = NB_COLS + 3;
    for (int col=completed_col; col<NB_ROWS-1; col++) {
        for (int row=0; row<NB_ROWS; row++) {
            grid[row][col] = grid[row][col+1];        
        }
    }
}
static void SetColWhite(int col_number) {
    for (int row=0; row<NB_ROWS; row++) {
        grid[row][col_number] = 8;
    }
}
static void UpdateLoop(void) {
    if (!current_piece.active) {
        // Desactivamos la limpieza de lineas completas
        // para ver el gameplay sin ellas
        //completed_col = CheckColsCompleted();
        if (is_cleaning) {
            if (time_accum < update_interval) {
                SetColWhite(completed_col);
            } else {
                // limpiamos despues de completar la animacion
                score += 100;
                CleanColCompleted(completed_col);
                is_cleaning = false;
                completed_col = -1;
            }
            
        } else {
            score += 10;
            total_passengers--;
            if (total_passengers <= 0) {
                game_state = TITRIS_WIN_GAME;
                return;
            }
            current_piece.pos.x = NB_COLS - 1;
            current_piece.pos.y = 5;
            // Cogemos la siguiente pieza de la lista
            // y añadimos otra pieza random al final
            current_piece.pieza = GetNextPieza(&piezas);
            AddPiezaToList(&piezas, GetPieza(rand() % 7));
            current_piece.active = true;
            
            // DIFFICULT
            int level = (int)score/1000;
            switch (level) {
                case 1:
                    update_interval = 0.35f;
                    break;
                case 2:
                    update_interval = 0.3f;
                    break;
                case 3:
                    update_interval = 0.25f;
                    break;
                case 4:
                    update_interval = 0.2f;
                    break;
                case 5:
                    update_interval = 0.15f;
                    break;
                case 6:
                    update_interval = 0.1f;
                    break;
                case 7:
                    update_interval = 0.05f;
                    break;
                default:
                    break;
            }
        }
    } else {
        // printf("Si hay pieza activa\n");
        // printf("%f", time_accum);
    // Si ya hay pieza activa, la desplazamos un row, pero antes limpiamos
    // los cuadrados que hubiera pintados del frame anterior
        UpdatePiece(false);

        // Solo se actualiza si ha pasado el tiempo necesario
        // (Control de la velocidad)
        if (time_accum >= update_interval) {
            time_accum = 0.0f;
            // printf("Toca moverse. Comprobamos:\n");
            if (PieceCanMoveLeft(current_piece.pos, &current_piece.pieza)) {
                // printf("Ha podido moverse a la izquierda\n");
                current_piece.pos.x--;
            } else {
                // printf("No puede moverse a la izquierda\n");
                current_piece.active = false;
            }
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        if (PieceCanMoveLeft(current_piece.pos, &current_piece.pieza)) {
            current_piece.pos.x--;
        } else {
            current_piece.active = false;
        }
    }
    if (IsKeyPressed(KEY_UP) && PieceCanMoveUp(current_piece.pos, &current_piece.pieza)) {
        current_piece.pos.y--;
    } 
    if (IsKeyPressed(KEY_RIGHT)) {
        PieceCanRotate(current_piece.pos, &current_piece.pieza);
    } 
    if (IsKeyPressed(KEY_DOWN) && PieceCanMoveDown(current_piece.pos, &current_piece.pieza)) {
            current_piece.pos.y++;
    }
    if (!is_cleaning) UpdatePiece(true);
}
