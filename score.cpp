//**************************************************
//
// FPG���� ( score.h )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "score.h"
#include "player.h"
#include "number.h"
#include "color.h"
#include "rule.h"
#include "mode.h"
#include "time.h"
#include "ui.h"
#include "game.h"
#include "result.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_UI_TIME = 120;			// UI�̕`�悷�鎞��
const float	NUMBER_WIDTH = 40.0f;		// ���̕�
const float	NUMBER_HEIGHT = 60.0f;		// ���̍���
const float	NUMBER_UI_WIDTH = 200.0f;	// UI�̐��̕�
const float	NUMBER_UI_HEIGHT = 300.0f;	// UI�̐��̍���
const float	START_POS_Y = 70.0f;		// �n�܂��Y�̈ʒu
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nSet[MAXPLAYER];		// �Z�b�g��
int s_nPoint[MAXPLAYER];	// �|�C���g��
int s_nSetIdx[MAXPLAYER];	// �Z�b�g���̒l�̃C���f�b�N�X
int s_nPointIdx[MAXPLAYER];	// �|�C���g���̒l�̃C���f�b�N�X
int s_nPointUI[MAXPLAYER];	// �|�C���g����UI�p�̒l�̃C���f�b�N�X
int s_nUITime;				// UI��`�悷�鎞��
int s_nCntSet;				// �Z�b�g���̃J�E���g
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitScore(void)
{
	s_nCntSet = 0;
	s_nUITime = -1;

	D3DXVECTOR3 size = D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f);
	D3DXVECTOR3 sizeUI = D3DXVECTOR3(NUMBER_UI_WIDTH, NUMBER_UI_HEIGHT, 0.0f);

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		s_nSet[nPlayerNo] = 0;
		s_nPoint[nPlayerNo] = 0;

		D3DXVECTOR3 posSet = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
		D3DXVECTOR3 posPoint = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
		D3DXCOLOR color = GetColor(COLOR_WHITE);

		switch (nPlayerNo)
		{
		case 0:
			posSet.x = SCREEN_WIDTH * 0.17f;
			posPoint.x = SCREEN_WIDTH * 0.37f;
			color = GetColor(COLOR_RED);
			break;

		case 1:
			posSet.x = SCREEN_WIDTH * 0.97f;
			posPoint.x = SCREEN_WIDTH * 0.77f;
			color = GetColor(COLOR_BLUE);
			break;

		default:
			assert(false);
			break;
		}

		// ���̐ݒ�
		s_nSetIdx[nPlayerNo] = SetNumber(posSet, size, GetColor(COLOR_BLACK), s_nSet[nPlayerNo]);
		s_nPointIdx[nPlayerNo] = SetNumber(posPoint, size, GetColor(COLOR_BLACK), s_nPoint[nPlayerNo]);

		int nDigit = DigitNumber(s_nPoint[nPlayerNo]);
		D3DXVECTOR3 posUI = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + ((nPlayerNo * 600.0f) - 200.0f), 450.0f, 0.0f);
		posUI.x += (NUMBER_UI_WIDTH * ((nDigit - 1) * 0.5f));

		s_nPointUI[nPlayerNo] = SetNumber(posUI, sizeUI, color, s_nPoint[nPlayerNo]);
		
		// ���̕`�悷�邩�ǂ���
		SetDrawNumber(s_nPointUI[nPlayerNo], false);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitScore(void)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		// �����g���̂��~�߂�
		StopUseNumber(s_nSetIdx[nPlayerNo]);
		StopUseNumber(s_nPointIdx[nPlayerNo]);
		StopUseNumber(s_nPointUI[nPlayerNo]);
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateScore(void)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		if (s_nPoint[nPlayerNo] >= GetPointRule())
		{// �|�C���g�����w��̒l���z����
			// �|�C���g����0�ɂ���
			ZeroPointScore(0);
			ZeroPointScore(1);

			// ���Ԃ̃��Z�b�g
			SetTime(GetTimeRule());

			// �Z�b�g���̉��Z
			AddSetScore(nPlayerNo, 1);

			// �v���C���[�̃Q�[���̃��Z�b�g
			ResetGaugePlayer();
		}

		if (s_nSet[nPlayerNo] >= GetSetRule())
		{// �Z�b�g�����w��̒l���z����

			if (s_nSet[0] == s_nSet[1])
			{// ��������
				SetResult(0, RESULT_DRAW);
				SetResult(1, RESULT_DRAW);
			}
			else if (s_nSet[0] > s_nSet[1])
			{// 1P�̏���
				SetResult(0, RESULT_WIN);
				SetResult(1, RESULT_LOSE);
			}
			else if (s_nSet[0] < s_nSet[1])
			{// 2P�̏���
				SetResult(0, RESULT_LOSE);
				SetResult(1, RESULT_WIN);
			}

			// ���U���g�̕`�悷�邩�ǂ���
			SetDrawResult(true);

			// �Q�[���̏�Ԃ̐ݒ�
			SetGameState(GAMESTATE_RESULT);

			s_nCntSet = 0;
		}
	}

	if (s_nUITime >= 0)
	{
		s_nUITime++;

		if (s_nUITime >= MAX_UI_TIME)
		{
			// ���̕`�悷�邩�ǂ���
			SetDrawNumber(s_nPointUI[0], false);
			SetDrawNumber(s_nPointUI[1], false);
			s_nUITime = -1;

			// �Q�[���̏�Ԃ̐ݒ�
			SetGameState(GAMESTATE_START);
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawScore(void)
{

}

//--------------------------------------------------
// �Z�b�g����0�ɂ���
//--------------------------------------------------
void ZeroSetScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nSet[nPlayerNo] = 0;

	// ���̕ύX
	s_nSetIdx[nPlayerNo] = ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);
}

