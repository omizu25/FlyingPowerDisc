//========================================================
//
//UI����
//Author:����]��
//
//========================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//====================================
//UI�\���̂̒�`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Ui;
//====================================
//�v���g�^�C�v�錾
//====================================
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void DeleteUi(int nCntUi);
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
Ui * GetUi(void);

#endif