#include <stdlib.h>
//適移動方向設定関数
//(動きを記録する場所,移動量,敵の座標,同じ方向にどれだけ動いたかを記録する場所,移動下限,移動上限)
typedef struct enemymovedata{
    float *move;
    float movee;
    float x;
    float *count;
    int movablemin;
    int movablemax;
}enemymovedata;
int Setenemymove(enemymovedata emove){
    //同じ方向に移動しすぎないように、カウンターを操作する
    if(*emove.move>0){
        *emove.count+=1;
    }
    if(*emove.move<0){
        *emove.count-=1;
    }
    //カウンターが一定値に達するか移動範囲外に出ようとしたら、向きを逆転させる
    if((emove.x>emove.movablemin&&*emove.move>0)||*emove.count>2){
        *emove.move=emove.movee*(-1.0);
        if(*emove.count>2){
            *emove.count=2-rand()%4;
        }
    }
    if((emove.x<emove.movablemax&&*emove.move<0)||*emove.count<-2){
        *emove.move=emove.movee*(-1.0);
        if(*emove.count<-2){
            *emove.count=2-rand()%4;
        }
    }
    return 0;
}