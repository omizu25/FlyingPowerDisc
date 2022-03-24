//**************************************************
//
// �^�C�g����� ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "rectangle.h"
#include "title.h"
#include "color.h"
#include "sound.h"
#include "texture.h"
#include "cursor.h"
#include "player.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_LIGHT = 2;			// ����̍ő吔
const float	TITLE_WIDTH = 600.0f;	// �^�C�g���̕�
const float	TITLE_HEIGHT = 600.0f;	// �^�C�g���̍���
const float	MENU_WIDTH = 300.0f;	// ���j���[�̕�
const float	MENU_HEIGHT = 80.0f;	// ���j���[�̍���
const float	CURSOR_SIZE = 50.0f;	// �J�[�\���̃T�C�Y

typedef enum
{
	MENU_GAME = 0,	// �Q�[��
	MENU_RULE,		// ���[��
	MENU_MAP,		// �}�b�v�I��
	MENU_PLAYER,	// �v���C���[�I��
	MENU_TUTORIAL,	// �`���[�g���A��
	MENU_MAX
}MENU;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nIdxBG;		// �w�i�̋�`�̃C���f�b�N�X
int	s_nIdx;			// ��`�̃C���f�b�N�X
int	s_nSelectMenu;	// �I�΂�Ă��郁�j���[
int	s_nIdxUseMenu;	// �g���Ă��郁�j���[�̔ԍ�
int	s_nIdxCursor;	// �J�[�\���̔z��̃C���f�b�N�X
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
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_TITLE2);

	// ��`�̏�����
	InitRectangle();

	s_nSelectMenu = 0;

	// ��`�̐ݒ�
	s_nIdxBG = SetRectangle(TEXTURE_BG);

	// ��`�̐ݒ�
	s_nIdx = SetRectangle(TEXTURE_Title_blue);

	{// �w�i
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// ���S
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdx, pos, size);
	}

	// �v���C���[�̏�����
	InitPlayer();

	// �v���C���[�̐ݒ�
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.775f, 0.0f), 0, true, PLAYERSIZE * 2.0f);

	{// ���j���[
		// ���j���[�̏�����
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = SCREEN_WIDTH * 0.55f;
		menu.fRight = SCREEN_WIDTH * 0.95f;
		menu.fTop = 0.0f;
		menu.fBottom = SCREEN_HEIGHT;
		menu.fWidth = MENU_WIDTH;
		menu.fHeight = MENU_HEIGHT;
		menu.bSort = true;

		menu.texture[MENU_GAME] = TEXTURE_Game_Start;
		menu.texture[MENU_RULE] = TEXTURE_Rule_Select;
		menu.texture[MENU_MAP] = TEXTURE_Map_Select;
		menu.texture[MENU_PLAYER] = TEXTURE_Char_Select;
		menu.texture[MENU_TUTORIAL] = TEXTURE_Tutorial_Start;

		FrameArgument Frame;
		Frame.bUse = true;
		Frame.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		Frame.texture = TEXTURE_NONE;

		// ���j���[�̐ݒ�
		s_nIdxUseMenu = SetMenu(menu, Frame);
	}

	{// �J�[�\��
		// �J�[�\��������
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MENU_MAX;
		cursor.fPosX = SCREEN_WIDTH * 0.595f;
		cursor.fTop = 0.0f;
		cursor.fBottom = SCREEN_HEIGHT;
		cursor.fWidth = CURSOR_SIZE;
		cursor.fHeight = CURSOR_SIZE;
		cursor.texture = TEXTURE_Disc;
		cursor.nSelect = s_nSelectMenu;
		cursor.bRotation = true;

		// �J�[�\���̐ݒ�
		s_nIdxCursor = SetCursor(cursor);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTitle(void)
{
	//�T�E���h��~
	StopSound();

	// ��`�̏I��
	UninitRectangle();

	// ���j���[�̏I��
	UninitMenu();

	// �J�[�\���̏I��
	UninitCursor();

	// �v���C���[�̏I��
	UninitPlayer();

	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateTitle(void)
{
	// ����
	Input();

	// ���j���[�̍X�V
	UpdateMenu();

	// �J�[�\���̍X�V
	UpdateCursor();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawTitle(void)
{
	// ��`�̕`��
	DrawRectangle();
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

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{// W�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);

		// �J�[�\���̈ʒu�̕ύX
		ChangePosCursor(s_nIdxCursor, s_nSelectMenu);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{// S�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);

		// �J�[�\���̈ʒu�̕ύX
		ChangePosCursor(s_nIdxCursor, s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAME:	// �Q�[��
			ChangeMode(MODE_GAME);
			break;

		case MENU_RULE:	// ���[��
			ChangeMode(MODE_RULE);
			break;

		case MENU_MAP:	// �}�b�v�I��
			ChangeMode(MODE_MAP);
			break;

		case MENU_PLAYER:	// �v���C���[�I��
			ChangeMode(MODE_PLAYER);
			break;

		case MENU_TUTORIAL:	// �`���[�g���A��
			ChangeMode(MODE_TUTORIAL);
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
