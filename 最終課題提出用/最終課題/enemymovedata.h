#ifndef _Setenemymove_H_
#define _Setenemymove_H_
typedef struct enemymovedata{
    float *move;
    float movee;
    float x;
    float *count;
    int movablemin;
    int movablemax;
}enemymovedata;
int Setenemymove(enemymovedata emove);
#endif