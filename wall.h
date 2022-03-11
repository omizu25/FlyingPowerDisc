//========================
//�v���C���[�w�b�^�[
//========================
#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"

#define WALLMOVE (50)  //���[�v�̕�
#define WALLSIZ_X (100)//�T�C�Y
#define WALLSIZ_Y (100)//�T�C�Y

//�v���g�^�C�v�錾
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

