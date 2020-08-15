#ifndef _Setbullet_H_
#define _Setbullet_H_
typedef struct setbullet{
    float *bulletx;
    float *bullety;
    float x;
    float y;
    int *alive;
    float *set;
    float *resetgraze;
}setbullet;
int Setbullet(setbullet set);
#endif

