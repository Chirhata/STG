//自機方向転換関数
//(どの向きの異動フラグを切るか,その向きと逆のフラグ,移動量)
typedef struct playermove{
    float *change;
    float other;
    float *speed;
}playermove;
int Playerturn(playermove move){
    //話したキーが示す向きの移動量を０にし、逆方向に動くキーが押されてたらその値を与える
    *move.change=0;
    *move.speed=move.other;
    return 0;
}