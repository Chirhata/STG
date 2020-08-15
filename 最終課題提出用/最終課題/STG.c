//853743 林拓也 1/11
#include <stdio.h>
#include <handy.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "bulletmove.h"
#include "setbullet.h"
#include "Hit.h"
#include "savebullet.h"
#include "playerturn.h"
#include "pemove.h"
#include "drawbullet.h"
#include "enemymovedata.h"


#define wsx 700//画面の横の大きさ
#define wsy 640//画面の縦の大きさ
#define titlecontents 2//タイトル画面の項目数
#define enemylife 4//wave数上限
#define movablex 400//移動できるx距離
#define movabley 600//移動できるy距離
#define xmargine 20//移動できるx距離をどれだけずらすか
#define ymargine 20//移動できるy距離をどれだけずらすか
#define fplayerpositionx 200//プレイヤーの初期位置x
#define fplayerpositiony 50//プレイヤーの初期位置y
#define enemylife 4//wave数上限
doubleLayer layers;
int windowID;//ウィンドウ
int layerID;//レイヤー
int game = 0;//シーン判定
//x座標,y座標,x軸への動き,y軸への動き,存在
float bullet[30][5] = {};
float rightsidebullet[30][5] = {};
float leftsidebullet[30][5] = {};
float rightsidebullet2[30][5] = {};
float leftsidebullet2[30][5] = {};
//x座標,y座標,x軸への動き,y軸への動き,速度,存在,グレイズ
float enemybullet[500][7] = {};
float enemybullet2[500][7] = {};
float enemybullet3[500][7] = {};
float empty[1];//捨て駒
int enemyHP[4] = {800,1200,1500,1900};//敵体力
int score[8] = {};//スコア表示用
int graze[8] = {};//グレイズ表示用
int bcount;//自機の弾を置くための変数
int ebcount;//敵の弾を置くための変数
int iii;//弾の角度をいじる変数
int scene;//タイトル,説明書,戦闘画面,クリア画面＆敗北画面
int title;//ステージ選択,説明書,やめる

typedef struct player{
    float x;//自機のx座標
    float y;//自機のy座標
    float fx;//自機の初期x座標
    float fy;//自機の初期y座標
    int maxmx;//自機が動けるx座標の上限
    int maxmy;//自機が動けるy座標の上限
    int minmx;//自機が動けるy座標の下限
    int minmy;//自機が動けるy座標の下限
    float xpbutton;//x移動
    float ypbutton;//y移動
    int life;//残機
    int nodamage;//無敵時間
    int hit;//当たり判定
    //上下左右移動フラグ
    float up;
    float down;
    float right;
    float left;
    //
    int shot;//ショットを撃ってるか判定するフラグ
    int lowspead;//低速移動してるか判定するフラグ
    int cool;//弾を撃つ時のクールタイム
    int graze;//総グレイズ数
    int grazeback;//グレイズ集計
    int score;//スコア
    int scoreback;//スコア集計
    int bonus;//ボーナススコア付与フラグ
}player;
typedef struct enemy{
    float x;//敵のx座標
    float y;//敵のy座標
    int HP;//体力
    int totalHP;//総HP量
    int totalMaxHP;//総HP量上限
    int die;//体力０でフラグ
    int hit;//当たり判定
    float movex;//x軸の移動量
    float movey;//y軸の移動量
    float xcount;//x軸方向に動かす時間
    float ycount;//y軸方向に動かす時間
    int movecool;//動いた後のクールタイム
    int wave;//現在のwave
    int wavelimit;//wave上限
    int nodamage;//無敵時間
    int difficultie;//難易度
}enemy;

