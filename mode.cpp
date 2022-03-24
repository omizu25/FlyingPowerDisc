//**************************************************
//
// FPD���� ( mode.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "fade.h"
#include "game.h"
#include "mode.h"
#include "map.h"
#include "title.h"
#include "rule.h"
#include "playerselect.h"
#include "tutorial.h"
#include "texture.h"

#include <assert.h>

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
MODE	s_mode = MODE_NONE;		// ���݂̃��[�h
MODE	s_modeNext = MODE_NONE;	// ���̃��[�h
}// namesapce�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMode(void)
{

}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMode(void)
{
	// �^�C�g���̏I��
	UninitTitle();

	//�}�b�v�I���̏I��
	UninitMap();

	// �Q�[���̏I��
	UninitGame();

	// ���[���I����ʂ̏I��
	UninitRule();

	// �L�����I����ʂ̏I��
	UninitCharacter();

	// �`���[�g���A���̏I��
	UninitTutorial();

	// ���[���I����ʂ̃��Z�b�g�̃Z�[�u
	ResetSaveRule();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:	// �^�C�g��
		UpdateTitle();
		break;

	case MODE_MAP:		// �}�b�v
		UpdateMap();
		break;

	case MODE_RULE:		// ���[��
		UpdateRule();
		break;

	case MODE_GAME:		// �Q�[��
		UpdateGame();
		break;

	case MODE_PLAYER:	// �v���C���[�I��
		UpdateCharacter();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A��
		UpdateTutorial();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:	// �^�C�g��
		DrawTitle();
		break;

	case MODE_MAP:		// �}�b�v
		DrawMap();
		break;

	case MODE_RULE:		// ���[��
		DrawRule();
		break;

	case MODE_GAME:		// �Q�[��
		DrawGame();
		break;

	case MODE_PLAYER:	// �v���C���[�I��
		DrawCharacter();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A��
		DrawTutorial();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// ���̃��[�h�����܂��ĂȂ�
		return;
	}

	if (GetFade() == FADE_NONE)
	{// �������Ă��Ȃ���ԂȂ�
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// �t�F�[�h�C����Ԃ���Ȃ�
		return;
	}

	switch (s_mode)
	{// ���݂̃��[�h�̏I��
	case MODE_TITLE:	// �^�C�g��
		UninitTitle();
		break;

	case MODE_MAP:		// �}�b�v
		UninitMap();
		break;

	case MODE_RULE:		// ���[��
		UninitRule();
		break;

	case MODE_GAME:		// �Q�[��
		UninitGame();
		break;

	case MODE_PLAYER:	// �v���C���[�I��
		UninitCharacter();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A��
		UninitTutorial();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}

	switch (s_modeNext)
	{// ���̃��[�h�̏�����
	case MODE_TITLE:	// �^�C�g��
		InitTitle();
		SetLightRed(D3DXVECTOR3(80.0f, 150.0f, 0.0f), TEXTURE_Light_Red);		//�����
		SetLightRed(D3DXVECTOR3(330.0f, 550.0f, 0.0f), TEXTURE_Light_Red);		//�E����
		SetLightBlue(D3DXVECTOR3(560.0f, 350.0f, 0.0f), TEXTURE_Light_Blue);	//�E���
		SetLightBlue(D3DXVECTOR3(70.0f, 450.0f, 0.0f), TEXTURE_Light_Blue);		//������
		break;

	case MODE_MAP:		// �}�b�v
		InitMap();
		SetBGMap(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		SetMap(D3DXVECTOR3(200.0f, 300.0f, 0.0f));
		SetMap(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
		SetMap(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
		break;

	case MODE_RULE:		// ���[��
		LoadRule();
		InitRule();

		// ���[���I����ʂ̐ݒ�
		SetBG(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 150.0f, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 350.0f, 0.0f));
		SetRule(D3DXVECTOR3(400.0f, 550.0f, 0.0f));
		break;

	case MODE_GAME:		// �Q�[��
		InitGame();
		break;

	case MODE_PLAYER:	// �v���C���[�I��
		InitCharacter();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A��
		InitTutorial();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_mode = s_modeNext;	// ���݂̉��(���[�h)��؂�ւ���
	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// �ύX
//--------------------------------------------------
void ChangeMode(MODE modeNext)
{
	assert(modeNext >= 0 && modeNext < MODE_MAX);

	s_modeNext = modeNext;
}
