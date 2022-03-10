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
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static Disc						s_disc;					// �f�B�X�N�̏��
static int						s_nPossPlayer;			// ���̃f�B�X�N�̎n�܂�̃v���C���[

//==================================================
// �v���g�^�C�v�錾
//==================================================
static void OffScreen(void);
static void NormalizeAngle(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitDisc(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/disc.png",
		&s_pTexture);

	s_nPossPlayer = 0;

	s_disc.pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(MAX_MOVE, MAX_MOVE, 0.0f);
	s_disc.fSize = DISC_SIZE * 0.5f;

	// ��`�̐ݒ�
	s_disc.nIdx = SetRectangle(s_pTexture);

	// ��`�̉�]����ʒu�̐ݒ�
	SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitDisc(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateDisc(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_START:		// �J�n���
	{
		Player *pPlayer = GetPlayer();

		pPlayer += (s_nPossPlayer * 1);

		//float fRotMove, fRotDest, fRotDiff;

		////���݂̈ړ�����(�p�x)
		//fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);

		////�ړI�̈ړ�����(�p�x)
		//fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);

		//fRotDiff = fRotDest - fRotMove;		//�ړI�̈ړ������܂ł̍���

		//if (fRotDiff >= D3DX_PI)
		//{
		//	fRotDiff = fRotDiff - (D3DX_PI * 2.0f);
		//}
		//else if (fRotDiff <= -D3DX_PI)
		//{
		//	fRotDiff = fRotDiff + (D3DX_PI * 2.0f);
		//}

		//fRotMove += fRotDiff * 0.075f;		//�ړ�����(�p�x)�̕␳

		//if (fRotMove >= D3DX_PI)
		//{
		//	fRotMove = fRotMove - (D3DX_PI * 2.0f);
		//}
		//else if (fRotMove <= -D3DX_PI)
		//{
		//	fRotMove = fRotMove + (D3DX_PI * 2.0f);
		//}

		//g_aBullet[nCntBullet].move.x = sinf(fRotMove) * 9.0f;
		//g_aBullet[nCntBullet].move.y = cosf(fRotMove) * 9.0f;
	}
	break;

	case GAMESTATE_RESTART:		// �ĊJ�n���
		break;

	case GAMESTATE_NORMAL:		// �ʏ���
		// ��]
		s_disc.rot.z += DISC_ROT_SPEED;

		// �p�x�̐��K��
		NormalizeAngle();

		// �ʒu�̍X�V
		s_disc.pos += s_disc.move;

		// ��ʊO
		OffScreen();

		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(s_disc.nIdx, s_disc.pos, s_disc.rot, s_disc.fSize, s_disc.fSize);
		break;

	case GAMESTATE_END:			// �I�����

		break;

	case GAMESTATE_RESULT:		// ���U���g���

		break;

	case GAMESTATE_NONE:		// �������Ă��Ȃ����
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ��ʊO
//--------------------------------------------------
static void OffScreen(void)
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
		s_disc.pos.x = SCREEN_WIDTH - s_disc.fSize;
		s_disc.move.x *= -1.0f;
	}
	else if (s_disc.pos.x <= s_disc.fSize)
	{// ��
		s_disc.pos.x = s_disc.fSize;
		s_disc.move.x *= -1.0f;
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