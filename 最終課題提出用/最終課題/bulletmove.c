#include <math.h>
//弾移動関数
//(弾の現在のx座標,弾の現在のy座標,どれだけx方向に動かすか,どれだけy方向に動かすか,動かす弾のx座標のポインタ,動かす弾のy座標のポインタ,動けるx範囲の上限,動けるx範囲の下限,動けるy範囲の上限,動けるy範囲の下限,弾幕の存在を記録する場所)
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
int Movebullet(movebulletdata movedata){
    //弾を動かす
    *movedata.bulletx = movedata.bulletnowx+movedata.movex;
    *movedata.bullety = movedata.bulletnowy+movedata.movey;
    //弾が画面外に出たら描写するように設定し、画面外に出たら消去する
    if(movedata.bulletnowx+movedata.movex<movedata.movablemaxX+5&&movedata.bulletnowx+movedata.movex>movedata.movableminX-5&&movedata.bulletnowy+movedata.movey<movedata.movablemaxY+5&&movedata.bulletnowy+movedata.movey>movedata.movableminY-5){
        *movedata.deletebullet = 1;
    }
    else{
        *movedata.deletebullet = 0;
    }
    return 0;
}