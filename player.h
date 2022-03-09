//========================
//プレイヤーヘッター
//========================
#ifndef _PLAYE_H_
#define _PLAYE_H_

#include"main.h"

#define PLAYERMOVE (50)  //ループの幅
#define PLAYERSIZ_X (100)//サイズ
#define PLAYERSIZ_Y (100)//サイズ
#define NUM_PL (1)

typedef enum
{
	PLAYESTATE_NORMAL = 0,
	PLAYESTATE_DAMAGE,		//ダメージ
	PLAYESTATE_ATTACK,		//アタック
	PLAYESTATE_JUMPUP,		//フライ
	PLAYESTATE_JUMPDOWN,	//ジャンプ
	PLAYESTATE_RUNNOW,		//走れ
	PLAYESTATE_ROTATE,		//回転
	PLAYESTATE_MAX

}PLAYESTATE;
typedef enum
{
	PLAYEROTSTATE_NORMAL = 0,
	PLAYEROTSTATE_ROTATE,		//回転
	PLAYEROTSTATE_MAX

}PLAYEROTSTATE;
typedef enum
{
	JUMP_NORMAL = 0,
	JUMP_NOU,
	JUMP_MAX,

}JUMP;
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 move;			//前回の位置
	PLAYESTATE state;			//状態
	PLAYEROTSTATE rotstate;		//状態
	int Speed;					//速度
	int nLife;					//寿命
	int nType;					//タイプ
	float fheight;				//高さ
	float fwidth;				//幅
	float MapMove;              //マップをどれだけ走ったか
	bool bUse;					//使用してるかどうか
}Player;
//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetUp(VERTEX_2D *pVtx,
	float posx,		//中心点ｘ
	float posy,		//中心点ｙ
	float rotPlayer,		//プレイヤー回転
	float fAngleAttack 		//対角線角度
	, float fLengthAttack);	//対角線ノ長さ
void LoadFile(char *Filename);
#endif

