//**************************************************
//
// FPD���� ( bg.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "bg.h"
#include "rectangle.h"
#include "color.h"
#include "texture.h"
#include "player.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const float	MAX_WIDTH = 200.0f;		// ���̍ő�l
const float	MAX_HEIGHT = 125.0f;	// �����̍ő�l
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	nIdxBG;					// �w�i�̋�`�̃C���f�b�N�X
int	nIdxStage;				// �X�e�[�W�̋�`�̃C���f�b�N�X
int	nIdxSet[MAXPLAYER];		// �Z�b�g��
int	nIdxPoint[MAXPLAYER];	// �|�C���g��
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBG(void)
{
	{// �w�i
		// ��`�̐ݒ�
		nIdxBG = SetRectangle(TEXTURE_NONE);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(nIdxBG, pos, size);
	}

	{// �X�e�[�W
		// ��`�̐ݒ�
		nIdxStage = SetRectangle(TEXTURE_Stage01);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.8f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(nIdxStage, pos, size);
	}

	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	{// �Z�b�g��
		// ��`�̐ݒ�
		nIdxSet[0] = SetRectangle(TEXTURE_GAME_SetFrame01);
		nIdxSet[1] = SetRectangle(TEXTURE_GAME_SetFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(nIdxSet[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.9f;

		SetPosRectangle(nIdxSet[1], pos, size);
	}

	{// �|�C���g��
		// ��`�̐ݒ�
		nIdxPoint[0] = SetRectangle(TEXTURE_GAME_PtsFrame01);
		nIdxPoint[1] = SetRectangle(TEXTURE_GAME_PtsFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(nIdxPoint[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.7f;

		SetPosRectangle(nIdxPoint[1], pos, size);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBG(void)
{
	// �g���̂��~�߂�
	StopUseRectangle(nIdxBG);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBG(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBG(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}
