//========================
//�v���C���[�w�b�^�[
//========================
#ifndef _PLAYE_H_
#define _PLAYE_H_

#include"main.h"
#include"disc.h"

#define PLAYERMOVE (50)  //���[�v�̕�
#define PLAYERSIZE_X (100)//�T�C�Y
#define PLAYERSIZE_Y (100)//�T�C�Y
#define MAXPLAYER (2)//�o��ő吔

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


typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 move;			//�O��̈ʒu
	D3DXVECTOR3 rot;			// ����
	PLAYESTATE state;			//���
	int nLife;					//����
	int nType;					//�^�C�v
	int nIdx;					//��`�̃C���f�b�N�X
	int nSkillCount;
	int nDiveCount;				//�^�b�N���̍d������
	int nHaveCount;				//�����Ă鎞��
	float fheight;				//����
	float fwidth;				//��
	float Pow;					//�p���[
	float Speed;				//�X�s�[�h
	bool bUse;					//�g�p���Ă邩�ǂ���
	bool bHave;					//�����Ă邩�ǂ���
	bool bDive;					//�^�b�N������
	bool bSkill;					//�K�E�Z�g���邩�g���Ȃ���
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos, int nType, bool light);
bool CollisionPlayer(Disc *pDisc, float Size,int number);

void MovePlayer(void);
void LoadFile(char *Filename);
Player* GetPlayer(void);
#endif

