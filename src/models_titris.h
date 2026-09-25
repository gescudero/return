#ifndef MODELS_TITRIS_H
#define MODELS_TITRIS_H

#include "raylib.h"


#define NB_COLS 18
#define NB_ROWS 9
#define NB_PIEZAS 5
#define NB_TEXTURES 5
#define CELL_SIZE 32

typedef enum{
    ROT_UP = 0,
    ROT_RIGHT = 1,
    ROT_DOWN = 2,
    ROT_LEFT = 3
} rot_states;

typedef enum {
    TITRIS_ACTIVE,
    TITRIS_GAME_OVER,
    TITRIS_WIN_GAME
} titris_states_t;

typedef struct pieza_t{
    int id;
    rot_states rot_state;
    int color;
    const char ***formas;
}pieza_t ;

typedef struct screenpiece_t{
    Vector2 pos;
    bool active;
    pieza_t pieza;
} screenpiece_t;

typedef struct queue_t{
    pieza_t list[NB_PIEZAS];
    int head;
    int tail;
    int count;
} queue_t;


extern pieza_t t;
extern pieza_t l;
extern pieza_t s;
extern pieza_t z;
extern pieza_t o;
extern pieza_t i;


void InitTitrisModels();
void RotatePieceRight(pieza_t *p);
void RotatePieceLeft(pieza_t *p);
pieza_t GetPieza(int id);

#endif
