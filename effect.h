//========================
//プレイヤーヘッター
//========================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"
#include"disc.h"

#define MAXEFFECTR (2)//登場最大数
#define MAX_EFFECT (128)//Effectの最大数
typedef enum
{
	EFFECTSTATE_SHOOT = 0,	//炎
	EFFECTSTATE_SPIN,		//ディスク
	EFFECTSTATE_MAX

}EFFECTSTATE;

typedef struct
{
	int nCnt;
	int nPatternX;
	int nPatternY;
	int nDivisionX;
	int nDivisionY;
	int nSpeed;
}ANIM_TEX;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		 //カラー
	int nLife;			//寿命
	bool bUse;			//使用してるかどうか
	float fRadeius;		//半径
	EFFECTSTATE nType;			//タイプ
	int nMaxLife;		//ライフの最大
	int nIdx;			// 矩形のインデックス
	ANIM_TEX AnimTex;
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTSTATE nType,int life, float size);


void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
#endif