//--------------------------------------------------
// �|�C���g����0�ɂ���
//--------------------------------------------------
void ZeroPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nPoint[nPlayerNo] = 0;

	// ���̕ύX
	s_nPointIdx[nPlayerNo] = ChangeNumber(s_nPointIdx[nPlayerNo], s_nPoint[nPlayerNo]);
}

//--------------------------------------------------
// �Z�b�g�������Z
//--------------------------------------------------
void AddSetScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nSet[nPlayerNo] += nValue;

	// ���̕ύX
	s_nSetIdx[nPlayerNo] = ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);

	if (s_nSet[nPlayerNo] >= GetSetRule())
	{// �Z�b�g�����w��̒l���z����
		return;
	}

	switch (s_nCntSet)
	{// �Z�b�g���ɉ����ĕ\������
	case 0:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 5);
		break;

	case 1:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 6);
		break;

	case 2:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 7);
		break;

	case 3:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 8);
		break;

	default:
		assert(false);
		break;
	}

	s_nCntSet++;	// �Z�b�g���̃J�E���g�𑝂₷
}

//--------------------------------------------------
// �S�ẴZ�b�g�������Z
// ����2  : int nValue / ���Z����l
//--------------------------------------------------
void AllAddSetScore(int nValue)
{
	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		s_nSet[nPlayerNo] += nValue;

		// ���̕ύX
		s_nSetIdx[nPlayerNo] = ChangeNumber(s_nSetIdx[nPlayerNo], s_nSet[nPlayerNo]);
	}

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		if (s_nSet[nPlayerNo] >= GetSetRule())
		{// �Z�b�g�����w��̒l���z����
			return;
		}
	}

	switch (s_nCntSet)
	{// �Z�b�g���ɉ����ĕ\������
	case 0:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 5);
		break;

	case 1:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 6);
		break;

	case 2:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 7);
		break;

	case 3:
		// �Z�b�g���̕\��
		SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f, 4, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 8);
		break;

	default:
		assert(false);
		break;
	}

	s_nCntSet++;	// �Z�b�g���̃J�E���g�𑝂₷
}

//--------------------------------------------------
// �|�C���g�������Z
//--------------------------------------------------
void AddPointScore(int nPlayerNo, int nValue)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	s_nPoint[nPlayerNo] += nValue;

	// ���̕ύX
	s_nPointIdx[nPlayerNo] = ChangeNumber(s_nPointIdx[nPlayerNo], s_nPoint[nPlayerNo]);

	if (s_nPoint[nPlayerNo] < GetPointRule())
	{// �|�C���g�����w��̒l��艺
		for (int i = 0; i < MAXPLAYER; i++)
		{
			// ���̕ύX
			s_nPointUI[i] = ChangeNumber(s_nPointUI[i], s_nPoint[i]);

			int nDigit = DigitNumber(s_nPoint[i]);
			D3DXVECTOR3 size = D3DXVECTOR3(NUMBER_UI_WIDTH, NUMBER_UI_HEIGHT, 0.0f);
			D3DXVECTOR3 pos = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + ((i * 600.0f) - 200.0f), 450.0f, 0.0f);
			pos.x += (NUMBER_UI_WIDTH * ((nDigit - 1) * 0.5f));

			// ���̈ʒu�̐ݒ�
			SetPosNumber(s_nPointUI[i], pos, size);

			// ���̕`�悷�邩�ǂ���
			SetDrawNumber(s_nPointUI[i], true);
		}
		
		s_nUITime = 0;
	}
}

//--------------------------------------------------
// �Z�b�g���̎擾
//--------------------------------------------------
int GetSetScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return s_nSet[nPlayerNo];
}

//--------------------------------------------------
// �|�C���g���̎擾
//--------------------------------------------------
int GetPointScore(int nPlayerNo)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	return s_nPoint[nPlayerNo];
}