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
bool CollisionWall(VERTEX_2D *pVtx, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
void MoveWall(void);
//Wall* GetWall(void);
#endif

