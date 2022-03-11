//================================
//
// ���[���I����ʂ̃w�b�_�[
// Author:Sato Teruto
//
//================================
#ifndef _RULE_H_	//���̃}�N������`����ĂȂ�������
#define _RULE_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//�C���N���[�h
#include "main.h"

//�X�C�b�`�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩
	int	nIdx;			// ��`�̃C���f�b�N�X
}Switch;

//���[���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩
	int	nIdx;			// ��`�̃C���f�b�N�X
}Rule;

//�v���g�^�C�v�錾
void InitRule(void);
void UninitRule(void);
void UpdateRule(void);
void DrawRule(void);
void SetRule(D3DXVECTOR3 pos);
void SetSwitchLeft(D3DXVECTOR3 pos);
void SetSwitchRight(D3DXVECTOR3 pos);
void FlashTexture(int nNumber);
void AddRule(int nNumber);
void SubRule(int nNumber);
int ChangeSelect(void);

#endif