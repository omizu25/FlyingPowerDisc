//================================
//
// ���[���I����ʂ̏���
// Author:Sato Teruto
//
//================================

//------------------------------
// �C���N���[�h
//------------------------------
#include "rule.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"
#include "number.h"
#include "mode.h"
#include "sound.h"

#include <stdio.h>
#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	START_TIME = 60;					// �^�C���̎n�܂�̒l
const int	START_POINT = 21;					// �|�C���g�̎n�܂�̒l
const int	START_SET = 2;						// �Z�b�g�̎n�܂�̒l
const int	CHANGE_TIME = 30;					// �^�C���̕ύX�l
const int	CHANGE_POINT = 3;					// �|�C���g�̕ύX�l
const int	CHANGE_SET = 1;						// �Z�b�g�̕ύX�l
const int	MAX_TIME = 90;						// �^�C���̍ő�l
const int	MAX_POINT = 24;						// �|�C���g�̍ő�l
const int	MAX_SET = 3;						// �Z�b�g�̍ő�l
const int	MIN_TIME = 30;						// �^�C���̍ŏ��l
const int	MIN_POINT = 18;						// �|�C���g�̍ŏ��l
const int	MIN_SET = 1;						// �Z�b�g�̍ŏ��l
const float	NUMBER_WIDTH = 30.0f;				// ���̕�
const float	NUMBER_HEIGHT = 100.0f;				// ���̍���
const char	*RULE_FILE = "data/txt/Rule.txt";	// ���[���̃t�@�C��

typedef enum
{
	OPTION_TIME = 0,	// �^�C��
	OPTION_POINT,		// �|�C���g
	OPTION_SET,			// �Z�b�g
	OPTION_MAX
}OPTION;
}// namespace�͂����܂�

//------------------------------
// �}�N����`
//------------------------------
#define MAX_RULE	(3)					//���[���̍ő吔
#define MAX_SWITCH	(6)					//�X�C�b�`�̍ő吔
#define HALF_SWITCH	(MAX_SWITCH / 2)	//�X�C�b�`�̔���
#define MAX_TEXTURE	(6)					//�e�N�X�`���̍ő吔
#define MAX_FLASH	(80)				//�_�ł̉�������
#define HALF_FLASH	(MAX_FLASH / 2)		//�_�ł̐؂�ւ�����

//------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------
static TEXTURE		s_Texture[MAX_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^

//�\����
static Rule s_Rule[MAX_RULE];		//���[���\���̂̎擾
static Switch s_Switch[MAX_SWITCH];	//�X�C�b�`�\���̂̎擾
static BG s_BG;						//�w�i�\���̂̎擾

//�l
static int s_nFlashTime;				//�_�ł̎���
static int s_nSelect;					//�I�𒆂̔ԍ�
static int s_nOption[OPTION_MAX];		// �I�����̒l
static int s_nNumberIdx[OPTION_MAX];	// �I�����̒l�̃C���f�b�N�X

//============================
// ���[���I����ʂ̏�����
//============================
void InitRule(void)
{
	// ��`�̏�����
	InitRectangle();

	//------------------------------
	// �e�N�X�`���̎擾
	//------------------------------
	s_Texture[0] = TEXTURE_Select_Time;			//����
	s_Texture[1] = TEXTURE_Select_Point;		//�|�C���g��
	s_Texture[2] = TEXTURE_Select_Set;			//�Z�b�g��
	s_Texture[3] = TEXTURE_Select_Left;			//���{�^��
	s_Texture[4] = TEXTURE_Select_Right;		//�E�{�^��
	s_Texture[5] = TEXTURE_Select_BG;			//�w�i

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	//���[���\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		rule->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		rule->fWidth = 0.0f;	//��
		rule->fHeight = 0.0f;	//����
		rule->bUse = false;		//�g�p���Ă��Ȃ����
	}

	//�X�C�b�`�\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		Switch->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		Switch->fWidth = 0.0f;		//��
		Switch->fHeight = 0.0f;		//����
		Switch->bUse = false;		//�g�p���Ă��Ȃ����
	}

	s_BG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_BG.fWidth = 0.0f;
	s_BG.fHeight = 0.0f;
	s_BG.bUse = false;

	// ���̏�����
	InitNumber();
}

