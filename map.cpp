//================================
//
// �}�b�v�I����ʂ̏���
// Author:Sato Teruto
// Author:Seino Riku
//
//================================

//------------------------------
// �C���N���[�h
//------------------------------
#include "map.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"
#include "number.h"
#include "mode.h"
#include "sound.h"
#include "cursor.h"

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // �}�N����`
 //------------------------------
#define MAX_MAP			(3)				//���[���̍ő吔
#define MAX_TEXTURE		(3)				//�e�N�X�`���̍ő吔
#define MAX_FLASH		(80)			//�_�ł̉�������
#define HALF_FLASH		(MAX_FLASH / 2)	//�_�ł̐؂�ւ�����
#define MAP_WIDTH		(350.0f)		//�}�b�v�̕�
#define MAP_HEIGHT		(150.0f)		//�}�b�v�̍���
#define NAME_WIDTH		(300.0f)		//���O�̕�
#define NAME_HEIGHT		(125.0f)		//���O�̍���
#define SELECT_WIDTH	(250.0f)		//�I���̕�
#define SELECT_HEIGHT	(50.0f)			//�I���̍���
#define CURSOR_SIZE		(75.0f)			//�J�[�\���̃T�C�Y

 //------------------------------
 // �X�^�e�B�b�N�ϐ�
 //------------------------------
static TEXTURE		s_Texture[MAX_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
static Map s_Map[MAX_MAP];							//���[���\���̂̎擾
static MAPBG s_MAPBG;								//�w�i�\���̂̎擾
static int s_nIdx;									//��`�̃C���f�b�N�X
static int s_nIdxName[MAX_MAP];						//���O�̋�`�̃C���f�b�N�X
static int s_nFlashTime;							//�_�ł̎���
static int s_nSelect;								//�I�𒆂̔ԍ�
static int s_nIdxCursor;							//�J�[�\���̔z��̃C���f�b�N�X

//============================
// ���[���I����ʂ̏�����
//============================
void InitMap(void)
{
	// ��`�̏�����
	InitRectangle();

	//------------------------------
	// �e�N�X�`���̎擾
	//------------------------------
	s_Texture[0] = TEXTURE_Stage01;
	s_Texture[1] = TEXTURE_Stage02;
	s_Texture[2] = TEXTURE_Stage03;

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	//���[���\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		Map->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		Map->fWidth = 0.0f;	//��
		Map->fHeight = 0.0f;	//����
		Map->bUse = false;		//�g�p���Ă��Ȃ����
	}

	s_MAPBG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_MAPBG.fWidth = 0.0f;
	s_MAPBG.fHeight = 0.0f;
	s_MAPBG.bUse = false;

	SetBGMap(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	{// �I��
		// ��`�̐ݒ�
		s_nIdx = SetRectangle(TEXTURE_Map_Title);

		D3DXVECTOR3 size = D3DXVECTOR3(SELECT_WIDTH, SELECT_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SELECT_WIDTH * 0.5f, SELECT_HEIGHT * 0.5f, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdx, pos, size);
	}

	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.25f, 0.0f));
	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.5f, 0.0f));
	SetMap(D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.75f, 0.0f));

	{// ���O
		TEXTURE aTexture[MAX_MAP];

		aTexture[0] = TEXTURE_Stage1;
		aTexture[1] = TEXTURE_Stage2;
		aTexture[2] = TEXTURE_Stage3;

		for (int i = 0; i < MAX_MAP; i++)
		{
			// ��`�̐ݒ�
			s_nIdxName[i] = SetRectangle(aTexture[i]);

			D3DXVECTOR3 size = D3DXVECTOR3(NAME_WIDTH, NAME_HEIGHT, 0.0f);
			D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, (SCREEN_HEIGHT * 0.25f) + ((SCREEN_HEIGHT * 0.25f) * i), 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(s_nIdxName[i], pos, size);
		}
	}

	{// �J�[�\��
		// �J�[�\��������
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MAX_MAP;
		cursor.fPosX = SCREEN_WIDTH * 0.15f;
		cursor.fTop = 0.0f;
		cursor.fBottom = SCREEN_HEIGHT;
		cursor.fWidth = CURSOR_SIZE;
		cursor.fHeight = CURSOR_SIZE;
		cursor.texture = TEXTURE_Cursor_Right;
		cursor.nSelect = s_nSelect;
		cursor.bRotation = false;

		// �J�[�\���̐ݒ�
		s_nIdxCursor = SetCursor(cursor);
	}
}

