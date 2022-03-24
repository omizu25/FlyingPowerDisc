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
#include "effect.h"
#include "time.h"
#include "rule.h"
#include "number.h"
#include "score.h"
#include "bg.h"
#include "result.h"
#include "cursor.h"

#include <assert.h>

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
GAMESTATE	s_gameState = GAMESTATE_NONE;	// �Q�[���̏��
int			s_nCounterState;				// ��ԊǗ��J�E���^�[
bool		s_bPause = false;				// �|�[�Y�����ǂ��� [���Ă�  : true ���ĂȂ�  : false]
}// namesapce�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	// ���[���̓ǂݍ���
	LoadRule();

	// ��`�̏�����
	InitRectangle();

	// �w�i�̏�����
	InitBG();

	// �^�C���̏�����
	InitTime();

	// ���̏�����
	InitNumber();

	// ���ׂ̏�����
	InitWall();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �f�B�X�N�̏�����
	InitDisc();

	// �v���C���[�̏�����
	InitPlayer();

	// �v���C���[�̐ݒ�
	Player *pPlayer = GetPlayer();
	SetPlayer(D3DXVECTOR3((float)PLAYER_POS_X + PLAYERSIZE, (float)SCREEN_HEIGHT * 0.6f, 0.0f), pPlayer->nType, true, PLAYERSIZE);
	pPlayer++;
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYER_POS_X - PLAYERSIZE, (float)SCREEN_HEIGHT * 0.6f, 0.0f), pPlayer->nType, false, PLAYERSIZE);

	// UI�̏�����
	InitUi();

	// �X�R�A�̏�����
	InitScore();

	// ���j���[�̏�����
	InitMenu();

	// �J�[�\��������
	InitCursor();

	// �|�[�Y�̏�����
	InitPause();

	// ���U���g�̏�����
	InitResult();

	// UI�̔z�u			�u�����W			����	�c��	�^�C�v			�g�嗦�ƃt�F�[�h	�e�N�X�`���̎��
	SetUi(D3DXVECTOR3 (125.0f, 350.0f, 0.0f), 150.0f, 50.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f),0);	
	SetUi(D3DXVECTOR3 (SCREEN_WIDTH - 125.0f, 350.0f, 0.0f), 150.0f, 50.0f, 1, D3DXVECTOR3(1.0f, 1.0f, 0.0f), 1);
	// ������o�Ă�����
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 250.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 450.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, 650.0f, 0.0f), 150.0f, 60.0f, 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// �E����o�Ă�����																				
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 250.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 450.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),2);
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 650.0f, 0.0f), 150.0f, 60.0f, 3, D3DXVECTOR3(0.0f, 1.0f, 0.0f),3);
	// �Z�b�g��																							
	SetUi(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), 400.0f, 150.0f,4, D3DXVECTOR3(1.0f, 0.0f, 0.0f),4);
	
	//�T�E���h�J�n
	PlaySound(SOUND_LABEL_BGM);

	// �I���������Ԃ�\��
	SetTime(GetTimeRule());

	s_gameState = GAMESTATE_NONE;	// �������Ă��Ȃ���Ԃɐݒ�

	s_nCounterState = 0;			// �J�E���^�[�̏�����

	s_bPause = false;				// �|�[�Y����
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �w�i�̏I��
	UninitBG();

	// �^�C���̏I��
	UninitTime();

	// ���̏I��
	UninitNumber();

	// �X�R�A�̏I��
	UninitScore();

	// ���ׂ̏I��
	UninitWall();

	// ��`�̏I��
	UninitRectangle();

	// �f�B�X�N�̏I��
	UninitDisc();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �v���C���[�̏I��
	UninitPlayer();

	// ���U���g�̏I��
	UninitResult();

	// UI�̏I��
	UninitUi();

	// �|�[�Y�̏I��
	UninitPause();

	// ���j���[�̏I��
	UninitMenu();

	// �J�[�\���̏I��
	UninitCursor();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{// P�L�[�������ꂽ
		if (s_gameState == GAMESTATE_NORMAL)
		{// �ʏ��Ԃ̎�
			s_bPause = !s_bPause;

			if (s_bPause)
			{// �|�[�Y���Ă�
				// ���j���[�̐ݒ�
				SetPause();
			}
			else
			{// �|�[�Y���ĂȂ�
				// �|�[�Y�̃��Z�b�g
				ResetPause();
			}
		}
	}

	if (s_bPause)
	{// �|�[�Y���Ă���
		// �|�[�Y�̍X�V
		UpdatePause();

		// ���j���[�̍X�V
		UpdateMenu();

		// �J�[�\���̍X�V
		UpdateCursor();
		return;
	}

	// �f�B�X�N�̍X�V
	UpdateDisc();

	// �G�t�F�N�g�̍X�V
	UpdateEffect();

	// UI�̍X�V
	UpdateUi();

	// ���ׂ̍X�V
	UpdateWall();

	// �^�C���̍X�V
	UpdateTime();

	// �X�R�A�̍X�V
	UpdateScore();

	// �v���C���[�̍X�V
	UpdatePlayer();

	// ���U���g�̍X�V
	UpdateResult();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawGame(void)
{
	// ��`�̕`��
	DrawRectangle();

	// �^�C���̕`��
	DrawTime();

	// UI�̕`��
	DrawUi();
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	assert(state >= 0 && state < GAMESTATE_MAX);

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
