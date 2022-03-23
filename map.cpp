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

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // �}�N����`
 //------------------------------
#define MAX_MAP	(3)						//���[���̍ő吔
#define MAX_TEXTURE	(3)					//�e�N�X�`���̍ő吔
#define MAX_FLASH	(80)				//�_�ł̉�������
#define HALF_FLASH	(MAX_FLASH / 2)		//�_�ł̐؂�ւ�����

 //------------------------------
 // �X�^�e�B�b�N�ϐ�
 //------------------------------
static TEXTURE		s_Texture[MAX_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
static Map s_Map[MAX_MAP];							//���[���\���̂̎擾
static MAPBG s_MAPBG;								//�w�i�\���̂̎擾
static int s_nFlashTime;							//�_�ł̎���
static int s_nSelect;								//�I�𒆂̔ԍ�
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
	s_Texture[0] = TEXTURE_Stage01;		//����
	s_Texture[1] = TEXTURE_Stage01;		//�|�C���g��
	s_Texture[2] = TEXTURE_Stage01;		//�Z�b�g��

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

	// ���̏�����
	InitNumber();
}

//============================
// �}�b�v�I����ʂ̏I��
//============================
void UninitMap(void)
{
	// ���̏I��
	UninitNumber();

	for (int nCnt = 0; nCnt < MAX_MAP; nCnt++)
	{
		Map *Map = s_Map + nCnt;

		// ��`���g���̂��~�߂�
		StopUseRectangle(Map->nIdx);
	}
	// ��`���g���̂��~�߂�
	StopUseRectangle(s_MAPBG.nIdx);
}

//============================
// �}�b�v�I����ʂ̍X�V
//============================
void UpdateMap(void)
{
	s_nFlashTime++;				//�^�C���̉��Z
	s_nFlashTime %= MAX_FLASH;	//�^�C���̏�����

	//�I��ԍ��̐؂�ւ�
	int nNumber = ChangeSelectMap();

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{//�G���^�[�L�[�������ꂽ�Ƃ�
	 //�^�C�g���ɖ߂�
		ChangeMode(MODE_TITLE);
	}
	//�e�N�X�`���̓_��
	FlashTextureMap(nNumber);

	//// ���̍X�V
	//UpdateNumber();
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
			Map->fWidth = 250.0f;
			Map->fHeight = 100.0f;
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
		s_MAPBG.nIdx = SetRectangle(s_Texture[5]);

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

	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
	{//A�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 1 && s_nSelect <= MAX_MAP)
		{//0�����ɂȂ�Ȃ��Ȃ�
			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 0 && s_nSelect < (MAX_MAP - 1))
		{//�ő吔�𒴂��Ȃ�Ȃ��Ȃ�
			s_nSelect++;
		}
	}

	return s_nSelect;
}