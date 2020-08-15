#ifndef _Playerturn_H_
#define _Playerturn_H_

typedef struct playermove{
    float *change;
    float other;
    float *speed;
}playermove;
int Playerturn(playermove move);
#endif