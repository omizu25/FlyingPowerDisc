//================================
//
// ���[���I����ʂ̃w�b�_�[
// Author:Sato Teruto
//
//================================
#ifndef _MAP_H_	//���̃}�N������`����ĂȂ�������
#define _MAP_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//�C���N���[�h
#include "main.h"

//�w�i�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩
	int	nIdx;			// ��`�̃C���f�b�N�X
}MAPBG;

//���[���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩
	int	nIdx;			// ��`�̃C���f�b�N�X
}Map;

//�v���g�^�C�v�錾
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos);
void SetBGMap(D3DXVECTOR3 pos);
void FlashTextureMap(int nNumber);
int ChangeSelectMap(void);
int GetSelectMap(void);
#endif
