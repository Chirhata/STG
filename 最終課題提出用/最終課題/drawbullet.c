#include <handy.h>
//弾描写関数
//(画面外にあるか否か記録されてる場所,レイヤー,弾のx座標,弾のy座標,描写する弾の種類,弾の大きさ)
typedef struct drawbulletdata{
    float bulletexist;
    int layer;
    float bulletx;
    float bullety;
    int bullettype;
    float bulletsize;
}drawbulletdata;
int Drawbullet(drawbulletdata draw){
    //弾が存在するなら、指定した弾を描写する
    if(draw.bulletexist!=0){
        HgWImagePut(draw.layer,draw.bulletx,draw.bullety,draw.bullettype,draw.bulletsize,0);
    }
    return 0;
}