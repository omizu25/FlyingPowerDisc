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
bool CollisionWall(VERTEX_2D *pVtx, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
void MoveWall(void);
//Wall* GetWall(void);
#endif

