//========================
//プレイヤーヘッター
//========================
#ifndef _PLAYE_H_
#define _PLAYE_H_

#include"main.h"
#include"disc.h"

#define PLAYERMOVE (50)  //ループの幅
#define PLAYERSIZE_X (100)//サイズ
#define PLAYERSIZE_Y (100)//サイズ
#define MAXPLAYER (2)//登場最大数

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


typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 move;			//前回の位置
	D3DXVECTOR3 rot;			// 向き
	PLAYESTATE state;			//状態
	int nLife;					//寿命
	int nType;					//タイプ
	int nIdx;					//矩形のインデックス
	int nSkillCount;
	int nDiveCount;				//タックルの硬直時間
	int nHaveCount;				//もってる時間
	float fheight;				//高さ
	float fwidth;				//幅
	float Pow;					//パワー
	float Speed;				//スピード
	bool bUse;					//使用してるかどうか
	bool bHave;					//持ってるかどうか
	bool bDive;					//タックル判定
	bool bSkill;					//必殺技使えるか使えないか
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos, int nType, bool light);
bool CollisionPlayer(Disc *pDisc, float Size,int number);

void MovePlayer(void);
void LoadFile(char *Filename);
Player* GetPlayer(void);
#endif

