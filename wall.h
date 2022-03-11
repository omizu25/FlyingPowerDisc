//========================
//プレイヤーヘッター
//========================
#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"

#define WALLMOVE (50)  //ループの幅
#define WALLSIZ_X (100)//サイズ
#define WALLSIZ_Y (100)//サイズ

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *move);
bool CalcParticlePlaneAfterPos(
	D3DXVECTOR3 *pColliPos,
	D3DXVECTOR3 *pVelo,
	FLOAT res,
	FLOAT time,
	D3DXVECTOR3 *pOut_pos,
	D3DXVECTOR3 *pOut_velo
);
void MoveWall(void);
//Wall* GetWall(void);
#endif

