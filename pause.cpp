//**************************************************
//
// FPG���� ( pause.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "input.h"
#include "fade.h"
#include "menu.h"
#include "game.h"
#include "mode.h"
#include "texture.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const float		MENU_WIDTH = 500.0f;					// ���j���[�̕�
const float		MENU_HEIGHT = 150.0f;					// ���j���[�̍���
const float		MENU_LEFT = SCREEN_WIDTH * 0.25f;		// ���j���[�̍��[
const float		MENU_RIGHT = SCREEN_WIDTH * 0.75f;		// ���j���[�̉E�[
const float		MENU_TOP = SCREEN_HEIGHT * 0.1f;		// ���j���[�̏�[
const float		MENU_BOTTOM = SCREEN_HEIGHT * 0.9f;		// ���j���[�̉��[
const D3DXCOLOR	BG_COLOR(0.0f, 0.0f, 0.0f, 0.6f);		// �w�i�̐F
const D3DXCOLOR	FRAME_COLOR(0.5f, 1.0f, 0.5f, 1.0f);	// �t���[���̐F

typedef enum
{
	MENU_GAME = 0,	// �Q�[��
	MENU_RETRY,		// ���g���C
	MENU_TITLE,		// �^�C�g��
	MENU_MAX
}MENU;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nSelectMenu;	// �I�΂�Ă��郁�j���[
int	s_nIdxMenu;		// �g���Ă��郁�j���[�̔ԍ�
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
//==================================================
namespace
{
void Input(void);
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPause(void)
{
	s_nSelectMenu = 0;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPause(void)
{
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePause(void)
{
	// ����
	Input();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawPause(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetPause(void)
{
	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = MENU_LEFT;
	menu.fRight = MENU_RIGHT;
	menu.fTop = MENU_TOP;
	menu.fBottom = MENU_BOTTOM;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	menu.texture[MENU_GAME] = TEXTURE_Back_to_game;
	menu.texture[MENU_RETRY] = TEXTURE_Retry;
	menu.texture[MENU_TITLE] = TEXTURE_Back_to_title;
	
	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = FRAME_COLOR;
	Frame.texture = TEXTURE_NONE;

	// ���j���[�̐ݒ�
	s_nIdxMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetPause(void)
{
	// ���j���[�̃��Z�b�g
	ResetMenu(s_nIdxMenu);
}

namespace
{
//--------------------------------------------------
// ����
//--------------------------------------------------
void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W))
	{// W�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);

	}
	else if (GetKeyboardTrigger(DIK_S))
	{// S�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAME:		// �Q�[��
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
			break;

		case MENU_RETRY:	// ���g���C
			ChangeMode(MODE_GAME);
			break;

		case MENU_TITLE:	// �^�C�g��
			ChangeMode(MODE_TITLE);
			break;

		default:
			assert(false);
			break;
		}

		// �I�����̌���
		DecisionOption();
	}
}
}// namespace�͂����܂�
