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
// ��`
//==================================================
namespace
{
const float	DISC_SIZE = 75.0f;									// �f�B�X�N�̃T�C�Y
const float	DISC_ROT_SPEED = -0.25f;							// �f�B�X�N�̉�]���x
const float	MAX_MOVE = 5.0f;									// �ړ��ʂ̍ő�l
const float	START_POS_X = SCREEN_WIDTH * 0.5f;					// �f�B�X�N�̎n�܂��X�̈ʒu
const float	START_POS_Y = SCREEN_HEIGHT - (DISC_SIZE * 0.5f);	// �f�B�X�N�̎n�܂��Y�̈ʒu

typedef struct
{
	D3DXVECTOR3	pos;	// �ʒu
	D3DXVECTOR3	rot;	// ����
	D3DXVECTOR3	move;	// �ړ���
	int			nIdx;	// ��`�̃C���f�b�N�X
	int			nThrow;	// ���Ԗڂ̃v���C���[����������
	bool		bHave;	// �v���C���[�������Ă��邩�ǂ���
}Disc;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Disc	s_disc;			// �f�B�X�N�̏��
int		s_nPossPlayer;	// ���̃f�B�X�N�̎n�܂�̃v���C���[
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
//==================================================
namespace
{
void UpdateStart(void);
void UpdateReset(void);
void UpdateNormal(void);
void Reflect(void);
void Collision(void);
void NormalizeAngle(float *pAngle);
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitDisc(void)
{
	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.nThrow = 0;
	s_disc.bHave = false;

	// ��`�̐ݒ�
	s_disc.nIdx = SetRectangle(TEXTURE_Disc);

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
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
	case GAMESTATE_START:	// �J�n���
		UpdateStart();
		break;

	case GAMESTATE_NORMAL:	// �ʏ���
		UpdateNormal();
		break;

	case GAMESTART_RESET:	// ���Z�b�g���
		UpdateReset();
		break;

	case GAMESTATE_END:		// �I�����
	case GAMESTATE_RESULT:	// ���U���g���

		break;

	case GAMESTATE_NONE:	// �������Ă��Ȃ����
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

//==================================================
// �X�^�e�B�b�N�֐�
//==================================================
namespace
{
//--------------------------------------------------
// �J�n���
//--------------------------------------------------
void UpdateStart(void)
{
	Player *pPlayer = GetPlayer();

	pPlayer += (s_nPossPlayer * 1);

	float fRotMove, fRotDest, fRotDiff;

	// ���݂̈ړ�����(�p�x)
	fRotMove = atan2f(s_disc.move.x, s_disc.move.y);

	// �ړI�̈ړ�����(�p�x)
	fRotDest = atan2f(pPlayer->pos.x - s_disc.pos.x, pPlayer->pos.y - s_disc.pos.y);

	fRotDiff = fRotDest - fRotMove;	// �ړI�̈ړ������܂ł̍���

	// �p�x�̐��K��
	NormalizeAngle(&fRotDiff);

	fRotMove += fRotDiff;	// �ړ�����(�p�x)�̕␳

	// �p�x�̐��K��
	NormalizeAngle(&fRotMove);

	s_disc.move.x = sinf(fRotMove) * MAX_MOVE;
	s_disc.move.y = cosf(fRotMove) * MAX_MOVE;
}

//--------------------------------------------------
// �ʏ���
//--------------------------------------------------
void UpdateNormal(void)
{
	// ��]
	s_disc.rot.z += DISC_ROT_SPEED;

	// �p�x�̐��K��
	NormalizeAngle(&s_disc.rot.z);

	// �ʒu�̍X�V
	s_disc.pos += s_disc.move;

	Collision();

	// ����
	Reflect();

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// ���Z�b�g���
//--------------------------------------------------
void UpdateReset(void)
{
	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
void Reflect(void)
{
	float fRadius = DISC_SIZE * 0.5f;

	if (s_disc.pos.y >= SCREEN_HEIGHT - fRadius)
	{// ��
		s_disc.pos.y = SCREEN_HEIGHT - fRadius;
		s_disc.move.y *= -1.0f;
	}
	else if (s_disc.pos.y <= fRadius)
	{// ��
		s_disc.pos.y = fRadius;
		s_disc.move.y *= -1.0f;
	}

	if (s_disc.pos.x >= SCREEN_WIDTH - fRadius)
	{// �E
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTART_RESET);
	}
	else if (s_disc.pos.x <= fRadius)
	{// ��
		// �Q�[���̏�Ԃ̐ݒ�
		//SetGameState(GAMESTART_RESET);
	}
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void Collision(void)
{
	if (s_disc.bHave)
	{// �v���C���[�������Ă���
		return;
	}

	Player *pPlayer = GetPlayer();

	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (!pPlayer->bUse || pPlayer->bHave)
		{// �g�p����Ă��Ȃ��A�f�B�X�N�������Ă���
			continue;
		}

		float fDiscSize = DISC_SIZE * 0.5f;
		float fHeight = (fDiscSize + (pPlayer->fheight * 0.5f));
		float fWidth = (fDiscSize + (pPlayer->fwidth * 0.5f));

		if ((s_disc.pos.y <= (pPlayer->pos.y + fHeight)) &&
			(s_disc.pos.y >= (pPlayer->pos.y - fHeight)) &&
			(s_disc.pos.x <= (pPlayer->pos.x + fWidth)) &&
			(s_disc.pos.x >= (pPlayer->pos.x - fWidth)))
		{// �v���C���[�Ƀf�B�X�N������������
			pPlayer->bHave = true;
			s_disc.bHave = true;
			s_disc.nThrow = i;

			break;
		}
	}

	s_disc.bHave = false;
}

//--------------------------------------------------
// �p�x�̐��K��
//--------------------------------------------------
void NormalizeAngle(float *pAngle)
{
	if (*pAngle >= D3DX_PI)
	{// 3.14���傫��
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle <= -D3DX_PI)
	{// -3.14��菬����
		*pAngle += D3DX_PI * 2.0f;
	}
}
} // namespace�͂����܂�
