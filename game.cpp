//**************************************************
//
// FPD���� ( game.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "rectangle.h"
#include "disc.h"
#include "ui.h"
#include "pause.h"
#include "menu.h"

#include "wall.h"
#include <assert.h>
//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static GAMESTATE		s_gameState = GAMESTATE_NONE;		// �Q�[���̏��
static int				s_nCounterState;					// ��ԊǗ��J�E���^�[
static bool				s_bPause = false;					// �|�[�Y�����ǂ��� [���Ă�  : true ���ĂȂ�  : false]

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{

	InitWall();
	// ��`�̏�����
	InitRectangle();

	// �f�B�X�N�̏�����
	InitDisc();

	//�v���C���[�̏�����
	InitPlayer();

	//UI�̏�����
	InitUi();

	// �|�[�Y�̏�����
	InitPause();

	// ���j���[�̏�����
	InitMenu();

	//UI�̔z�u
	SetUi(D3DXVECTOR3 (50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 0);
	SetUi(D3DXVECTOR3 (SCREEN_WIDTH - 50.0f, 15.0f, 0.0f), 100.0f, 30.0f, 1);

	s_gameState = GAMESTATE_START;		// �J�n��Ԃɐݒ�

	s_nCounterState = 0;				// �J�E���^�[�̏�����

	s_bPause = false;					// �|�[�Y����
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	// �T�E���h�̒�~
	StopSound();

	UninitWall();

	// ��`�̏I��
	UninitRectangle();

	// �f�B�X�N�̏I��
	UninitDisc();

	// �v���C���[�̏I��
	UninitPlayer();

	//UI�̏I��
	UninitUi();

	// �|�[�Y�̏I��
	UninitPause();

	// ���j���[�̏I��
	UninitMenu();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetDirectJoypadTrigger(JOYKEY_DIRECT_7_BUTTON) || GetKeyboardTrigger(DIK_P))
	{
		// �|�[�Y�̃��Z�b�g
		ResetPause();

		s_bPause = !s_bPause;

		if (s_bPause)
		{// �|�[�Y���Ă���
			// ���j���[�̐ݒ�
			SetPause();
		}
	}

	if (s_bPause)
	{// �|�[�Y���Ă���
		// �|�[�Y�̍X�V
		UpdatePause();

		// ���j���[�̍X�V
		UpdateMenu();
		return;
	}

	// �f�B�X�N�̍X�V
	UpdateDisc();

	switch (s_gameState)
	{
	case GAMESTATE_START:		// �J�n���
		s_gameState = GAMESTATE_NORMAL;
		break;

	case GAMESTATE_NORMAL:		// �ʏ���
		
		UpdateWall();
		// �v���C���[�̍X�V
		UpdatePlayer();
		
		//UI�̍X�V(�܂��������ĂȂ�)
		UpdateUi();

		break;

	case GAMESTART_RESET:		// ���Z�b�g���

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
// �`��
//--------------------------------------------------
void DrawGame(void)
{
	DrawWall();
	// �v���C���[�̕`��
	DrawPlayer();

	//UI�̕`��
	DrawUi();

	// ��`�̕`��
	DrawRectangle();
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
