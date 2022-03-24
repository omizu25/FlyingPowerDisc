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
#include "effect.h"
#include "utility.h"
#include "score.h"
#include "wall.h"
#include "sound.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	EFFECT_TIME = 1;									// �G�t�F�N�g�̏o���^�C��
const float	DISC_SIZE = 75.0f;									// �f�B�X�N�̃T�C�Y
const float	DISC_ROT_SPEED = -0.25f;							// �f�B�X�N�̉�]���x
const float	MAX_MOVE = 10.0f;									// �ړ��ʂ̍ő�l
const float	START_POS_X = SCREEN_WIDTH * 0.5f;					// �f�B�X�N�̎n�܂��X�̈ʒu
const float	START_POS_Y = SCREEN_HEIGHT - (DISC_SIZE * 0.5f);	// �f�B�X�N�̎n�܂��Y�̈ʒu
const float	GOAL_TOP = 350.0f;									// �S�[���̏�[
const float	GOAL_BOTTOM = 500.0f;								// �S�[���̉��[
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
void UpdateNormal(void);
void Goal(void);
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
	s_disc.nThrow = s_nPossPlayer ^ 1;
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
	//�T�E���h��~
	StopSound();

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
	case GAMESTATE_NONE:	// �������Ă��Ȃ����
		break;

	case GAMESTATE_START:	// �J�n���
		UpdateStart();
		break;

	case GAMESTATE_NORMAL:	// �ʏ���
		UpdateNormal();
		break;

	case GAMESTATE_RESET:	// ���Z�b�g���
		break;

	case GAMESTATE_END:		// �I�����
	case GAMESTATE_RESULT:	// ���U���g���

		break;

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
// �擾
//--------------------------------------------------
Disc *GetDisc(void)
{
	return &s_disc;
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetDisc(void)
{
	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// ���Ɏn�߂�v���C���[�̐ݒ�
//--------------------------------------------------
void SetPossDisc(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAXPLAYER);

	s_nPossPlayer = nIdx;
	s_disc.nThrow = s_nPossPlayer ^ 1;
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

	D3DXVECTOR3 posDest = pPlayer->pos;

	switch (s_nPossPlayer)
	{
	case 0:
		posDest.x += PLAYERSIZE * 0.5f;
		break;
		
	case 1:
		posDest.x += -PLAYERSIZE * 0.5f;
		break;

	default:
		posDest = pPlayer->pos;
		assert(false);
		break;
	}

	// �z�[�~���O
	Homing(&s_disc.pos, s_disc.pos, posDest, MAX_MOVE);

	// ��]
	s_disc.rot.z += DISC_ROT_SPEED;

	// �p�x�̐��K��
	NormalizeAngle(&s_disc.rot.z);

	// �ʒu�̍X�V
	s_disc.pos += s_disc.move;
	
	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);

	if (CollisionPlayer(&s_disc, DISC_SIZE, s_disc.nThrow ^ 1))
	{// �v���C���[�ƃf�B�X�N�̓����蔻��
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTATE_NORMAL);

		s_nPossPlayer = s_nPossPlayer ^ 1;
	}
}

//--------------------------------------------------
// �ʏ���
//--------------------------------------------------
void UpdateNormal(void)
{
	if (s_disc.bHave)
	{// �v���C���[�������Ă���
		return;
	}

	/*�� �v���C���[�������ĂȂ� ��*/

	// ��]
	s_disc.rot.z += DISC_ROT_SPEED;

	// �p�x�̐��K��
	NormalizeAngle(&s_disc.rot.z);

	// �ʒu�̍X�V
	s_disc.posOld = s_disc.pos;
	s_disc.pos += s_disc.move;
	
	//------------------------
	//���̂܂܎g���ƕς�������Ԋu�Ŏg���Ƃ�������
	//------------------------
	s_disc.nEffect++;

	if (s_disc.nEffect % EFFECT_TIME == 0)
	{// ���Ԋu
		// �G�t�F�N�g�̐ݒ�
		SetEffect(s_disc.pos, GetColor(COLOR_WHITE), EFFECTSTATE_SPIN, 30, 100.0f, false);
	}
	
	// �v���C���[�ƃf�B�X�N�̓����蔻��
	CollisionPlayer(&s_disc, DISC_SIZE, s_disc.nThrow ^ 1);

	if (CollisionWall(&s_disc.pos, &s_disc.posOld))
	{// �ǂƃf�B�X�N�̓����蔻��
		s_disc.move.y *= -1.0f;
	}

	// �S�[��
	Goal();

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, DISC_SIZE, DISC_SIZE);
}

//--------------------------------------------------
// �S�[��
//--------------------------------------------------
void Goal(void)
{
	float fRadius = DISC_SIZE * 0.5f;

	if (s_disc.pos.x >= SCREEN_WIDTH - fRadius)
	{// �E
		if ((s_disc.pos.y >= GOAL_TOP) && s_disc.pos.y <= GOAL_BOTTOM)
		{
			// �|�C���g���̉��Z
			AddPointScore(0, 5);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT5, 100, 50.0f, true);
		}
		else
		{
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT3, 100, 50.0f, true);
			// �|�C���g���̉��Z
			AddPointScore(0, 3);
		}
	
		//����
		PlaySound(SOUND_LABEL_KANSEI);
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTATE_RESET);

		// ���Z�b�g
		ResetDisc();

		// ���Ɏn�߂�v���C���[�̐ݒ�
		SetPossDisc(1);
	}
	else if (s_disc.pos.x <= fRadius)
	{// ��
		if ((s_disc.pos.y >= GOAL_TOP) && s_disc.pos.y <= GOAL_BOTTOM)
		{
			// �|�C���g���̉��Z
			AddPointScore(1, 5);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT5, 100, 50.0f, false);
		}
		else
		{
			// �|�C���g���̉��Z
			AddPointScore(1, 3);
			SetEffect(D3DXVECTOR3(s_disc.pos.x, s_disc.pos.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_GOAL_POINT3, 100, 50.0f, false);
		}
		
		//����
		PlaySound(SOUND_LABEL_KANSEI);
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTATE_RESET);

		// ���Z�b�g
		ResetDisc();

		// ���Ɏn�߂�v���C���[�̐ݒ�
		SetPossDisc(0);
	}
}
} // namespace�͂����܂�
