//========================
//�v���C���[�w�b�^�[
//========================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"
#include"disc.h"

#define EFFECTRMOVE (50)  //���[�v�̕�
#define MAXEFFECTR (2)//�o��ő吔
#define MAX_EFFECT (1280)//Effect�̍ő吔
typedef enum
{
	EFFECTSTATE_NORMAL = 0,
	EFFECTSTATE_SHOOT,		//�ʏ�ATTACK
	EFFECTSTATE_GOAL,		//����
	EFFECTSTATE_MAXGAGE,	//�Z�Q�[�WMAX
	EFFECTSTATE_JUMP,	//�W�����v
	EFFECTSTATE_RUNNOW,		//����
	EFFECTSTATE_SPIN,		//��]
	EFFECTSTATE_MAX

}EFFECTSTATE;

typedef struct
{
	int nCnt;
	int nPatternX;
	int nPatternY;
	int nDivisionX;
	int nDivisionY;
	int nSpeed;
}ANIM_TEX;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		 //�J���[
	int nLife;			//����
	bool bUse;			//�g�p���Ă邩�ǂ���
	float fRadeius;		//���a
	EFFECTSTATE nType;			//�^�C�v
	int nMaxLife;		//���C�t�̍ő�
	ANIM_TEX AnimTex;
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTSTATE nType,int life, float size);
Effect* GetEffect(void);

void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
#endif

