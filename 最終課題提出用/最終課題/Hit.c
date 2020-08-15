#include <math.h>
//当たり判定設定関数★
//(敵と自機のx距離,敵と自機のy距離,敵と自機の当たり判定,移動させる弾幕のx座標のポインタ,移動させる弾幕のy座標のポインタ,移動先のx座標,移動先のy座標,敵にダメージを与えるか否か,体力の記録場所,総体力の記録場所,スコアの記録場所,次のHPの記録場所,wave数の記録場所,wave上限の記録場所,グレイズしたかどうかの記録場所,グレイズ数の記録場所,敵無敵時間の記録場所,残機の記録場所,シーンの記録場所,プレイヤー無敵時間の記録場所)
typedef struct hitdata{
    float powx;
    float powy;
    float detectzone;
    float *bulletx;
    float *bullety;
    int x;
    int y;
    int damage;
    int *HP;
    int *TotalHP;
    int *score;
    int *nextHP;
    int *wave;
    int *maxwave;
    float *grazedata;
    int *grazecount;
    int *nodamage;
    int *playerlife;
    int *gameover;
    int *pnodamage;
}hitdata;
int hit(hitdata hd){
    //弾幕との距離を求める
    int i=powf(hd.powx,2)+powf(hd.powy,2);//弾と標的の距離
    int ii=powf(hd.detectzone,2);//被弾
    int iii=powf(hd.detectzone+5,2);//グレイズ
    if(i<=ii){
        //プレイヤーの弾が当たったら体力を減らし、当たった弾を消してスコア付与
        if(hd.damage!=0&&*hd.HP>0){
            *hd.HP-=10;
            *hd.TotalHP-=10;
            *hd.bulletx=hd.x;
            *hd.bullety=hd.y;
            if(*hd.score<99999999){
                *hd.score+=100;
            }
            else{
                *hd.score=99999999;
            }
        }
        //体力を削りきったら次のwaveへ、完全撃破したら総体力を０に
        if(hd.damage!=0&&*hd.HP<=0){
            *hd.HP=*hd.nextHP;
            *hd.nodamage=10;
            if(*hd.wave+1<=*hd.maxwave){
                *hd.wave+=1;
            }
            else{
                *hd.HP=0;
                *hd.TotalHP=0;
            }
        }
        //被弾したら残機を減らし、無敵時間を与える。残機０になったらゲームオーバーに
        if(hd.damage==0){
            if(*hd.pnodamage<=0){
                *hd.bulletx=hd.x;
                *hd.bullety=hd.y;
                *hd.pnodamage=30;
                *hd.playerlife-=1;
            }
            if(*hd.playerlife<0){
                *hd.gameover=3;
            }
        }
    }
    //かすったらグレイズの数を増やし、スコアを付与
    if(i>ii&&i<=iii&&hd.damage==0&&*hd.grazedata==0){
        if(*hd.grazecount<99999999){
            *hd.grazecount+=1;
        }
        *hd.grazedata=1;
        if(*hd.score<=99999999){
            *hd.score+=200;
        }
        if(*hd.score>99999999){
            *hd.score=99999999;
        }
    }
    return 0;
}