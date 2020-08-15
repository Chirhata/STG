#ifndef _savebulletdata_H_
#define _savebulletdata_H_

typedef struct bulletdata{
    float *xmoveplace;
    float xmove;
    float *ymoveplace;
    float ymove;
    float *speedplace;
    float speed;
}bulletdata;
int Savebulletdata(bulletdata data);
#endif