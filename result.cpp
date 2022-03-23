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
const float	MAX_WIDTH = 200.0f;		// ���̍ő�l
const float	MAX_HEIGHT = 125.0f;	// �����̍ő�l
const int	LIMIT_TIME = 300;		// ���Ԃ̌��E

/*�� ���s ��*/

typedef struct
{
	int		nIdx;		// ��`�̃C���f�b�N�X
	bool	bResult;	// ���s [ true : ���� / false : ���� ]
	bool	bDraw;		// �`�悷�邩�ǂ���
}Result;

/*�� �������� ��*/

typedef struct
{
	int		nIdx;		// ��`�̃C���f�b�N�X
	bool	bDraw;		// �`�悷�邩�ǂ���
}Draw;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Result	s_result[MAXPLAYER];	// ���s�̏��
Draw	s_draw;					// ���������̏��
int		s_nTime;				// ����

}// namesapce�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitResult(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_result[i];

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5f, 0.0f);

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

		pResult->bResult = false;
		pResult->bDraw = false;
	}

	// ��`�̐ݒ�
	s_draw.nIdx = SetRectangle(TEXTURE_NONE);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// ��`�̈ʒu�̐ݒ�
	SetPosRectangle(s_draw.nIdx, pos, size);

	// ��`�̕`�悷�邩�ǂ���
	SetDrawRectangle(s_draw.nIdx, false);

	s_draw.bDraw = false;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitResult(void)
{
	
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

	if (GetKeyboardTrigger(DIK_RETURN))
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
void SetResult(int nPlayerNo, bool bResult)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	Result *pResult = &s_result[nPlayerNo];

	pResult->bResult = bResult;

	if (bResult)
	{// ����
		// ��`�̃e�N�X�`���̐ݒ�
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Disc);
	}
	else
	{// ����
		// ��`�̃e�N�X�`���̐ݒ�
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Select_Left);
	}
}

//--------------------------------------------------
// �`�悷�邩�ǂ���
//--------------------------------------------------
void SetDrawResult(bool bDraw)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_result[i];

		pResult->bDraw = bDraw;

		// ��`�̕`�悷�邩�ǂ���
		SetDrawRectangle(pResult->nIdx, bDraw);
	}
}
