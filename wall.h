//========================
//プレイヤーヘッター
//========================
#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"

#define WALLMOVE (50)  //ループの幅
#define WALLSIZ_X (100)//サイズ
#define WALLSIZ_Y (100)//サイズ
#define MAXWALL			(2)//登場最大数
#define MAXWALLTYPE		(4)//Type最大数
#define MOVESPEED		(5.0f)
#define WALLWIDE		(80.0f)

typedef enum
{
	WALL_UP = 0,
	WALL_DOWN,
	WALL_LEFT,
	WALL_LIGHT,
	WALL_MAX
}WALL;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
//bool CalcParticlePlaneAfterPos(
//	D3DXVECTOR3 *pColliPos,
//	D3DXVECTOR3 *pVelo,
//	FLOAT res,
//	FLOAT time,
//	D3DXVECTOR3 *pOut_pos,
//	D3DXVECTOR3 *pOut_velo
//);
//void MoveWall(void);
//Wall* GetWall(void);
#endif