//============================
// ���[���I����ʂ̏I��
//============================
void UninitRule(void)
{
	//���̒�~
	StopSound();

	// ���̏I��
	UninitNumber();

	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		// ��`���g���̂��~�߂�
		StopUseRectangle(rule->nIdx);
	}

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		// ��`���g���̂��~�߂�
		StopUseRectangle(Switch->nIdx);
	}

	// ��`���g���̂��~�߂�
	StopUseRectangle(s_BG.nIdx);
}

//============================
// ���[���I����ʂ̍X�V
//============================
void UpdateRule(void)
{
	s_nFlashTime++;				//�^�C���̉��Z
	s_nFlashTime %= MAX_FLASH;	//�^�C���̏�����

	//�I��ԍ��̐؂�ւ�
	int nNumber = ChangeSelect();

	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
	{//A�L�[�������ꂽ�Ƃ�
		//���l�̌��Z
		SubRule(nNumber);

		// �Z�[�u
		SaveRule();
	}

	if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ�Ƃ�
		//���l�̉��Z
		AddRule(nNumber);

		// �Z�[�u
		SaveRule();
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		 //���̍Đ�
		PlaySound(SOUND_LABEL_ENTER);

		 //�^�C�g���ɖ߂�
		ChangeMode(MODE_TITLE);
	}
	//�e�N�X�`���̓_��
	FlashTexture(nNumber);

	// ���̍X�V
	UpdateNumber();
}

//============================
// ���[���I����ʂ̕`��
//============================
void DrawRule(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//============================
// ���[���I����ʂ̐ݒ�
//============================
void SetRule(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
			//�\���̂̐ݒ�
			rule->pos = pos;
			rule->fWidth = 400.0f;
			rule->fHeight = 100.0f;
			rule->bUse = true;

			// ��`�̐ݒ�
			rule->nIdx = SetRectangle(s_Texture[nCnt]);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(rule->nIdx, pos, size);

			//�؂�ւ��{�^���̐ݒ�
			SetSwitchLeft(rule->pos);	//��
			SetSwitchRight(rule->pos);	//�E

			// ����
			int nDigit = DigitNumber(s_nOption[nCnt]);

			D3DXVECTOR3 posNumber = pos + D3DXVECTOR3(450.0f + (NUMBER_WIDTH * (nDigit * 0.5f)), 0.0f, 0.0f);
			D3DXVECTOR3 sizeNumber = D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f);

			// ���̐ݒ�
			s_nNumberIdx[nCnt] = SetNumber(posNumber, sizeNumber, GetColor(COLOR_RED), s_nOption[nCnt]);

			break;
		}
	}
}

//============================
// �؂�ւ��{�^��(��)�̐ݒ�
//============================
void SetSwitchLeft(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < HALF_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
			//�\���̂̐ݒ�
			Switch->pos = D3DXVECTOR3(pos.x + 300.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// ��`�̐ݒ�
			Switch->nIdx = SetRectangle(s_Texture[3]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// �؂�ւ��{�^��(�E)�̐ݒ�
//============================
void SetSwitchRight(D3DXVECTOR3 pos)
{
	for (int nCnt = HALF_SWITCH; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
			Switch->pos = D3DXVECTOR3(pos.x + 600.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// ��`�̐ݒ�
			Switch->nIdx = SetRectangle(s_Texture[4]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// �w�i�̐ݒ�
//============================
void SetBG(D3DXVECTOR3 pos)
{
	if (s_BG.bUse == false)
	{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
		s_BG.pos = pos;
		s_BG.fWidth = SCREEN_WIDTH;
		s_BG.fHeight = SCREEN_HEIGHT;
		s_BG.bUse = true;

		// ��`�̐ݒ�
		s_BG.nIdx = SetRectangle(s_Texture[5]);

		D3DXVECTOR3 size = D3DXVECTOR3(s_BG.fWidth, s_BG.fHeight, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_BG.nIdx, pos, size);
	}
}

//============================
// �e�N�X�`���̓_��
//============================
void FlashTexture(int nNumber)
{
	//------------------------------
	// ���ڂ̃e�N�X�`��
	//------------------------------
	if (s_nFlashTime >= HALF_FLASH)
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Rule[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Rule[nNumber].nIdx, GetColor(COLOR_WHITE));
	}

	//�I���{�^���̐F�̏�����
	SetColorRectangle(s_Switch[nNumber].nIdx, GetColor(COLOR_WHITE));
	SetColorRectangle(s_Switch[nNumber + 3].nIdx, GetColor(COLOR_WHITE));

	//------------------------------
	// ���I��
	//------------------------------
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
	{//A�L�[�������ꂽ�Ƃ�
	 //���̍Đ�
		PlaySound(SOUND_LABEL_SELECT);

		if (s_nFlashTime >= HALF_FLASH)
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber].nIdx, GetColor(COLOR_WHITE));
		}
	}

	//------------------------------
	// �E�I��
	//------------------------------
	if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ�Ƃ�
	 //���̍Đ�
		PlaySound(SOUND_LABEL_SELECT);

		if (s_nFlashTime >= HALF_FLASH)
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, GetColor(COLOR_WHITE));
		}
	}
}

