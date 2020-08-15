#ifndef _PandEmove_H_
#define _PandEmove_H_
typedef struct pemove{
    float *targetpositionpoint;
    int targethit;
    float speed;
    int movablemin;
    int movablemax;
}pemove;
int PandEmove(pemove pandmove);
#endif