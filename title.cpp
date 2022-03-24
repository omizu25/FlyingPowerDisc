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
const int	MAX_LIGHT = 4;				// ���C�g�̍ő吔
const int	HALF_LIGHT = MAX_LIGHT / 2;	// ���C�g�̔���
const float	TITLE_WIDTH = 600.0f;		// �^�C�g���̕�
const float	TITLE_HEIGHT = 600.0f;		// �^�C�g���̍���
const float	MENU_WIDTH = 300.0f;		// ���j���[�̕�
const float	MENU_HEIGHT = 80.0f;		// ���j���[�̍���
const float	CURSOR_SIZE = 50.0f;		// �J�[�\���̃T�C�Y
const int	MAX_FLASH_RED = 80;						// �_�łɂ����鎞��(��)
const int	HALF_FLASH_RED = MAX_FLASH_RED / 2;		// �_�łɂ����鎞��(��)�̔���
const int	MAX_FLASH_BLUE = 120;					// �_�łɂ����鎞��(��)
const int	HALF_FLASH_BLUE = 80;	// �_�łɂ����鎞��(��)�̔���


typedef enum
{
	MENU_GAME = 0,	// �Q�[��
	MENU_RULE,		// ���[��
	MENU_MAP,		// �}�b�v�I��
	MENU_PLAYER,	// �v���C���[�I��
	MENU_TUTORIAL,	// �`���[�g���A��
	MENU_MAX
}MENU;

//���C�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩
	int	nIdx;			// ��`�̃C���f�b�N�X
}Light;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nIdxBG;				// �w�i�̋�`�̃C���f�b�N�X
int	s_nIdx;					// ��`�̃C���f�b�N�X
int	s_nSelectMenu;			// �I�΂�Ă��郁�j���[
int	s_nIdxUseMenu;			// �g���Ă��郁�j���[�̔ԍ�
int	s_nIdxCursor;			// �J�[�\���̔z��̃C���f�b�N�X
int s_nFlashTimeRed;		// �_�ŗp�̎���(��)
int s_nFlashTimeBlue;		// �_�ŗp�̎���(��)
Light s_Light[MAX_LIGHT];	// ���C�g�\���̂̎擾
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

	// ���C�g�\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		light->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		light->fWidth = 0.0f;	//��
		light->fHeight = 0.0f;	//����
		light->bUse = false;	//�g�p���Ă��Ȃ����
	}

	s_nSelectMenu = 0;

	{// �w�i
		// ��`�̐ݒ�
		s_nIdxBG = SetRectangle(TEXTURE_BG);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	// ���C�g�̐ݒ�
	SetLightRed(D3DXVECTOR3(80.0f, 100.0f, 0.0f), TEXTURE_Light_Red);		//�����
	SetLightRed(D3DXVECTOR3(330.0f, 550.0f, 0.0f), TEXTURE_Light_Red);		//�E����
	SetLightBlue(D3DXVECTOR3(545.0f, 350.0f, 0.0f), TEXTURE_Light_Blue);	//�E���
	SetLightBlue(D3DXVECTOR3(70.0f, 450.0f, 0.0f), TEXTURE_Light_Blue);		//������

	{// ���S
		// ��`�̐ݒ�
		s_nIdx = SetRectangle(TEXTURE_Title_Logo);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdx, pos, size);
	}

	{// �v���C���[
		// �v���C���[�̏�����
		InitPlayer();

		Player *pPlayer = GetPlayer();

		// �v���C���[�̐ݒ�
		SetPlayer(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.815f, 0.0f), pPlayer->nType, true, PLAYERSIZE * 1.4f);
		
		pPlayer++;

		// �v���C���[�̐ݒ�
		SetPlayer(D3DXVECTOR3(SCREEN_WIDTH * 0.475f, SCREEN_HEIGHT * 0.815f, 0.0f), pPlayer->nType, false, PLAYERSIZE * 1.4f);
	}

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

	// ���C�g�̓_��
	FlashLight();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawTitle(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//--------------------------------------------------
// �ԃ��C�g�̐ݒ�
//--------------------------------------------------
void SetLightRed(D3DXVECTOR3 pos,TEXTURE texture)
{
	for (int nCnt = 0; nCnt < HALF_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		if (light->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
			light->pos = pos;
			light->fWidth = 100.0f;
			light->fHeight = 100.0f;
			light->bUse = true;

			// ��`�̐ݒ�
			light->nIdx = SetRectangle(texture);

			D3DXVECTOR3 size = D3DXVECTOR3(light->fWidth, light->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(light->nIdx, pos, size);

			break;
		}
	}
}

//--------------------------------------------------
// ���C�g�̐ݒ�
//--------------------------------------------------
void SetLightBlue(D3DXVECTOR3 pos, TEXTURE texture)
{
	for (int nCnt = HALF_LIGHT; nCnt < MAX_LIGHT; nCnt++)
	{
		Light *light = s_Light + nCnt;

		if (light->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
			light->pos = pos;
			light->fWidth = 100.0f;
			light->fHeight = 100.0f;
			light->bUse = true;

			// ��`�̐ݒ�
			light->nIdx = SetRectangle(texture);

			D3DXVECTOR3 size = D3DXVECTOR3(light->fWidth, light->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(light->nIdx, pos, size);

			break;
		}
	}
}

//--------------------------------------------------
// ���C�g�̓_��
//--------------------------------------------------
void FlashLight(void)
{
	s_nFlashTimeRed++;						//�^�C���̉��Z
	s_nFlashTimeRed %= MAX_FLASH_RED;		//�^�C���̏�����

	s_nFlashTimeBlue++;		//�^�C���̉��Z
	//�^�C���̏�����
	if (s_nFlashTimeBlue % MAX_FLASH_BLUE == 0)
	{
		s_nFlashTimeBlue = 40;
	}

	//------------------------------
	// �ԃ��C�g�̓_��
	//------------------------------
	if (s_nFlashTimeRed >= HALF_FLASH_RED)
	{
		SetColorRectangle(s_Light[0].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		SetColorRectangle(s_Light[1].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		SetColorRectangle(s_Light[0].nIdx, GetColor(COLOR_WHITE));
		SetColorRectangle(s_Light[1].nIdx, GetColor(COLOR_WHITE));
	}

	//------------------------------
	// ���C�g�̓_��
	//------------------------------
	if (s_nFlashTimeBlue >= HALF_FLASH_BLUE)
	{
		SetColorRectangle(s_Light[2].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		SetColorRectangle(s_Light[3].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		SetColorRectangle(s_Light[2].nIdx, GetColor(COLOR_WHITE));
		SetColorRectangle(s_Light[3].nIdx, GetColor(COLOR_WHITE));
	}
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
		PlaySound(SOUND_LABEL_SELECT);

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
		PlaySound(SOUND_LABEL_SELECT);

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
		PlaySound(SOUND_LABEL_ENTER);
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
