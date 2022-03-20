//================================
//
// �L�����I����ʂ̏���
// Author:HAMADA
//
//================================

//------------------------------
// �C���N���[�h
//------------------------------
#include "playerselect.h"
#include "player.h"
#include "input.h"
#include "rectangle.h"
#include "main.h"
#include "mode.h"
#include <stdio.h>
#include <assert.h>

//==================================================
// ��`
//==================================================

 //------------------------------
 // �}�N����`
 //------------------------------
#define MAX_CHARACTER	(4)					//�L�����̍ő吔


 //------------------------------
 // �X�^�e�B�b�N�ϐ�
 //------------------------------
int s_nSelect;
LPDIRECT3DTEXTURE9 s_pTexture[MAX_CHARACTER];
//============================
// �L�����I����ʂ̏�����
//============================
void InitCharacter(void)
{
	LoadFileSet("data\\txt\\Status.txt");

	// ��`�̏�����
	InitRectangle();

	//Player
	InitPlayer();

	SetPlayer(D3DXVECTOR3((float)PLAYERSIZE_X , (float)SCREEN_HEIGHT * 0.6f, 0.0f), 0, true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.6f, 0.0f), 1, false);
}

//============================
// �L�����I����ʂ̏I��
//============================
void UninitCharacter(void)
{

	UninitPlayer();
	// ��`�̏I��
	UninitRectangle();
}

//============================
// �L�����I����ʂ̍X�V
//============================
void UpdateCharacter(void)
{
	{
		Player*player = GetPlayer();

		if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT))
		{//A�L�[�������ꂽ�Ƃ�
		 //���l�̌��Z
			player->nType++;

			if (player->nType > 3)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//D�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z
			player->nType--;
			if (player->nType < 0)
			{
				player->nType = 3;
				
			}
		}

		//�e�N�X�`���X�V
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);
	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_LEFT))
		{//A�L�[�������ꂽ�Ƃ�
		 //���l�̌��Z
			player->nType++;
			if (player->nType > 3)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_NUMPAD3) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//D�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z
			player->nType--;
			
			if (player->nType < 0)
			{
				player->nType = 3;
			}
		}

		//�e�N�X�`���X�V
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{	//�Q�[���I����ʍs��
		ChangeMode(MODE_GAME);		
	}
}

//============================
// �L�����I����ʂ̕`��
//============================
void DrawCharacter(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//----------------------------
//Player�̃X�e�[�^�X�ǂݍ���
//----------------------------
void LoadFileSet(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");
	int number = 0;

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//���ǂݍ��ނ��
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//�f�o�C�X�̎擾
				pDevice = GetDevice();
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTexture[Num_Tex]);
				Num_Tex++;

			}
			
		}
		//�t�@�C�������
		fclose(pFile);
	}
}

