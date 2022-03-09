//========================
//�v���C���[�w�b�^�[
//========================
#ifndef _PLAYE_H_
#define _PLAYE_H_

#include"main.h"

#define PLAYERMOVE (50)  //���[�v�̕�
#define PLAYERSIZ_X (100)//�T�C�Y
#define PLAYERSIZ_Y (100)//�T�C�Y
#define NUM_PL (1)

typedef enum
{
	PLAYESTATE_NORMAL = 0,
	PLAYESTATE_DAMAGE,		//�_���[�W
	PLAYESTATE_ATTACK,		//�A�^�b�N
	PLAYESTATE_JUMPUP,		//�t���C
	PLAYESTATE_JUMPDOWN,	//�W�����v
	PLAYESTATE_RUNNOW,		//����
	PLAYESTATE_ROTATE,		//��]
	PLAYESTATE_MAX

}PLAYESTATE;
typedef enum
{
	PLAYEROTSTATE_NORMAL = 0,
	PLAYEROTSTATE_ROTATE,		//��]
	PLAYEROTSTATE_MAX

}PLAYEROTSTATE;
typedef enum
{
	JUMP_NORMAL = 0,
	JUMP_NOU,
	JUMP_MAX,

}JUMP;
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 move;			//�O��̈ʒu
	PLAYESTATE state;			//���
	PLAYEROTSTATE rotstate;		//���
	int Speed;					//���x
	int nLife;					//����
	int nType;					//�^�C�v
	float fheight;				//����
	float fwidth;				//��
	float MapMove;              //�}�b�v���ǂꂾ����������
	bool bUse;					//�g�p���Ă邩�ǂ���
}Player;
//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetUp(VERTEX_2D *pVtx,
	float posx,		//���S�_��
	float posy,		//���S�_��
	float rotPlayer,		//�v���C���[��]
	float fAngleAttack 		//�Ίp���p�x
	, float fLengthAttack);	//�Ίp���m����
void LoadFile(char *Filename);
#endif

