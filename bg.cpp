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
#include "map.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_STAGE = 3;			// �X�e�[�W�̍ő吔
const float	MAX_WIDTH = 200.0f;		// ���̍ő�l
const float	MAX_HEIGHT = 125.0f;	// �����̍ő�l
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nIdxBG;				// �w�i�̋�`�̃C���f�b�N�X
int	s_nIdxStage;			// �X�e�[�W�̋�`�̃C���f�b�N�X
int	s_nIdxSet[MAXPLAYER];	// �Z�b�g��
int	s_nIdxPoint[MAXPLAYER];	// �|�C���g��
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBG(void)
{
	{// �w�i
		TEXTURE aTexture[MAX_STAGE];

		aTexture[0] = TEXTURE_BG;
		aTexture[1] = TEXTURE_BG2;
		aTexture[2] = TEXTURE_BG3;

		// ��`�̐ݒ�
		s_nIdxBG = SetRectangle(aTexture[GetSelectMap()]);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// �X�e�[�W
		TEXTURE aTexture[MAX_STAGE];

		aTexture[0] = TEXTURE_Stage01;
		aTexture[1] = TEXTURE_Stage02;
		aTexture[2] = TEXTURE_Stage03;

		// ��`�̐ݒ�
		s_nIdxStage = SetRectangle(aTexture[GetSelectMap()]);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.8f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxStage, pos, size);
	}

	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	{// �Z�b�g��
		// ��`�̐ݒ�
		s_nIdxSet[0] = SetRectangle(TEXTURE_GAME_SetFrame01);
		s_nIdxSet[1] = SetRectangle(TEXTURE_GAME_SetFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxSet[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.9f;

		SetPosRectangle(s_nIdxSet[1], pos, size);
	}

	{// �|�C���g��
		// ��`�̐ݒ�
		s_nIdxPoint[0] = SetRectangle(TEXTURE_GAME_PtsFrame01);
		s_nIdxPoint[1] = SetRectangle(TEXTURE_GAME_PtsFrame02);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.1f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxPoint[0], pos, size);

		pos.x = SCREEN_WIDTH * 0.7f;

		SetPosRectangle(s_nIdxPoint[1], pos, size);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBG(void)
{
	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdxStage);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		StopUseRectangle(s_nIdxSet[i]);
		StopUseRectangle(s_nIdxPoint[i]);
	}
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