//============================
// �}�b�v�I����ʂ̏I��
//============================
void UninitMap(void)
{
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		// ��`���g���̂��~�߂�
		StopUseRectangle(Map->nIdx);
	}

	// ��`���g���̂��~�߂�
	StopUseRectangle(s_MAPBG.nIdx);
	StopUseRectangle(s_nIdx);

	// �J�[�\���̏I��
	UninitCursor();
}

//============================
// �}�b�v�I����ʂ̍X�V
//============================
void UpdateMap(void)
{
	s_nFlashTime++;				//�^�C���̉��Z
	s_nFlashTime %= MAX_FLASH;	//�^�C���̏�����

	//�e�N�X�`���̓_��
	FlashTextureMap(ChangeSelectMap());

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		//���̍Đ�
		PlaySound(SOUND_LABEL_ENTER);

		//�^�C�g���ɖ߂�
		ChangeMode(MODE_TITLE);
	}
}

//============================
// �}�b�v�I����ʂ̕`��
//============================
void DrawMap(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//============================
// �}�b�v�I����ʂ̐ݒ�
//============================
void SetMap(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		if (Map->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
			Map->pos = pos;
			Map->fWidth = MAP_WIDTH;
			Map->fHeight = MAP_HEIGHT;
			Map->bUse = true;

			// ��`�̐ݒ�
			Map->nIdx = SetRectangle(s_Texture[nCnt]);

			D3DXVECTOR3 size = D3DXVECTOR3(Map->fWidth, Map->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(Map->nIdx, pos, size);
			break;
		}
	}
}

//============================
// �w�i�̐ݒ�
//============================
void SetBGMap(D3DXVECTOR3 pos)
{
	if (s_MAPBG.bUse == false)
	{//�g�p���Ă��Ȃ��Ȃ�
	 //�\���̂̐ݒ�
		s_MAPBG.pos = pos;
		s_MAPBG.fWidth = SCREEN_WIDTH;
		s_MAPBG.fHeight = SCREEN_HEIGHT;
		s_MAPBG.bUse = true;

		// ��`�̐ݒ�
		s_MAPBG.nIdx = SetRectangle(TEXTURE_Select_BG);

		D3DXVECTOR3 size = D3DXVECTOR3(s_MAPBG.fWidth, s_MAPBG.fHeight, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_MAPBG.nIdx, pos, size);
	}
}

//============================
// �e�N�X�`���̓_��
//============================
void FlashTextureMap(int nNumber)
{
	//------------------------------
	// ���ڂ̃e�N�X�`��
	//------------------------------
	if (s_nFlashTime >= HALF_FLASH)
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Map[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Map[nNumber].nIdx, GetColor(COLOR_WHITE));
	}
}

//============================
// �I��ԍ��̐؂�ւ�
//============================
int ChangeSelectMap(void)
{
	// ��`�̐F�̐ݒ�
	SetColorRectangle(s_Map[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//W�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 1 && s_nSelect <= MAX_MAP)
		{//0�����ɂȂ�Ȃ��Ȃ�
			//���̍Đ�
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect--;

			// �J�[�\���̈ʒu�̕ύX
			ChangePosCursor(s_nIdxCursor, s_nSelect);
		}
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//S�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 0 && s_nSelect < (MAX_MAP - 1))
		{//�ő吔�𒴂��Ȃ�Ȃ��Ȃ�
		 //���̍Đ�
			PlaySound(SOUND_LABEL_SELECT);

			s_nSelect++;

			// �J�[�\���̈ʒu�̕ύX
			ChangePosCursor(s_nIdxCursor, s_nSelect);
		}
	}

	return s_nSelect;
}

//============================
// �I��ԍ��̎擾
//============================
int GetSelectMap(void)
{
	return s_nSelect;
}
