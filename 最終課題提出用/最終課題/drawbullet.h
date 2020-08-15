#ifndef _Drawbullet_H_
#define _Drawbullet_H_
#include <handy.h>

typedef struct drawbulletdata{
    float bulletexist;
    int layer;
    float bulletx;
    float bullety;
    int bullettype;
    float bulletsize;
}drawbulletdata;
int Drawbullet(drawbulletdata draw);
#endif