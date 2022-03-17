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

//============================
// �L�����I����ʂ̏�����
//============================
void InitCharacter(void)
{


	// ��`�̏�����
	InitRectangle();

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
			player->nIdx++;
			if (player->nType > 4)
			{
				player->nType = 0;
			}

		}

		if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//D�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z

			player->nType--;
			player->nIdx--;
			if (player->nType < -1)
			{
				player->nType = 4;
				
			}
		}

	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_LEFT))
		{//A�L�[�������ꂽ�Ƃ�
		 //���l�̌��Z
			player->nType++;
			if (player->nType > 4)
			{
				player->nType = 0;
			}

		}

		if (GetKeyboardTrigger(DIK_NUMPAD3) || GetJoypadTrigger(JOYKEY_RIGHT))
		{//D�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z
			player->nType--;
			if (player->nType < -1)
			{
				player->nType = 4;
			}

		}
		D3DXVECTOR2 texU(1.0f, 0.0f);
		D3DXVECTOR2 texV(0.0f, 1.0f);

		// ��`�̃e�N�X�`�����W�̐ݒ�
		SetTexRectangle(player->nIdx, texU, texV);
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



