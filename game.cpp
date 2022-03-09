//==================================================
//
// FPD���� ( game.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include <assert.h>

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		// �Q�[���̏��
static int				s_nCounterState;					// ��ԊǗ��J�E���^�[
static bool				s_bPause = false;					// �|�[�Y�����ǂ��� [���Ă�  : true ���ĂȂ�  : false]

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	s_gameState = GAMESTATE_START;		// �J�n��Ԃɐݒ�

	s_nCounterState = 0;				// �J�E���^�[�̏�����

	s_bPause = false;					// �|�[�Y����

	InitPlayer();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	UninitPlayer();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START, 0))
	{// P�L�[�������ꂽ���ǂ���
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{// �|�[�Y��
		return;
	}

	switch (s_gameState)
	{
	case GAMESTATE_NONE:		// �������Ă��Ȃ����
		
		break;

	case GAMESTATE_START:		// �J�n���
		UpdatePlayer();
		break;

	case GAMESTATE_NORMAL:		// �ʏ���
		
		break;

	case GAMESTATE_END:			// �I�����

		break;

	case GAMESTATE_RESULT:		// ���U���g���

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawGame(void)
{
	if (s_bPause)
	{// �|�[�Y��
		return;
	}
	DrawPlayer();
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;
	s_nCounterState = 0;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
GAMESTATE GetGameState(void)
{
	return s_gameState;
}

//--------------------------------------------------
// �|�[�Y�̗L�������ݒ�
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}