int main() {
    //初期設定
    int bmbullet = HgImageLoad("mirache blue middle.png");//中玉　青
    int grbullet = HgImageLoad("mirache green ring.png");//リング玉　緑
    int rmbullet = HgImageLoad("mirache red middle.png");//中玉　赤
    int bsbullet = HgImageLoad("mirache blue star.png");//星弾 青
    int lifemark = HgImageLoad("jagu heart.png");//ハート

    player pstatus;
    enemy estatus;

    pstatus.x = fplayerpositionx;
    pstatus.y = fplayerpositiony;
    pstatus.fx = fplayerpositionx;
    pstatus.fy = fplayerpositiony;
    pstatus.maxmx = movablex;
    pstatus.maxmy = movabley;
    pstatus.minmx = xmargine;
    pstatus.minmy = ymargine;
    pstatus.up = 0;
    pstatus.down = 0;
    pstatus.right = 0;
    pstatus.left = 0;
    pstatus.hit = 4;
    pstatus.xpbutton = 0;
    pstatus.ypbutton = 0;
    pstatus.lowspead = 1;
    pstatus.life = 2;
    pstatus.nodamage = 3;
    pstatus.shot = 0;
    pstatus.graze = 0;
    pstatus.score = 0;
    pstatus.bonus = 0;

    estatus.x = 210;
    estatus.y = 500;
    estatus.xcount = 0;
    estatus.ycount = 0;
    estatus.hit = 10;
    estatus.movecool = -30;
    estatus.HP = enemyHP[0];
    estatus.die = 0;
    estatus.movex = 0;
    estatus.movey = 0;
    estatus.wave = 0;
    estatus.wavelimit = enemylife;
    estatus.nodamage = 0;
    estatus.difficultie = 4;
    estatus.totalHP = 0;
    estatus.totalMaxHP = 0;
    for(int i = enemylife-1;i >= 0;i--){
        estatus.totalHP+ = enemyHP[i];
        estatus.totalMaxHP+ = enemyHP[i];
    }

    //////////////弾の配列を初期化
    for(int i = 29;i >= 0;i--){
        for(int ii = 1;ii >= 0;ii--){
            bullet[i][ii] = 5000;
            rightsidebullet[i][ii] = 5000;
            leftsidebullet[i][ii] = 5000;
            rightsidebullet2[i][ii] = 5000;
            leftsidebullet2[i][ii] = 5000;
        }
    }
    for(int i = 499;i >= 0;i--){
        for(int ii = 1;ii >= 0;ii--){
            enemybullet[i][ii] = 5000;
            enemybullet2[i][ii] = 5000;
            enemybullet3[i][ii] = 5000;
        }
        for(int ii = 4;ii >= 0;ii--){
            enemybullet[i][2+ii] = 0;
            enemybullet2[i][2+ii] = 0;
            enemybullet3[i][2+ii] = 0;
        }
    }

    srand((unsigned)time(NULL));//時間による変数のシード

    //起動
    HgOpen(wsx,wsy);
    layers = HgWAddDoubleLayer(0);
    scene = 0;
    title = 0;
    bcount = 0;
    ebcount = 0;
    iii = 0;
    HgWAddLayer(0);
    while(game == 0){
        //キーボードを押した時の処理
        int ln = HgLSwitch(&layers);
        HgLClear(ln);
        HgSetEventMask(HG_KEY_DOWN|HG_KEY_UP);
        hgevent*event = HgEventNonBlocking();
        if (event !=  NULL&&game == 0){
            if (event->type == HG_KEY_DOWN){
                switch(event->ch){
                    //カーソル移動上,なし,プレイヤー移動上,なし
                    case HG_U_ARROW:
                        switch(scene){
                            case 0:
                                if(title>0){
                                    title--;
                                }
                                break;
                            case 2:
                                pstatus.ypbutton = 0.1;
                                pstatus.up = 0.1;
                                break;
                        }
                        break;
                    //カーソル移動下,なし,プレイヤー移動下,なし
                    case HG_D_ARROW:
                        switch(scene){
                            case 0:
                                if(title<titlecontents){
                                    title++;
                                }
                                break;
                            case 2:
                                pstatus.ypbutton = -0.1;
                                pstatus.down = -0.1;
                                break;
                        }
                        break;
                    //難易度を難しくする,なし,プレイヤー移動右,なし
                    case HG_R_ARROW:
                        switch(scene){
                            case 0:
                                if(estatus.difficultie>1){
                                    estatus.difficultie--;
                                }
                                break;
                            case 2:
                                pstatus.xpbutton = 0.1;
                                pstatus.right = 0.1;
                                break;
                        }
                        break;
                    //難易度を易しくする,なし,プレイヤー移動左,なし
                    case HG_L_ARROW:
                        switch(scene){
                            case 0:
                                if(estatus.difficultie<4){
                                    estatus.difficultie++;
                                }
                                break;
                            case 2:
                                pstatus.xpbutton = -0.1;
                                pstatus.left = -0.1;
                                break;
                        }
                        break;
                    //なし,タイトルに戻る,低速移動,なし
                    case 'z':
                    case 'Z':
                        switch(scene){
                            case 1:
                                scene = 0;
                                break;
                            case 2:
                                pstatus.lowspead = 2;
                                break;
                        }
                        break;
                    //決定,なし,弾を撃つ,なし
                    case 's':
                    case 'S':
                        switch(scene){
                            case 0:
                                switch(title){
                                    //戦闘開始
                                    case 0:
                                        scene = 2;
                                        break;
                                    case 1:
                                        scene = 1;
                                        break;
                                    //ゲーム終了
                                    case 2:
                                        game = 999;
                                        break;
                                }
                                break;
                            case 2:
                                pstatus.shot = 1;
                                if(pstatus.cool == 0){
                                    pstatus.cool = 1;
                                }
                                break;
                        }
                        break;
                    //なし,なし,なし,タイトルに戻って値を初期化
                    case 'e':
                    case 'E':
                        switch(scene){
                            case 3:
                                pstatus.x = fplayerpositionx;
                                pstatus.y = fplayerpositiony;
                                pstatus.fx = fplayerpositionx;
                                pstatus.fy = fplayerpositiony;
                                pstatus.up = 0;
                                pstatus.down = 0;
                                pstatus.right = 0;
                                pstatus.left = 0;
                                pstatus.xpbutton = 0;
                                pstatus.ypbutton = 0;
                                pstatus.lowspead = 1;
                                pstatus.life = 2;
                                pstatus.nodamage = 3;
                                pstatus.shot = 0;
                                pstatus.graze = 0;
                                pstatus.score = 0;
                                pstatus.bonus = 0;
                                estatus.x = 210;
                                estatus.y = 500;
                                estatus.xcount = 0;
                                estatus.ycount = 0;
                                estatus.movecool = -30;
                                estatus.HP = enemyHP[0];
                                estatus.die = 0;
                                estatus.movex = 0;
                                estatus.movey = 0;
                                estatus.wave = 0;
                                estatus.nodamage = 0;
                                estatus.totalHP = estatus.totalMaxHP;
                                scene = 0;
                                for(int i = 499;i >= 0;i--){
                                    for(int ii = 1;ii >= 0;ii--){
                                        enemybullet[i][ii] = 3000;
                                        enemybullet2[i][ii] = 3000;
                                        enemybullet3[i][ii] = 3000;
                                    }
                                    for(int ii = 5;ii >= 0;ii--){
                                        enemybullet[i][2+ii] = 0;
                                        enemybullet2[i][2+ii] = 0;
                                        enemybullet3[i][2+ii] = 0;
                                    }
                                }                  
                                break;
                        }
                        break;
                    //チーとコマンド
                    case 'q':
                    case 'Q':
                        switch(scene){
                            case 2:
                            estatus.totalHP- = (estatus.HP-1);
                            estatus.HP = 1;
                            break;
                        }
                        break;
                }
            }
            ////////////Playerturn引数短縮用データ★
            playermove move[4] = {
                {&pstatus.up,pstatus.down,&pstatus.ypbutton},//上停止
                {&pstatus.down,pstatus.up,&pstatus.ypbutton},//下停止
                {&pstatus.right,pstatus.left,&pstatus.xpbutton},//右停止
                {&pstatus.left,pstatus.right,&pstatus.xpbutton},//左停止
            };
            if(event->type == HG_KEY_UP&&game == 0){
                switch(scene){
                    case 2:
                        switch(event->ch){
                            //上移動をやめるとき、同時押ししてた場合に方向を変える
                            case HG_U_ARROW:
                                Playerturn(move[0]);
                                break;
                            //下移動をやめるとき、同時押ししてた場合に方向を変える
                            case HG_D_ARROW:
                                Playerturn(move[1]);
                                break;
                            //右移動をやめるとき、同時押ししてた場合に方向を変える
                            case HG_R_ARROW:
                                Playerturn(move[2]);
                                break;
                            //左移動をやめるとき、同時押ししてた場合に方向を変える
                            case HG_L_ARROW:
                                Playerturn(move[3]);
                                break;
                            //ショットキーを離したら弾を撃つのをやめる
                            case 's':
                            case 'S':
                                pstatus.cool = 0;
                                pstatus.shot = 0;
                                break;
                            //低速移動キーを押したら低速移動を解除する
                            case 'z':
                            case 'Z':
                                pstatus.lowspead = 1;
                                break;
                        }
                        break;
                }
            }
        }
        HgSetFillColor(HG_BLACK);
        HgCircleFill(-10,-10,999,1);
        switch(scene){
            //タイトル
            case 0:
                HgWSetColor(ln,HG_WHITE);
                HgWSetFont(ln,HG_HB,105);
                HgWText(ln,130,480,"最終課題");
                //選択枠
                HgWBox(ln,150,315-title*130,400,80);
                HgWSetFont(ln,HG_G,45);
                switch(title){
                    case 0:
                        if(estatus.difficultie != 1){
                            HgWText(ln,570,320,"▶︎");
                        }
                        if(estatus.difficultie != 4){
                            HgWText(ln,90,320,"◀");
                        }
                        HgWText(ln,320,260,"▼");
                        break;
                    case 1:
                        HgWText(ln,320,260,"▲");
                        HgWText(ln,320,130,"▼");
                        break;
                    case 2:
                        HgWText(ln,320,130,"▲");
                        break;
                }
                HgWSetFont(ln,HG_G,35);
                HgWText(ln,520,5,"Sで決定");
                HgWSetFont(ln,HG_G,75);
                //難易度設定
                switch(estatus.difficultie){
                    case 4:
                        HgWText(ln,240,310,"EASY");
                        break;
                    case 3:
                        HgWText(ln,180,310,"NORMAL");
                        break;
                    case 2:
                        HgWText(ln,240,310,"HARD");
                        break;
                    case 1:
                        HgWText(ln,190,310,"LUNATIC");
                        break;
                }
                HgWText(ln,240,180,"説明書");
                HgWText(ln,240,50,"やめる");
                break;
            //説明書
            case 1:
                HgWSetColor(ln,HG_WHITE);
                HgWSetFont(ln,HG_G,40);
                HgWText(ln,30,560,"↑キー:上に移動");
                HgWText(ln,350,560,"↓キー:下に移動");
                HgWText(ln,30,460,"→キー:左に移動");
                HgWText(ln,350,460,"←キー:右に移動");
                HgWText(ln,30,360,"Sキー:弾を発射");
                HgWText(ln,350,360,"Zキー:低速移動");
                HgWText(ln,30,210,"敵のHPを０にすれば勝ち");
                HgWText(ln,30,160,"３回被弾するとゲームオーバー");
                HgWSetFont(ln,HG_G,20);
                HgWText(ln,500,5,"Zでタイトルに戻る");
                break;
            //戦闘画面
            case 2:
                //敵の動きを制御
                if(estatus.movecool <= -1000){
                    /////////////Setenemymove引数短縮用データ★
                    enemymovedata emove[2] = {
                        {&estatus.movex,estatus.movex,estatus.x,&estatus.xcount,movablex+xmargine/2-50,movablex+xmargine/2+50},//x移動
                        {&estatus.movey,estatus.movey,estatus.y,&estatus.ycount,movabley+ymargine-20,movabley+ymargine-80},//y移動
                    };
                    estatus.movex = (0.05-rand()%2/10);
                    estatus.movey = (0.05-rand()%2/10);
                    Setenemymove(emove[0]);//x
                    Setenemymove(emove[1]);//y
                    estatus.movecool = (rand()%5+4)*70;
                }
                HgWSetFillColor(ln,HG_WHITE);
                HgWCircleFill(ln,estatus.x,estatus.y,estatus.hit,10);
                HgWSetFillColor(ln,HG_WHITE);
                HgWCircleFill(ln,pstatus.x,pstatus.y,pstatus.hit+pstatus.nodamage,10);
                HgWImagePut(ln,pstatus.x,pstatus.y,rmbullet,0.1,0);
                /////////////PandEmove引数短縮用データ★
                pemove pandmove[4] = {
                    {&pstatus.x,pstatus.hit,pstatus.xpbutton/pstatus.lowspead,pstatus.minmx,pstatus.maxmx+pstatus.minmx},//プレイヤーx移動
                    {&pstatus.y,pstatus.hit,pstatus.ypbutton/pstatus.lowspead,pstatus.minmy,pstatus.maxmy+pstatus.minmy},//プレイヤーy移動
                    {&estatus.x,estatus.hit,estatus.movex,pstatus.minmx,pstatus.maxmx+pstatus.minmx},//敵x移動
                    {&estatus.y,estatus.hit,estatus.movey,pstatus.minmy,pstatus.maxmy+pstatus.minmy},//敵y移動
                };
                ////////////////自機と敵を動かす
                for(int ii = 30;ii >= 0;ii--){
                    PandEmove(pandmove[0]);
                    PandEmove(pandmove[1]);
                    //敵を動かす
                    if(estatus.movecool>-200){
                        PandEmove(pandmove[2]);
                        PandEmove(pandmove[3]);
                    }
                    /////////////hit引数短縮用データ★
                    hitdata hd[1] = {
                        {estatus.x-pstatus.x,estatus.y-pstatus.y,estatus.hit+pstatus.hit,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[7+7][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                    };
                    //あたり判定（敵本体）
                    hit(hd[0]);
                    estatus.movecool--;
                }
                /////////////Setbullet引数短縮用データ★
                setbullet set[] = {
                    //0~4 自機の弾
                    {&bullet[bcount][0],&bullet[bcount][1],pstatus.x,pstatus.y,&pstatus.nodamage,&bullet[bcount][4],&empty[0]},
                    {&rightsidebullet[bcount][0],&rightsidebullet[bcount][1],pstatus.x+5,pstatus.y,&pstatus.nodamage,&rightsidebullet[bcount][4],&empty[0]},
                    {&leftsidebullet[bcount][0],&leftsidebullet[bcount][1],pstatus.x-5,pstatus.y,&pstatus.nodamage,&leftsidebullet[bcount][4],&empty[0]},
                    {&rightsidebullet2[bcount][0],&rightsidebullet2[bcount][1],pstatus.x+7,pstatus.y-2,&pstatus.nodamage,&rightsidebullet2[bcount][4],&empty[0]},
                    {&leftsidebullet2[bcount][0],&leftsidebullet2[bcount][1],pstatus.x-7,pstatus.y-2,&pstatus.nodamage,&leftsidebullet2[bcount][4],&empty[0]},
                    //5~12
                    {&enemybullet[ebcount][0],&enemybullet[ebcount][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount][5],&enemybullet[ebcount][6]},
                    {&enemybullet[ebcount+1][0],&enemybullet[ebcount+1][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+1][5],&enemybullet[ebcount+1][6]},
                    {&enemybullet[ebcount+2][0],&enemybullet[ebcount+2][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+2][5],&enemybullet[ebcount+2][6]},
                    {&enemybullet[ebcount+3][0],&enemybullet[ebcount+3][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+3][5],&enemybullet[ebcount+3][6]},
                    {&enemybullet[ebcount+4][0],&enemybullet[ebcount+4][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+4][5],&enemybullet[ebcount+4][6]},
                    {&enemybullet[ebcount+5][0],&enemybullet[ebcount+5][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+5][5],&enemybullet[ebcount+5][6]},
                    {&enemybullet[ebcount+6][0],&enemybullet[ebcount+6][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+6][5],&enemybullet[ebcount+6][6]},
                    {&enemybullet[ebcount+7][0],&enemybullet[ebcount+7][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+7][5],&enemybullet[ebcount+7][6]},
                    //13~16 敵の弾
                    {&enemybullet[ebcount][0],&enemybullet[ebcount][1],estatus.x+10,estatus.y-10,&estatus.die,&enemybullet[ebcount][5],&enemybullet[ebcount][6]},
                    {&enemybullet[ebcount+1][0],&enemybullet[ebcount+1][1],estatus.x+10,estatus.y+10,&estatus.die,&enemybullet[ebcount+1][5],&enemybullet[ebcount+1][6]},
                    {&enemybullet2[ebcount][0],&enemybullet2[ebcount][1],estatus.x-10,estatus.y-10,&estatus.die,&enemybullet2[ebcount][5],&enemybullet2[ebcount][6]},
                    {&enemybullet2[ebcount+1][0],&enemybullet2[ebcount+1][1],estatus.x-10,estatus.y+10,&estatus.die,&enemybullet2[ebcount+1][5],&enemybullet2[ebcount+1][6]},
                    //17~22 
                    {&enemybullet[ebcount][0],&enemybullet[ebcount][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount][5],&enemybullet[ebcount][6]},
                    {&enemybullet[ebcount+1][0],&enemybullet[ebcount+1][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+1][5],&enemybullet[ebcount+1][6]},
                    {&enemybullet[ebcount+2][0],&enemybullet[ebcount+2][1],estatus.x,estatus.y,&estatus.die,&enemybullet[ebcount+2][5],&enemybullet[ebcount+2][6]},
                    {&enemybullet2[ebcount][0],&enemybullet2[ebcount][1],estatus.x,estatus.y,&estatus.die,&enemybullet2[ebcount][5],&enemybullet2[ebcount][6]},
                    {&enemybullet2[ebcount+1][0],&enemybullet2[ebcount+1][1],estatus.x,estatus.y,&estatus.die,&enemybullet2[ebcount+1][5],&enemybullet2[ebcount+1][6]},
                    {&enemybullet2[ebcount+2][0],&enemybullet2[ebcount+2][1],estatus.x,estatus.y,&estatus.die,&enemybullet2[ebcount+2][5],&enemybullet2[ebcount+2][6]},
                    //23~25
                    {&enemybullet[ebcount][0],&enemybullet[ebcount][1],-5,pstatus.y,&estatus.die,&enemybullet[ebcount][5],&enemybullet[ebcount][6]},
                    {&enemybullet2[ebcount][0],&enemybullet2[ebcount][1],425,pstatus.y+5,&estatus.die,&enemybullet2[ebcount][5],&enemybullet2[ebcount][6]},
                    {&enemybullet3[ebcount][0],&enemybullet3[ebcount][1],pstatus.x,625,&estatus.die,&enemybullet3[ebcount][5],&enemybullet3[ebcount][6]},
                };
                //////////////////
                //弾を設置する
                //0~4 自機の弾
                if(pstatus.cool == 1){
                    if(pstatus.shot == 1){
                        for(int i = 4;i >= 0;i--){
                            Setbullet(set[i]);
                        }
                        bcount++;
                    }
                }
                //敵の弾
                switch(estatus.wave){
                    //wave1
                    case 0:
                        for(int i = 7;i >= 0;i--){
                            Setbullet(set[i+5]);
                        }
                        ebcount+ = 8;
                        break;
                    //wave2
                    case 1:
                        for(int i = 3;i >= 0;i--){
                            Setbullet(set[i+13]);
                        }
                        ebcount+ = 2;
                        break;
                    //wave3
                    case 2:
                        for(int i = 5;i >= 0;i--){
                            Setbullet(set[i+17]);
                        }
                        ebcount+ = 3;
                        break;
                    //wave4
                    case 3:
                        for(int i = 1;i >= 0;i--){
                            Setbullet(set[i+23]);
                        }
                        if(estatus.difficultie <= 2){
                            Setbullet(set[25]);
                        }
                        ebcount+ = 1;
                        break;
                }
                //配列の要素数以上になったらリセット
                if(bcount>29){
                    bcount = 0;
                }
                if(ebcount>499){
                    ebcount = 0;
                }
                //弾を動かす
                switch(estatus.wave){
                    //wave1
                    case 0:
                        for(int i = 61;i >= 0;i- = estatus.difficultie){
                            for(int ii = 7;ii >= 0;ii--){
                                /////////////Savebulletdata引数短縮用データ★
                                bulletdata data[1] = {
                                    {&enemybullet[i*8+ii][2],cosf(M_PI/360*(ii*90+iii*20))*0.75,&enemybullet[i*8+ii][3],sinf(M_PI/360*(ii*90+iii*20))*0.75,&enemybullet[i*8+ii][4],25},//緑の弾
                                };
                                //////////////Drawbulletdata引数短縮用データ★
                                drawbulletdata draw[1] = {
                                    {enemybullet[i*8+ii][5],ln,enemybullet[i*8+ii][0],enemybullet[i*8+ii][1],grbullet,0.15},
                                };
                                Drawbullet(draw[0]);
                                Savebulletdata(data[0]);
                            }
                            //弾が飛ぶ角度をいじる
                            iii++;
                            if(iii>17){
                                iii = 0;
                            }
                            /////////////Movebullet引数短縮用データ★
                            for(int ii = 15;ii >= 0;ii--){
                                movebulletdata movedata[8] = {
                                    {enemybullet[i*8][0],enemybullet[i*8][1],enemybullet[i*8][2],enemybullet[i*8][3],&enemybullet[i*8][0],&enemybullet[i*8][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8][5]},
                                    {enemybullet[i*8+1][0],enemybullet[i*8+1][1],enemybullet[i*8+1][2],enemybullet[i*8+1][3],&enemybullet[i*8+1][0],&enemybullet[i*8+1][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+1][5]},
                                    {enemybullet[i*8+2][0],enemybullet[i*8+2][1],enemybullet[i*8+2][2],enemybullet[i*8+2][3],&enemybullet[i*8+2][0],&enemybullet[i*8+2][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+2][5]},
                                    {enemybullet[i*8+3][0],enemybullet[i*8+3][1],enemybullet[i*8+3][2],enemybullet[i*8+3][3],&enemybullet[i*8+3][0],&enemybullet[i*8+3][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+3][5]},
                                    {enemybullet[i*8+4][0],enemybullet[i*8+4][1],enemybullet[i*8+4][2],enemybullet[i*8+4][3],&enemybullet[i*8+4][0],&enemybullet[i*8+4][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+4][5]},
                                    {enemybullet[i*8+5][0],enemybullet[i*8+5][1],enemybullet[i*8+5][2],enemybullet[i*8+5][3],&enemybullet[i*8+5][0],&enemybullet[i*8+5][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+5][5]},
                                    {enemybullet[i*8+6][0],enemybullet[i*8+6][1],enemybullet[i*8+6][2],enemybullet[i*8+6][3],&enemybullet[i*8+6][0],&enemybullet[i*8+6][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+6][5]},
                                    {enemybullet[i*8+7][0],enemybullet[i*8+7][1],enemybullet[i*8+7][2],enemybullet[i*8+7][3],&enemybullet[i*8+7][0],&enemybullet[i*8+7][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*8+7][5]},
                                };
                                /////////////hit引数短縮用データ★
                                hitdata hd[8] = {
                                    {enemybullet[i*8][0]-pstatus.x,enemybullet[i*8][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+1][0]-pstatus.x,enemybullet[i*8+1][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+1][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+2][0]-pstatus.x,enemybullet[i*8+2][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+2][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+3][0]-pstatus.x,enemybullet[i*8+3][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+3][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+4][0]-pstatus.x,enemybullet[i*8+4][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+4][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+5][0]-pstatus.x,enemybullet[i*8+5][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+5][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+6][0]-pstatus.x,enemybullet[i*8+6][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+6][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*8+7][0]-pstatus.x,enemybullet[i*8+7][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*8+7][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                };
                                for(int iii = 7;iii >= 0;iii--){
                                    Movebullet(movedata[iii]);
                                    hit(hd[iii]);
                                }
                            }
                        }
                        break;
                    //wave2
                    case 1:
                        for(int i = 249;i >= 0;i- = estatus.difficultie){
                            for(int ii = 1;ii >= 0;ii--){
                                /////////////Savebulletdata引数短縮用データ★
                                bulletdata data[2] = {
                                    {&enemybullet[i*2+ii][2],0.7*cosf(i*18/5+ii),&enemybullet[i*2+ii][3],-0.7*sinf(i*18/5+ii),&enemybullet[i*2+ii][4],25-5*ii},//緑の弾
                                    {&enemybullet2[i*2+ii][2],-0.7*cosf(i*36/5+ii),&enemybullet2[i*2+ii][3],-0.8*sinf(i*36/5+ii),&enemybullet2[i*2+ii][4],20-5*ii},//青の弾
                                };
                                //////////////Drawbulletdata引数短縮用データ★
                                drawbulletdata draw[2] = {
                                    {enemybullet[i*2+ii][5],ln,enemybullet[i*2+ii][0],enemybullet[i*2+ii][1],grbullet,0.15},
                                    {enemybullet2[i*2+ii][5],ln,enemybullet2[i*2+ii][0],enemybullet2[i*2+ii][1],bmbullet,0.3},
                                };
                                Drawbullet(draw[0]);
                                Drawbullet(draw[1]);
                                Savebulletdata(data[0]);
                                Savebulletdata(data[1]);
                            }
                            /////////////Movebullet引数短縮用データ★
                            for(int ii = 25;ii >= 0;ii--){
                                movebulletdata movedata[4] = {
                                    {enemybullet[i*2][0],enemybullet[i*2][1],enemybullet[i*2][2],enemybullet[i*2][3],&enemybullet[i*2][0],&enemybullet[i*2][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*2][5]},
                                    {enemybullet[i*2+1][0],enemybullet[i*2+1][1],enemybullet[i*2+1][2],enemybullet[i*2+1][3],&enemybullet[i*2+1][0],&enemybullet[i*2+1][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*2+1][5]},
                                    {enemybullet2[i*2][0],enemybullet2[i*2][1],enemybullet2[i*2][2],enemybullet2[i*2][3],&enemybullet2[i*2][0],&enemybullet2[i*2][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i*2][5]},
                                    {enemybullet2[i*2+1][0],enemybullet2[i*2+1][1],enemybullet2[i*2+1][2],enemybullet2[i*2+1][3],&enemybullet2[i*2+1][0],&enemybullet2[i*2+1][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i*2+1][5]},
                                };
                                /////////////hit引数短縮用データ★
                                hitdata hd[4] = {
                                    {enemybullet[i*2][0]-pstatus.x,enemybullet[i*2][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*2][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*2+1][0]-pstatus.x,enemybullet[i*2+1][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*2+1][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i*2][0]-pstatus.x,enemybullet2[i*2][1]-pstatus.y,pstatus.hit+8,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i*2][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i*2+1][0]-pstatus.x,enemybullet2[i*2+1][1]-pstatus.y,pstatus.hit+8,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i*2+1][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                };
                                if(30-ii <= enemybullet[i*2][4]){
                                    Movebullet(movedata[0]);
                                    hit(hd[0]);
                                }
                                if(30-ii <= enemybullet[i*2+1][4]){
                                    Movebullet(movedata[1]);
                                    hit(hd[1]);
                                }
                                if(30-ii <= enemybullet2[i*2][4]){
                                    Movebullet(movedata[2]);
                                    hit(hd[2]);
                                }
                                if(30-ii <= enemybullet2[i*2+1][4]){
                                    Movebullet(movedata[3]);
                                    hit(hd[3]);
                                }
                            }
                        }
                        break;
                    //wave3
                    case 2:
                        for(int i = 160;i >= 0;i- = estatus.difficultie){
                            for(int ii = 2;ii >= 0;ii--){
                                /////////////Savebulletdata引数短縮用データ★
                                //////////////Drawbulletdata引数短縮用データ★
                                bulletdata data[2] = {
                                    {&enemybullet[i*3+ii][2],cosf(M_PI/360*(ii*240+iii*5))*0.75,&enemybullet[i*3+ii][3],sinf(M_PI/360*(ii*240+iii*5))*0.75,&enemybullet[i*3+ii][4],25},//緑の弾
                                    {&enemybullet2[i*3+ii][2],cosf(M_PI/360*(ii*240-iii*5))*(-0.75),&enemybullet2[i*3+ii][3],sinf(M_PI/360*(ii*240-iii*5))*(-0.75),&enemybullet2[i*3+ii][4],25},//赤の弾
                                };
                                drawbulletdata draw[2] = {
                                    {enemybullet[i*3+ii][5],ln,enemybullet[i*3+ii][0],enemybullet[i*3+ii][1],grbullet,0.15},
                                    {enemybullet2[i*3+ii][5],ln,enemybullet2[i*3+ii][0],enemybullet2[i*3+ii][1],rmbullet,0.15},
                                };
                                Drawbullet(draw[0]);
                                Drawbullet(draw[1]);
                                Savebulletdata(data[0]);
                                Savebulletdata(data[1]);
                            }
                            //弾が飛ぶ角度をいじる
                            iii++;
                            if(iii>71){
                                iii = 0;
                            }
                            /////////////Movebullet引数短縮用データ★
                            for(int ii = 15;ii >= 0;ii--){
                                movebulletdata movedata[6] = {
                                    {enemybullet[i*3][0],enemybullet[i*3][1],enemybullet[i*3][2],enemybullet[i*3][3],&enemybullet[i*3][0],&enemybullet[i*3][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*3][5]},
                                    {enemybullet[i*3+1][0],enemybullet[i*3+1][1],enemybullet[i*3+1][2],enemybullet[i*3+1][3],&enemybullet[i*3+1][0],&enemybullet[i*3+1][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*3+1][5]},
                                    {enemybullet[i*3+2][0],enemybullet[i*3+2][1],enemybullet[i*3+2][2],enemybullet[i*3+2][3],&enemybullet[i*3+2][0],&enemybullet[i*3+2][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i*3+2][5]},
                                    {enemybullet2[i*3][0],enemybullet2[i*3][1],enemybullet2[i*3][2],enemybullet2[i*3][3],&enemybullet2[i*3][0],&enemybullet2[i*3][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i*3][5]},
                                    {enemybullet2[i*3+1][0],enemybullet2[i*3+1][1],enemybullet2[i*3+1][2],enemybullet2[i*3+1][3],&enemybullet2[i*3+1][0],&enemybullet2[i*3+1][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i*3+1][5]},
                                    {enemybullet2[i*3+2][0],enemybullet2[i*3+2][1],enemybullet2[i*3+2][2],enemybullet2[i*3+2][3],&enemybullet2[i*3+2][0],&enemybullet2[i*3+2][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i*3+2][5]},
                                };
                                /////////////hit引数短縮用データ★
                                hitdata hd[6] = {
                                    {enemybullet[i*3][0]-pstatus.x,enemybullet[i*3][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*3][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*3+1][0]-pstatus.x,enemybullet[i*3+1][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*3+1][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet[i*3+2][0]-pstatus.x,enemybullet[i*3+2][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i*3+2][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i*3][0]-pstatus.x,enemybullet2[i*3][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i*3][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i*3+1][0]-pstatus.x,enemybullet2[i*3+1][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i*3+1][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i*3+2][0]-pstatus.x,enemybullet2[i*3+2][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i*3+2][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                };
                                for(int iii = 5;iii >= 0;iii--){
                                    Movebullet(movedata[iii]);
                                    hit(hd[iii]);
                                }
                            }
                        }
                        break;
                    case 3:
                        for(int i = 499;i >= 0;i- = 6+estatus.difficultie*2){
                            for(int ii = 1;ii >= 0;ii--){
                                /////////////Savebulletdata引数短縮用データ★
                                //////////////Drawbulletdata引数短縮用データ★
                                bulletdata data[3] = {
                                    {&enemybullet[i][2],0.25,&enemybullet[i][3],0,&enemybullet[i][4],25},//緑の弾
                                    {&enemybullet2[i][2],-0.25,&enemybullet2[i][3],0,&enemybullet2[i][4],25},//赤の弾
                                    {&enemybullet3[i][2],0,&enemybullet3[i][3],-0.25,&enemybullet3[i][4],25},//青の弾
                                };
                                drawbulletdata draw[3] = {
                                    {enemybullet[i][5],ln,enemybullet[i][0],enemybullet[i][1],grbullet,0.15},
                                    {enemybullet2[i][5],ln,enemybullet2[i][0],enemybullet2[i][1],rmbullet,0.15},
                                    {enemybullet3[i][5],ln,enemybullet3[i][0],enemybullet3[i][1],bmbullet,0.2},
                                };
                                Drawbullet(draw[0]);
                                Drawbullet(draw[1]);
                                Drawbullet(draw[2]);
                                Savebulletdata(data[0]);
                                Savebulletdata(data[1]);
                                Savebulletdata(data[2]);
                            }
                            /////////////Movebullet引数短縮用データ★
                            for(int ii = 6;ii >= 0;ii--){
                                movebulletdata movedata[3] = {
                                    {enemybullet[i][0],enemybullet[i][1],enemybullet[i][2],enemybullet[i][3],&enemybullet[i][0],&enemybullet[i][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet[i][5]},
                                    {enemybullet2[i][0],enemybullet2[i][1],enemybullet2[i][2],enemybullet2[i][3],&enemybullet2[i][0],&enemybullet2[i][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet2[i][5]},
                                    {enemybullet3[i][0],enemybullet3[i][1],enemybullet3[i][2],enemybullet3[i][3],&enemybullet3[i][0],&enemybullet3[i][1],movablex+xmargine,xmargine,movabley+ymargine,ymargine,&enemybullet3[i][5]},
                                };
                                /////////////hit引数短縮用データ★
                                hitdata hd[3] = {
                                    {enemybullet[i][0]-pstatus.x,enemybullet[i][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet2[i][0]-pstatus.x,enemybullet2[i][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet2[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                    {enemybullet3[i][0]-pstatus.x,enemybullet3[i][1]-pstatus.y,pstatus.hit+2,&pstatus.x,&pstatus.y,pstatus.fx,pstatus.fy,0,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet3[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                                };
                                for(int iii = 2;iii >= 0;iii--){
                                    Movebullet(movedata[iii]);
                                    hit(hd[iii]);
                                }
                            }
                        }
                        break;
                }
                for(int i = 0;i<30;i++){
                    //////////////Drawbulletdata引数短縮用データ★
                    drawbulletdata draw[5] = {
                        {bullet[i][4],ln,bullet[i][0],bullet[i][1],bsbullet,0.03},
                        {rightsidebullet[i][4],ln,rightsidebullet[i][0],rightsidebullet[i][1],bsbullet,0.03},
                        {leftsidebullet[i][4],ln,leftsidebullet[i][0],leftsidebullet[i][1],bsbullet,0.03},
                        {rightsidebullet2[i][4],ln,rightsidebullet2[i][0],rightsidebullet2[i][1],bsbullet,0.03},
                        {leftsidebullet2[i][4],ln,leftsidebullet2[i][0],leftsidebullet2[i][1],bsbullet,0.03},
                    };
                    Drawbullet(draw[0]);
                    Drawbullet(draw[1]);
                    Drawbullet(draw[2]);
                    Drawbullet(draw[3]);
                    Drawbullet(draw[4]);
                    ////自機の弾を動かす
                    for(int ii = 0;ii<30;ii++){
                        /////////////Movebullet引数短縮用データ★
                        movebulletdata movedata[10] = {
                            //0~4 高速移動時の自機の弾
                            {bullet[i][0],bullet[i][1],0.5*cosf(M_PI/360*180),0.5*sinf(M_PI/360*180),&bullet[i][0],&bullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&bullet[i][4]},
                            {rightsidebullet[i][0],rightsidebullet[i][1],0.5*cosf(M_PI/360*170),0.5*sinf(M_PI/360*170),&rightsidebullet[i][0],&rightsidebullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&rightsidebullet[i][4]},
                            {leftsidebullet[i][0],leftsidebullet[i][1],0.5*cosf(M_PI/360*190),0.5*sinf(M_PI/360*190),&leftsidebullet[i][0],&leftsidebullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&leftsidebullet[i][4]},
                            {rightsidebullet2[i][0],rightsidebullet2[i][1],0.5*cosf(M_PI/360*220),0.5*sinf(M_PI/360*220),&rightsidebullet2[i][0],&rightsidebullet2[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&rightsidebullet2[i][4]},
                            {leftsidebullet2[i][0],leftsidebullet2[i][1],0.5*cosf(M_PI/360*140),0.5*sinf(M_PI/360*140),&leftsidebullet2[i][0],&leftsidebullet2[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&leftsidebullet2[i][4]},
                            //5~9 低速移動時の自機の弾
                            {bullet[i][0],bullet[i][1],0,0.5,&bullet[i][0],&bullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&bullet[i][4]},
                            {rightsidebullet[i][0],rightsidebullet[i][1],0,0.5,&rightsidebullet[i][0],&rightsidebullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&rightsidebullet[i][4]},
                            {leftsidebullet[i][0],leftsidebullet[i][1],0,0.5,&leftsidebullet[i][0],&leftsidebullet[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&leftsidebullet[i][4]},
                            {rightsidebullet2[i][0],rightsidebullet2[i][1],0,0.5,&rightsidebullet2[i][0],&rightsidebullet2[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&rightsidebullet2[i][4]},
                            {leftsidebullet2[i][0],leftsidebullet2[i][1],0,0.5,&leftsidebullet2[i][0],&leftsidebullet2[i][1],movablex+xmargine+10,-xmargine,movabley+ymargine+10,-ymargine,&leftsidebullet2[i][4]},
                        };
                        /////////////hit引数短縮用データ★
                        hitdata hd[5] = {
                            {estatus.x-bullet[i][0],estatus.y-bullet[i][1],10+3,&bullet[i][0],&bullet[i][1],5000,5000,1,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                            {estatus.x-rightsidebullet[i][0],estatus.y-rightsidebullet[i][1],10+3,&rightsidebullet[i][0],&rightsidebullet[i][1],5000,5000,1,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                            {estatus.x-leftsidebullet[i][0],estatus.y-leftsidebullet[i][1],10+3,&leftsidebullet[i][0],&leftsidebullet[i][1],5000,5000,1,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                            {estatus.x-rightsidebullet2[i][0],estatus.y-rightsidebullet2[i][1],10+3,&rightsidebullet2[i][0],&rightsidebullet2[i][1],5000,5000,1,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                            {estatus.x-leftsidebullet2[i][0],estatus.y-leftsidebullet2[i][1],10+3,&leftsidebullet2[i][0],&leftsidebullet2[i][1],5000,5000,1,&estatus.HP,&estatus.totalHP,&pstatus.score,&enemyHP[estatus.wave+1],&estatus.wave,&estatus.wavelimit,&enemybullet[i][6],&pstatus.graze,&estatus.nodamage,&pstatus.life,&scene,&pstatus.nodamage},
                        };
                        switch(pstatus.lowspead){
                            case 1:
                                for(int ii = 4;ii >= 0;ii--){
                                    Movebullet(movedata[ii]);
                                }
                                break;
                            case 2:
                                for(int ii = 4;ii >= 0;ii--){
                                    Movebullet(movedata[5+ii]);
                                }
                                break;
                        }
                        //ヒット判定
                        for(int ii = 4;ii >= 0;ii--){
                            hit(hd[ii]);
                        }
                        //倒した時に弾を全て消し、データを初期化する
                        if((estatus.HP <= 0&&estatus.wave >= estatus.wavelimit)||pstatus.life<0){
                            for(int i = 499;i >= 0;i--){
                                for(int ii = 1;ii >= 0;ii--){
                                    enemybullet[i][5+ii] = 0;
                                    enemybullet2[i][5+ii] = 0;
                                    enemybullet3[i][5+ii] = 0;
                                }
                                for(int ii = 2;ii >= 0;ii--){
                                    enemybullet[i][2+ii] = 0;
                                    enemybullet2[i][2+ii] = 0;
                                    enemybullet3[i][2+ii] = 0;
                                    bullet[i][2+ii] = 0;
                                    rightsidebullet[i][2+ii] = 0;
                                    leftsidebullet[i][2+ii] = 0;
                                    rightsidebullet2[i][2+ii] = 0;
                                    leftsidebullet2[i][2+ii] = 0;
                                }
                                for(int ii = 1;ii >= 0;ii--){
                                    enemybullet[i][ii] = 3000;
                                    enemybullet2[i][ii] = 3000;
                                    enemybullet3[i][ii] = 3000;
                                    bullet[i][ii] = 3000;
                                    rightsidebullet[i][ii] = 3000;
                                    leftsidebullet[i][ii] = 3000;
                                    rightsidebullet2[i][ii] = 3000;
                                    leftsidebullet2[i][ii] = 3000;
                                }
                            }
                            scene = 3;
                            estatus.HP = 1;
                        }
                    }
                }
                break;
            case 3:
                HgWImagePut(ln,pstatus.x,pstatus.y,rmbullet,0.1,0);
                //クリアできたら付与したボーナスと最終スコアを表示
                if(pstatus.life >= 0){
                    HgWSetFont(ln,HG_G,25);
                    HgWText(ln,80,530,"クリアボーナス:+%d",2500*(5-estatus.difficultie));
                    HgWText(ln,80,480,"残機ボーナス:+%d",5000*(pstatus.life+1));
                    HgWText(ln,80,430,"難易度ボーナス:×%.1f",0.5*(5-estatus.difficultie));
                    HgWSetFont(ln,HG_G,30);
                    HgWText(ln,80,350,"最終スコア:%d",pstatus.score);
                }
                //失敗したらゲームオーバーと表示
                if(pstatus.life<0){
                    HgWSetFont(ln,HG_G,45);
                    HgWText(ln,85,450,"GAME OVER");
                }
                HgWSetFont(ln,HG_G,15);
                HgWText(ln,30,20,"Eでタイトルに戻る");
                //ボーナス付与
                if(pstatus.bonus == 0){
                    pstatus.score+ = (2500*(5-estatus.difficultie)+5000*(pstatus.life+1));//クリアボーナス＆残機ボーナス
                    pstatus.score = pstatus.score*(0.5*(5-estatus.difficultie));//難易度ボーナス
                    //ゲームオーバー
                    if(pstatus.life<0){
                        pstatus.score = 0;
                    }
                    pstatus.bonus = 1;
                }
                break;
        }
        //背景などの描写
        if(scene == 2||scene == 3){
            pstatus.scoreback = pstatus.score;
            pstatus.grazeback = pstatus.graze;
            HgWSetColor(ln,HG_WHITE);
            //戦闘中の背景
            HgWSetFillColor(ln,HG_BLACK);
            HgWBoxFill(ln,0,0,20,640,0);
            HgWBoxFill(ln,20,0,680,20,0);
            HgWBoxFill(ln,420,20,280,620,0);
            HgWBoxFill(ln,20,620,400,20,0);
            HgWBox(ln,20,20,400,600);
            HgWSetFont(ln,HG_HB,30);
            //難易度、スコア、グレイズ表示
            switch(estatus.difficultie){
                case 1:
                    HgWSetColor(ln,HG_RED);
                    HgWText(ln,480,580,"LUNATIC");
                    break;
                case 2:
                    HgWSetColor(ln,HG_BLUE);
                    HgWText(ln,500,580,"HARD");
                    break;
                case 3:
                    HgWSetColor(ln,HG_YELLOW);
                    HgWText(ln,480,580,"NORMAL");
                    break;
                case 4:
                    HgWSetColor(ln,HG_GREEN);
                    HgWText(ln,500,580,"EASY");
                    break;
            }
            HgWSetColor(ln,HG_WHITE);
            HgWSetFont(ln,HG_G,25);
            HgWText(ln,440,530,"score:");
            for(int ii = 7;ii >= 0;ii--){
                score[ii] = pstatus.scoreback%10;
                pstatus.scoreback = pstatus.scoreback/10;
                HgWText(ln,520+ii*15,530,"%d",score[ii]);
            }
            HgWText(ln,440,480,"graze:");
            for(int ii = 7;ii >= 0;ii--){
                graze[ii] = pstatus.grazeback%10;
                pstatus.grazeback = pstatus.grazeback/10;
                HgWText(ln,520+ii*15,480,"%d",graze[ii]);
            }
            HgWText(ln,440,370,"life");
            for(int ii = pstatus.life;ii >= 0;ii--){
                HgWImagePut(ln,460+ii*40,350,lifemark,0.04,0);
            }
            //体力ゲージ
            HgWText(ln,440,270,"enemy");
            switch(estatus.wave){
                case 0:
                    HgWSetFillColor(ln,HG_GREEN);
                    break;
                case 1:
                    HgWSetFillColor(ln,HG_GREEN);
                    break;
                case 2:
                    HgWSetFillColor(ln,HG_YELLOW);
                    break;
                case 3:
                    HgWSetFillColor(ln,HG_RED);
                    break;
            }
            if(estatus.totalHP>0){
                HgWBoxFill(ln,440,240,estatus.totalHP/27,20,0);
            }
            HgWSetFont(ln,HG_HB,75);
            HgWText(ln,550,120,"最");
            HgWText(ln,550,50,"終");
            HgWText(ln,480,90,"課");
            HgWText(ln,480,20,"題");
            HgWCircle(ln,553,115,105);
        }
        //waveごとの体力を削りきった時、次の攻撃に移行する準備
        if(estatus.nodamage>0){
            ebcount = 0;
            iii = 0;
            estatus.die = 1;
            estatus.nodamage--;
            if(estatus.wave != estatus.wavelimit&&estatus.nodamage == 0){
                estatus.die = 0;
            }
            for(int i = 499;i >= 0;i--){
                for(int ii = 1;ii >= 0;ii--){
                    enemybullet[i][ii] = 3000;
                    enemybullet2[i][ii] = 3000;
                    enemybullet3[i][ii] = 3000;
                }
                for(int ii = 4;ii >= 0;ii--){
                    enemybullet[i][2+ii] = 0;
                    enemybullet2[i][2+ii] = 0;
                    enemybullet3[i][2+ii] = 0;
                }
            }
        }
        //無敵時間を減らす
        if(pstatus.nodamage>0){
            pstatus.nodamage--;
        }
        //弾のクールタイム
        if(pstatus.cool != 0){
            pstatus.cool++;
        }
        if(pstatus.cool >= 3){
            pstatus.cool = 1;
        }
        HgSleep(0.00001);
    }
    HgClose();
    return 0;
}