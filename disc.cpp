//**************************************************
//
// FPD���� ( disc.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "disc.h"
#include "rectangle.h"
#include "color.h"
#include "game.h"
#include "player.h"
#include "texture.h"

#include <assert.h>

//==================================================
// �}�N����`
//==================================================
#define DISC_SIZE			(75.0f)										// �f�B�X�N�̃T�C�Y
#define DISC_ROT_SPEED		(-0.25f)									// �f�B�X�N�̉�]���x
#define MAX_MOVE			(5.0f)										// �ړ��ʂ̍ő�l
#define START_POS_X			(SCREEN_WIDTH * 0.5f)						// �f�B�X�N�̎n�܂��X�̈ʒu
#define START_POS_Y			(SCREEN_HEIGHT - (DISC_SIZE * 0.5f))		// �f�B�X�N�̎n�܂��Y�̈ʒu

//==================================================
// �\����
//==================================================
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXVECTOR3		move;			// �ړ���
	int				nIdx;			// ��`�̃C���f�b�N�X
	float			fSize;			// �T�C�Y
	bool			bUse;			// �g�p���Ă邩�ǂ���
}Disc;

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static Disc		s_disc;				// �f�B�X�N�̏��
static int		s_nPossPlayer;		// ���̃f�B�X�N�̎n�܂�̃v���C���[

//==================================================
// �v���g�^�C�v�錾
//==================================================
static void UpdateStart(void);
static void UpdateReset(void);
static void UpdateNormal(void);
static void Reflect(void);
static void NormalizeAngle(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitDisc(void)
{
	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.fSize = DISC_SIZE * 0.5f;

	// ��`�̐ݒ�
	s_disc.nIdx = SetRectangle(GetTexture(TEXTURE_Disc));

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitDisc(void)
{
	// �g���̂��~�߂�
	StopUseRectangle(s_disc.nIdx);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateDisc(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_START:		// �J�n���
		UpdateStart();
		break;

	case GAMESTATE_NORMAL:		// �ʏ���
		UpdateNormal();
		break;

	case GAMESTART_RESET:		// ���Z�b�g���
		UpdateReset();
		break;

	case GAMESTATE_END:			// �I�����
	case GAMESTATE_RESULT:		// ���U���g���

		break;

	case GAMESTATE_NONE:		// �������Ă��Ȃ����
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawDisc(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}

//--------------------------------------------------
// �J�n���
//--------------------------------------------------
static void UpdateStart(void)
{
	Player *pPlayer = GetPlayer();

	pPlayer += (s_nPossPlayer * 1);

	float fRotMove, fRotDest, fRotDiff;

	//���݂̈ړ�����(�p�x)
	fRotMove = atan2f(s_disc.move.x, s_disc.move.y);

	//�ړI�̈ړ�����(�p�x)
	fRotDest = atan2f(pPlayer->pos.x - s_disc.pos.x, pPlayer->pos.y - s_disc.pos.y);

	fRotDiff = fRotDest - fRotMove;		//�ړI�̈ړ������܂ł̍���

	if (fRotDiff >= D3DX_PI)
	{
		fRotDiff = fRotDiff - (D3DX_PI * 2.0f);
	}
	else if (fRotDiff <= -D3DX_PI)
	{
		fRotDiff = fRotDiff + (D3DX_PI * 2.0f);
	}

	fRotMove += fRotDiff;		//�ړ�����(�p�x)�̕␳

	if (fRotMove >= D3DX_PI)
	{
		fRotMove = fRotMove - (D3DX_PI * 2.0f);
	}
	else if (fRotMove <= -D3DX_PI)
	{
		fRotMove = fRotMove + (D3DX_PI * 2.0f);
	}

	s_disc.move.x = sinf(fRotMove) * MAX_MOVE;
	s_disc.move.y = cosf(fRotMove) * MAX_MOVE;
}

//--------------------------------------------------
// �ʏ���
//--------------------------------------------------
static void UpdateNormal(void)
{
	// ��]
	s_disc.rot.z += DISC_ROT_SPEED;

	// �p�x�̐��K��
	NormalizeAngle();

	// �ʒu�̍X�V
	s_disc.pos += s_disc.move;

	// ����
	Reflect();

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// ���Z�b�g���
//--------------------------------------------------
static void UpdateReset(void)
{
	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Reflect(void)
{
	if (s_disc.pos.y >= SCREEN_HEIGHT - s_disc.fSize)
	{// ��
		s_disc.pos.y = SCREEN_HEIGHT - s_disc.fSize;
		s_disc.move.y *= -1.0f;
	}
	else if (s_disc.pos.y <= s_disc.fSize)
	{// ��
		s_disc.pos.y = s_disc.fSize;
		s_disc.move.y *= -1.0f;
	}

	if (s_disc.pos.x >= SCREEN_WIDTH - s_disc.fSize)
	{// �E
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTART_RESET);
	}
	else if (s_disc.pos.x <= s_disc.fSize)
	{// ��
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTART_RESET);
	}
}

//--------------------------------------------------
// �p�x�̐��K��
//--------------------------------------------------
static void NormalizeAngle(void)
{
	if (s_disc.rot.z >= D3DX_PI)
	{// 3.14���傫��
		s_disc.rot.z -= D3DX_PI * 2.0f;
	}
	else if (s_disc.rot.z <= -D3DX_PI)
	{// -3.14��菬����
		s_disc.rot.z += D3DX_PI * 2.0f;
	}
}
