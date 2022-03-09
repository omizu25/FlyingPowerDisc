//==================================================
// 
// FPG���� ( rectangle.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _RECTANGLE_H_		//���̃}�N����`������ĂȂ�������
#define _RECTANGLE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitRectAngle(void);
void UninitRectAngle(void);
void DrawRectAngle(void);
void SetRectAngle(LPDIRECT3DTEXTURE9 *pTexture, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff, const int nBuffLength);
void StopUseRectAngle(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff);

#endif // !_RECTANGLE_H_