//============================
// ���l�̉��Z
//============================
void AddRule(int nNumber)
{
	if (nNumber == 0)
	{
		if (s_nOption[nNumber] < MAX_TIME)
		{
			s_nOption[nNumber] += CHANGE_TIME;	//���Ԃ̑���
		}
	}
	else if (nNumber == 1)
	{
		if (s_nOption[nNumber] < MAX_POINT)
		{
			s_nOption[nNumber] += CHANGE_POINT;	//�|�C���g�̑���
		}
	}
	else if (nNumber == 2)
	{
		if (s_nOption[nNumber] < MAX_SET)
		{
			s_nOption[nNumber] += CHANGE_SET;	//�Z�b�g���̑���
		}
	}

	// ���̕ύX
	ChangeNumber(s_nNumberIdx[nNumber], s_nOption[nNumber]);
}

//============================
// ���l�̌��Z
//============================
void SubRule(int nNumber)
{
	if (nNumber == 0)
	{
		if (s_nOption[nNumber] > MIN_TIME)
		{
			s_nOption[nNumber] -= CHANGE_TIME;	//���Ԃ̌���
		}
	}
	else if (nNumber == 1)
	{
		if (s_nOption[nNumber] > MIN_POINT)
		{
			s_nOption[nNumber] -= CHANGE_POINT;	//�|�C���g�̌���
		}
	}
	else if (nNumber == 2)
	{
		if (s_nOption[nNumber] > MIN_SET)
		{
			s_nOption[nNumber] -= CHANGE_SET;	//�Z�b�g���̌���
		}
	}

	// ���̕ύX
	ChangeNumber(s_nNumberIdx[nNumber], s_nOption[nNumber]);
}

//============================
// �I��ԍ��̐؂�ւ�
//============================
int ChangeSelect(void)
{
	// ��`�̐F�̐ݒ�
	SetColorRectangle(s_Rule[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//W�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 1 && s_nSelect <= MAX_RULE)
		{//0�����ɂȂ�Ȃ��Ȃ�
		 //���̍Đ�
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//S�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 0 && s_nSelect < (MAX_RULE - 1))
		{//�ő吔�𒴂��Ȃ�Ȃ��Ȃ�
		 //���̍Đ�
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect++;
		}
	}

	return s_nSelect;
}

//============================
// �ǂݍ���
//============================
void LoadRule(void)
{
	FILE *pFile;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(RULE_FILE, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &s_nOption[OPTION_TIME]);
		fscanf(pFile, "%d", &s_nOption[OPTION_POINT]);
		fscanf(pFile, "%d", &s_nOption[OPTION_SET]);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//============================
// �Z�[�u
//============================
void SaveRule(void)
{
	FILE *pFile;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(RULE_FILE, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_TIME]);
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_POINT]);
		fprintf(pFile, "%d\n\n", s_nOption[OPTION_SET]);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//============================
// ���Z�b�g�̃Z�[�u
//============================
void ResetSaveRule(void)
{
	FILE *pFile;	// �t�@�C���|�C���^��錾

					// �t�@�C�����J��
	pFile = fopen(RULE_FILE, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fprintf(pFile, "%d\n\n", START_TIME);
		fprintf(pFile, "%d\n\n", START_POINT);
		fprintf(pFile, "%d\n\n", START_SET);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//============================
// ���Ԃ̎擾
//============================
int GetTimeRule(void)
{
	return s_nOption[OPTION_TIME];
}

//============================
// �|�C���g���̎擾
//============================
int GetPointRule(void)
{
	return s_nOption[OPTION_POINT];
}

//============================
// �Z�b�g���̎擾
//============================
int GetSetRule(void)
{
	return s_nOption[OPTION_SET];
}