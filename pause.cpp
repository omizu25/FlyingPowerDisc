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
// �}�N����`
//==================================================
#define MENU_WIDTH		(500.0f)								// ���j���[�̕�
#define MENU_HEIGHT		(150.0f)								// ���j���[�̍���
#define MENU_LEFT		(SCREEN_WIDTH * 0.25f)					// ���j���[�̍��[
#define MENU_RIGHT		(SCREEN_WIDTH * 0.75f)					// ���j���[�̉E�[
#define MENU_TOP		(SCREEN_HEIGHT * 0.1f)					// ���j���[�̏�[
#define MENU_BOTTOM		(SCREEN_HEIGHT * 0.9f)					// ���j���[�̉��[
#define BG_COLOR		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))		// �w�i�̐F
#define FRAME_COLOR		(D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f))		// �t���[���̐F

//==================================================
// �񋓌^
//==================================================
typedef enum
{
	MENU_GAME = 0,		// �Q�[��
	MENU_RETRY,			// ���g���C
	MENU_TITLE,			// �^�C�g��
	MENU_MAX
}MENU;

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static int		s_nSelectMenu;		// �I�΂�Ă��郁�j���[
static int		s_nIdxMenu;			// �g���Ă��郁�j���[�̔ԍ�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Input(void);

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

	menu.pTexture[MENU_GAME] = GetTexture(TEXTURE_Back_to_game);
	menu.pTexture[MENU_RETRY] = GetTexture(TEXTURE_Retry);
	menu.pTexture[MENU_TITLE] = GetTexture(TEXTURE_Back_to_title);
	
	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = FRAME_COLOR;
	Frame.pTexture = NULL;

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

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
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
		case MENU_GAME:			// �Q�[��
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
			break;

		case MENU_RETRY:		// ���g���C
			ChangeMode(MODE_GAME);
			break;

		case MENU_TITLE:		// �^�C�g��
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
