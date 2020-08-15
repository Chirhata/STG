#ifndef _Movebullet_H_
#define _Movebullet_H_


typedef struct movebulletdata{
    float bulletnowx;
    float bulletnowy;
    float movex;
    float movey;
    float *bulletx;
    float *bullety;
    int movablemaxX;
    int movableminX;
    int movablemaxY;
    int movableminY;
    float *deletebullet;
}movebulletdata;
int Movebullet(movebulletdata movedata);
#endif