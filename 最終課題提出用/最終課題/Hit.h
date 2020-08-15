#ifndef _hit_H_
#define _hit_H_
typedef struct hitdata{
    float powx;
    float powy;
    float detectzone;
    float *bulletx;
    float *bullety;
    int x;
    int y;
    int damage;
    int *HP;
    int *TotalHP;
    int *score;
    int *nextHP;
    int *wave;
    int *maxwave;
    float *grazedata;
    int *grazecount;
    int *nodamage;
    int *playerlife;
    int *gameover;
    int *pnodamage;
}hitdata;
int hit(hitdata hd);
#endif