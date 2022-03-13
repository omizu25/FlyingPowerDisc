//========================
//�v���C���[�w�b�^�[
//========================
#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"

#define WALLMOVE (50)  //���[�v�̕�
#define WALLSIZ_X (100)//�T�C�Y
#define WALLSIZ_Y (100)//�T�C�Y
#define MAXWALL			(2)//�o��ő吔
#define MAXWALLTYPE		(4)//Type�ő吔
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

//�v���g�^�C�v�錾
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

