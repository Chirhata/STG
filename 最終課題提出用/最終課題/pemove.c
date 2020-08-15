//自機と敵を動かす関数
//(動かす対象,対象の当たり判定,動かす速度,動かす範囲指定　最小値,動かす範囲指定　最大値)
typedef struct pemove{
    float *targetpositionpoint;
    int targethit;
    float speed;
    int movablemin;
    int movablemax;
}pemove;
int PandEmove(pemove pandmove){
    //現在地、動ける範囲を与え、範囲内なら動かし、範囲外に出ようとしたら止まらせる
    if(*pandmove.targetpositionpoint>=pandmove.movablemin+pandmove.targethit&&*pandmove.targetpositionpoint<=pandmove.movablemax-pandmove.targethit){
        *pandmove.targetpositionpoint+=pandmove.speed;
        if(*pandmove.targetpositionpoint<pandmove.movablemin+pandmove.targethit){
            *pandmove.targetpositionpoint=pandmove.movablemin+pandmove.targethit;
        } 
        if(*pandmove.targetpositionpoint>pandmove.movablemax-pandmove.targethit){
            *pandmove.targetpositionpoint=pandmove.movablemax-pandmove.targethit;
        }
    }
    return 0;
}