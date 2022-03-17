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
#include "texture.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_LIGHT = 2;			// ����̍ő吔
const float	TITLE_POS_Y = 200.0f;	// �^�C�g����Y�̈ʒu
const float	TITLE_WIDTH = 900.0f;	// �^�C�g���̕�
const float	TITLE_HEIGHT = 200.0f;	// �^�C�g���̍���
const float	LIGHT_WIDTH = 560.0f;	// ���C�g�̕�
const float	LIGHT_HEIGHT = 560.0f;	// ���C�g�̍���
const float	CHANGE_SPEED = 0.005f;	// ���k�̑��x
const float	CHANGE_AMOUNT = 0.1f;	// �ω���
const float	MEDIAN_LENGTH = 0.95f;	// �����̒����l
const float	MAX_ROTATION = 0.003f;	// ��]�̍ő�l
const float	DECREASE_SIZE = 0.6f;	// �T�C�Y�̌�������
const float	DECREASE_SPEED = 0.5f;	// ���x�̌�������
const float	MENU_WIDTH = 540.0f;	// ���j���[�̕�
const float	MENU_HEIGHT = 150.0f;	// ���j���[�̍���

typedef enum
{
	MENU_GAME = 0,	// �Q�[��
	MENU_RULE,		// ���[��
	MENU_PLAYER,	// �v���C���[�I��
	MENU_MAX
}MENU;

typedef struct
{
	D3DXVECTOR3	rot;		// ����
	float		fWidth;		// ��
	float		fHeight;	// ����
	float		fSpeed;		// ���x
	int			nIdx;		// ��`�̃C���f�b�N�X
}Light;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int		s_nIdxBG;			// �w�i�̋�`�̃C���f�b�N�X
int		s_nIdx;				// ��`�̃C���f�b�N�X
Light	s_light[MAX_LIGHT];	// ����̏��
int		s_nFlashTime;		// ����
int		s_nSelectMenu;		// �I�΂�Ă��郁�j���[
int		s_nIdxUseMenu;		// �g���Ă��郁�j���[�̔ԍ�
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
//==================================================
namespace
{
void UpdateLight(void);
void Input(void);
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTitle(void)
{
	// ��`�̏�����
	InitRectangle();

	s_nFlashTime = 0;
	s_nSelectMenu = 0;

	// ��`�̐ݒ�
	s_nIdxBG = SetRectangle(TEXTURE_BG);

	// ��`�̐ݒ�
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		pLight->nIdx = SetRectangle(TEXTURE_TitleLight_red);
	}

	// ��`�̐ݒ�
	s_nIdx = SetRectangle(TEXTURE_Title_blue);

	{// �w�i
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// ���S
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdx, pos, size);
	}

	{// ���
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			Light *pLight = &s_light[i];

			pLight->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (i == 0)
			{//	��
				pLight->fWidth = LIGHT_WIDTH;
				pLight->fHeight = LIGHT_HEIGHT;
				pLight->fSpeed = MAX_ROTATION * DECREASE_SPEED;
			}
			else if (i == 1)
			{// ��O
				pLight->fWidth = LIGHT_WIDTH * DECREASE_SIZE;
				pLight->fHeight = LIGHT_HEIGHT * DECREASE_SIZE;
				pLight->fSpeed = MAX_ROTATION;
			}

			// ��`�̉�]����ʒu�̐ݒ�
			SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, pLight->fWidth, pLight->fHeight);

			D3DXVECTOR2 texU = D3DXVECTOR2(0.0f + (i * 1.0f), 1.0f + (i * -1.0f));
			D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

			// ��`�̃e�N�X�`�����W�̐ݒ�
			SetTexRectangle(pLight->nIdx, texU, texV);
		}
	}

	{// ���j���[
		// ���j���[�̏�����
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = SCREEN_WIDTH * 0.25f;
		menu.fRight = SCREEN_WIDTH * 0.75f;
		menu.fTop = SCREEN_HEIGHT * 0.5f;
		menu.fBottom = SCREEN_HEIGHT;
		menu.fWidth = MENU_WIDTH;
		menu.fHeight = MENU_HEIGHT;
		menu.bSort = true;

		menu.texture[MENU_GAME] = TEXTURE_Game_Start;
		menu.texture[MENU_RULE] = menu.texture[MENU_GAME];
		menu.texture[MENU_PLAYER] = menu.texture[MENU_GAME];

		FrameArgument Frame;
		Frame.bUse = true;
		Frame.col = GetColor(COLOR_WHITE);
		Frame.texture = TEXTURE_Frame;

		// ���j���[�̐ݒ�
		s_nIdxUseMenu = SetMenu(menu, Frame);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTitle(void)
{
	// ��`�̏I��
	UninitRectangle();

	// ���j���[�̏I��
	UninitMenu();

	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		StopUseRectangle(pLight->nIdx);
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateTitle(void)
{
	// ���
	UpdateLight();

	// ����
	Input();

	// ���j���[�̍X�V
	UpdateMenu();
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
// ���
//--------------------------------------------------
void UpdateLight(void)
{
	s_nFlashTime++;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
	float fCurve = sinf((s_nFlashTime * CHANGE_SPEED) * (D3DX_PI * 2.0f));

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		if (i == 0)
		{// ��
			pLight->rot.z += pLight->fSpeed;
		}
		else if (i == 1)
		{// ��O
			pLight->rot.z += -pLight->fSpeed;
		}

		float fRatio = (fCurve * CHANGE_AMOUNT) + MEDIAN_LENGTH;
		float fWidth = pLight->fWidth * fRatio;
		float fHeight = pLight->fHeight * fRatio;

		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, fWidth, fHeight);
	}
}

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

	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
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
		case MENU_GAME:	// �Q�[��
			ChangeMode(MODE_GAME);
			break;

		case MENU_RULE:	// �Q�[��
			ChangeMode(MODE_RULE);
			break;

		case MENU_PLAYER:	// �v���C���[�I��
			ChangeMode(MODE_PLAYER);
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
