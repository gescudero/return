#include "models_titris.h"
#include <stdlib.h>

const char *pieza_l_a[] = {
    "0100",
    "0100",
    "0110",
    "0000"
};
const char *pieza_l_b[] = {
    "0010",
    "1110",
    "0000",
    "0000"
};
const char *pieza_l_c[] = {
    "1100",
    "0100",
    "0100",
    "0000"
};
const char *pieza_l_d[] = {
    "1110",
    "1000",
    "0000",
    "0000"
};
const char *pieza_j_a[] = {
    "0100",
    "0100",
    "1100",
    "0000"
};
const char *pieza_j_b[] = {
    "1000",
    "1110",
    "0000",
    "0000"
};
const char *pieza_j_c[] = {
    "1100",
    "1000",
    "1000",
    "0000"
};
const char *pieza_j_d[] = {
    "1110",
    "0010",
    "0000",
    "0000"
};
const char *pieza_s_a[] = {
    "0110",
    "1100",
    "0000",
    "0000"
};
const char *pieza_s_b[] = {
    "1000",
    "1100",
    "0100",
    "0000"
};
const char *pieza_z_a[] = {
    "1100",
    "0110",
    "0000",
    "0000"
};
const char *pieza_z_b[] = {
    "0100",
    "1100",
    "1000",
    "0000"
};
const char *pieza_o[] = {
    "1100",
    "1100",
    "0000",
    "0000"
};

const char *pieza_i_a[] = {
    "0100",
    "0100",
    "0100",
    "0100"
};
const char *pieza_i_b[] = {
    "0000",
    "1111",
    "0000",
    "0000"
};

const char *pieza_t_a[] = {
    "0100",
    "0110",
    "0100",
    "0000"
};
const char *pieza_t_b[] = {
    "0000",
    "1110",
    "0100",
    "0000"
};
const char *pieza_t_c[] = {
    "0100",
    "1100",
    "0100",
    "0000"
};
const char *pieza_t_d[] = {
    "0100",
    "1110",
    "0000",
    "0000"
};

pieza_t t = {0};
pieza_t l = {0};
pieza_t j = {0};
pieza_t s = {0};
pieza_t z = {0};
pieza_t o = {0};
pieza_t i = {0};

void InitTitrisModels() {
    // T 
    t.id = 0;
    t.rot_state = ROT_UP;
    t.color = 1;
    t.formas = malloc(4*sizeof(char**));
    t.formas[0] = pieza_t_b;
    t.formas[1] = pieza_t_c;
    t.formas[2] = pieza_t_d;
    t.formas[3] = pieza_t_a;
    // L 
    l.id = 1;
    l.rot_state = ROT_UP;
    l.color = 2;
    l.formas = malloc(4*sizeof(char**));
    l.formas[0] = pieza_l_b;
    l.formas[1] = pieza_l_c;
    l.formas[2] = pieza_l_d;
    l.formas[3] = pieza_l_a;
    // J 
    j.id = 2;
    j.rot_state = ROT_UP;
    j.color = 3;
    j.formas = malloc(4*sizeof(char**));
    j.formas[0] = pieza_j_b;
    j.formas[1] = pieza_j_c;
    j.formas[2] = pieza_j_d;
    j.formas[3] = pieza_j_a;
    // S 
    s.id = 3;
    s.rot_state = ROT_UP;
    s.color = 4;
    s.formas = malloc(4*sizeof(char**));
    s.formas[0] = pieza_s_b;
    s.formas[1] = pieza_s_a;
    s.formas[2] = pieza_s_b;
    s.formas[3] = pieza_s_a;
    // Z 
    z.id = 4;
    z.rot_state = ROT_UP;
    z.color = 5;
    z.formas = malloc(4*sizeof(char**));
    z.formas[0] = pieza_z_b;
    z.formas[1] = pieza_z_a;
    z.formas[2] = pieza_z_b;
    z.formas[3] = pieza_z_a;
    // O 
    o.id = 5;
    o.rot_state = ROT_UP;
    o.color = 6;
    o.formas = malloc(4*sizeof(char**));
    o.formas[0] = pieza_o;
    o.formas[1] = pieza_o;
    o.formas[2] = pieza_o;
    o.formas[3] = pieza_o;
    // I 
    i.id = 6;
    i.rot_state = ROT_UP;
    i.color = 7;
    i.formas = malloc(4*sizeof(char**));
    i.formas[0] = pieza_i_b;
    i.formas[1] = pieza_i_a;
    i.formas[2] = pieza_i_b;
    i.formas[3] = pieza_i_a;
}

void RotatePieceRight(pieza_t *p) {
    if (p->rot_state < 3) {
        p->rot_state++;
    } else {
        p->rot_state = 0;
    }
}
pieza_t GetPieza(int id) {
    switch (id) {
        case 0: return t; break;
        case 1: return l; break;
        case 2: return j; break;
        case 3: return s; break;
        case 4: return z; break;
        case 5: return o; break;
        case 6: return i; break;
        default: return t;
    }
}

