//**************************************************
//
// FPD���� ( result.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "result.h"
#include "player.h"
#include "rectangle.h"
#include "game.h"
#include "mode.h"
#include "input.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const float	MAX_WIDTH = 400.0f;		// ���̍ő�l
const float	MAX_HEIGHT = 300.0f;	// �����̍ő�l
const float	DRAW_WIDTH = 700.0f;	// ���̍ő�l
const float	DRAW_HEIGHT = 350.0f;	// �����̍ő�l
const int	LIMIT_TIME = 300;		// ���Ԃ̌��E

/*�� ���s ��*/

typedef struct
{
	int		nIdx;	// ��`�̃C���f�b�N�X
	RESULT	result;	// ���s
}Result;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Result	s_aResult[MAXPLAYER];	// ���s�̏��
int		s_nIdxDraw;				// ���������̏��
int		s_nTime;				// ����
}// namesapce�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitResult(void)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.6f, 0.0f);

		switch (i)
		{
		case 0:
			pos.x = SCREEN_WIDTH * 0.25f;
			break;

		case 1:
			pos.x = SCREEN_WIDTH * 0.75f;
			break;
		
		default:
			assert(false);
			break;
		}

		// ��`�̐ݒ�
		pResult->nIdx = SetRectangle(TEXTURE_NONE);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(pResult->nIdx, pos, size);

		// ��`�̕`�悷�邩�ǂ���
		SetDrawRectangle(pResult->nIdx, false);

		pResult->result = RESULT_NONE;
	}

	{// ��������
		// ��`�̐ݒ�
		s_nIdxDraw = SetRectangle(TEXTURE_Result_Draw);

		D3DXVECTOR3 size = D3DXVECTOR3(DRAW_WIDTH, DRAW_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxDraw, pos, size);

		// ��`�̕`�悷�邩�ǂ���
		SetDrawRectangle(s_nIdxDraw, false);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitResult(void)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		// �g���̂��~�߂�
		StopUseRectangle(pResult->nIdx);
	}

	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxDraw);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetGameState() != GAMESTATE_RESULT)
	{// ���U���g��Ԃł͂Ȃ�
		return;
	}

	s_nTime++;

	if (s_nTime >= LIMIT_TIME)
	{// ���Ԃ��z����
		s_nTime = 0;

		// ���[�h�̕ύX
		ChangeMode(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		// ���[�h�̕ύX
		ChangeMode(MODE_TITLE);
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawResult(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}

//--------------------------------------------------
// ���s�̐ݒ�
//--------------------------------------------------
void SetResult(int nPlayerNo, RESULT result)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	Result *pResult = &s_aResult[nPlayerNo];

	pResult->result = result;

	switch (result)
	{// ��`�̃e�N�X�`���̐ݒ�
	case RESULT_WIN:	// ����
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Result_Win);
		break;

	case RESULT_LOSE:	// ����
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Result_Lose);
		break;

	case RESULT_DRAW:	// ��������
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`�悷�邩�ǂ���
//--------------------------------------------------
void SetDrawResult(bool bDraw)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		if (pResult->result == RESULT_DRAW)
		{// ��������
			// ��`�̕`�悷�邩�ǂ���
			SetDrawRectangle(s_nIdxDraw, true);

			continue;
		}

		// ��`�̕`�悷�邩�ǂ���
		SetDrawRectangle(pResult->nIdx, bDraw);

		// ��`�̕`�悷�邩�ǂ���
		SetDrawRectangle(s_nIdxDraw, false);
	}
}
