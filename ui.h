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
	D3DXVECTOR3 scale;	//�g�嗦
	float nCntTime;		//�o������
	float fWidth;		//��
	float fHeight;		//����
	int nType;			//���
	int nTexture;		//�e�N�X�`���ԍ�
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bSwitch;		//�o�Ă��邩�����Ă�����
}Ui;
//====================================
//�v���g�^�C�v�錾
//====================================
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, D3DXVECTOR3 scale, int nTexture);
Ui * GetUi(void);

#endif