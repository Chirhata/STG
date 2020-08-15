//データ挿入
//(x方向への速度を記録する場所,x方向にどれだけ動かすか,y方向への速度を記録する場所,y方向にどれだけ動かすか,弾幕の速度を記録する場所,速度）
typedef struct bulletdata{
    float *xmoveplace;
    float xmove;
    float *ymoveplace;
    float ymove;
    float *speedplace;
    float speed;
}bulletdata;
int Savebulletdata(bulletdata data){
    //指定された配列を見て、動いていない弾幕に速度を与える
    if(*data.xmoveplace==0&&*data.ymoveplace==0){
        *data.xmoveplace=data.xmove;
        *data.ymoveplace=data.ymove;
        *data.speedplace=data.speed;
    }
    return 0;
}