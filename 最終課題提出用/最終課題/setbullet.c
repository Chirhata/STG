//弾幕設置関数
//(弾のx座標を入れる場所,弾のy座標を入れる場所,設置場所のx座標,設置場所のy座標,敵or自機の無敵時間を記録する場所,弾の存在を示す場所,グレイズしたかを記録する場所)
typedef struct setbullet{
    float *bulletx;
    float *bullety;
    float x;
    float y;
    int *alive;
    float *set;
    float *resetgraze;
}setbullet;
int Setbullet(setbullet set){
    //無敵時間内でなければ、指定した位置に弾を設置、設置した弾の存在を肯定、グレイズ判定を初期化する
    if(*set.alive==0){
        *set.bulletx=set.x;
        *set.bullety=set.y;
        *set.set=0;
        *set.resetgraze=0;
    }
    return 0;
}