//====================
//�v���C���[�ݒ�
//autho hamada ryuuga
//====================

#include"main.h"
#include"Player.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include "rectangle.h"
#include <stdio.h>

#define MAXPLAYER (2)//�o��ő吔
#define MAXPLAYERTYPE (4)//Type�ő吔
#define MOVESPEED (5.0f)
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //�e�N�X�`���̃|�C���^
static Player s_Player[MAXPLAYER];//�v���C���[�\���̎擾
static Player s_PlayerType[MAXPLAYERTYPE];//�v���C���[��Type��ۑ�����


//=======================
//�v���C���[�̏������ݒ�
//=======================
void InitPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^

							  //�f�o�C�X�̎擾
	pDevice = GetDevice();

	LoadFile("data\\txt\\Status.txt");

	for (int count = 0; count < MAXPLAYER; count++)
	{
		
		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bUse = false;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;

		// ��`�̐ݒ�
		s_Player[count].nIdx = SetRectangle(s_pTexturePlayer[count]);
	}
	SetPlayer(D3DXVECTOR3((float)0, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 0,true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 1,false);
}

//===================
//�I������
//===================
void UninitPlayer(void)
{
	//�T�E���h��~
	StopSound();

	//�e�N�X�`���̔j��
	for (int Tex = 0; Tex < MAXPLAYERTYPE; Tex++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexturePlayer[Tex] != NULL)
		{
			s_pTexturePlayer[Tex]->Release();
			s_pTexturePlayer[Tex] = NULL;
		}
	}

	for (int count = 0; count < MAXPLAYER; count++)
	{
		// �g���̂��~�߂�
		StopUseRectangle(s_Player[count].nIdx);
	}
}

//===================
//�X�V����
//===================
void UpdatePlayer(void)
{
	MovePlayer();

	for (int count = 0; count < MAXPLAYER; count++)
	{
		Player *pPlayer = &s_Player[count];

		//�ړ��ʂ��X�V(����������)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��											  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��

			//�O��̈ʒu�̕ۑ�
			s_Player[count].posOld = s_Player[count].pos;
			//�X�V
			s_Player[count].pos.x += s_Player[count].move.x;
			s_Player[count].pos.y += s_Player[count].move.y;

			//��---------------------------------------------------
			if (s_Player[count].pos.x <= 0.0f + PLAYERMOVE)
			{//���ǁi���j
				 s_Player[count].pos.x = 0.0f + PLAYERMOVE;
			}
			else if (s_Player[count].pos.x >= SCREEN_WIDTH - PLAYERMOVE)
			{//���ǁi�E�j
				s_Player[count].pos.x = SCREEN_WIDTH - PLAYERMOVE;
			}
			if (s_Player[count].pos.y <= 0.0f + PLAYERMOVE)
			{//��ǁ@
				s_Player[count].pos.y = 0.0f + PLAYERMOVE;
			}
			if (s_Player[count].pos.y >= SCREEN_HEIGHT - PLAYERMOVE)
			{//����
				s_Player[count].pos.y = SCREEN_HEIGHT - PLAYERMOVE;
			}
			//�^�񒆃��C��
			if (s_Player[0].pos.x >= SCREEN_WIDTH* 0.5f - PLAYERMOVE)
			{
				s_Player[0].pos.x = SCREEN_WIDTH* 0.5f - PLAYERMOVE;
			}
			if (s_Player[1].pos.x <= SCREEN_WIDTH * 0.5f + PLAYERMOVE)
			{
				s_Player[1].pos.x = SCREEN_WIDTH * 0.5f + PLAYERMOVE;
			}
			
			// ��`�̉�]����ʒu�̐ݒ�
			SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}
}
//===================
//�`�揈��
//===================
void DrawPlayer(void)
{
}
void SetPlayer(D3DXVECTOR3 pos, int nType,bool light)
{
	for (int count = 0; count< MAXPLAYER; count++)
	{
		Player *pPlayer = &s_Player[count];

		if (s_Player[count].bUse)
		{
			continue;
		}
		
		s_Player[count].nType = nType;
		s_Player[count].bUse = true;
		s_Player[count].pos = pos;

		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = s_PlayerType[nType].Speed;
		s_Player[count].Pow = s_PlayerType[nType].Pow;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;
	
		if (light)
		{
			D3DXVECTOR2 texU(1.0f, 0.0f);
			D3DXVECTOR2 texV(0.0f, 1.0f);

			// ��`�̃e�N�X�`�����W�̐ݒ�
			SetTexRectangle(pPlayer->nIdx, texU, texV);
		}
		else
		{
			D3DXVECTOR2 texU(0.0f, 1.0f);
			D3DXVECTOR2 texV(0.0f, 1.0f);

			// ��`�̃e�N�X�`�����W�̐ݒ�
			SetTexRectangle(pPlayer->nIdx, texU, texV);
		}

		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
		
		break;
	}
}
//------------------------------------
//���S�_�^�񒆂�POS�Z�b�g
//------------------------------------
void SetUp(VERTEX_2D *pVtx,
	float posx,		//���S�_��
	float posy,		//���S�_��
	float rotPlayer,		//�v���C���[��]
	float fAngleAttack 		//�Ίp���p�x
	, float fLengthAttack)	//�Ίp���m����
{
	//��]
	pVtx[0].pos.x = posx + sinf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.y = posy + cosf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = posx + sinf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.y = posy + cosf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = posx + sinf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.y = posy + cosf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = posx + sinf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.y = posy + cosf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.z = 0.0f;

}
//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//Auther�Fhamada ryuuga
//---------------------------------------
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}
void LoadFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");
	int nCnt = 0;

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
					&s_pTexturePlayer[Num_Tex]);
				Num_Tex++;
				
			}
			if (strcmp(&s_aString[0], "STATUSSET") == 0)
			{
				while(1)
				{
					fscanf(pFile, "%s", &s_aString[0]);

					if (strcmp(&s_aString[0], "ATTACKPOW") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_PlayerType[nCnt].Pow);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_PlayerType[nCnt].Speed);
					}
					if (strcmp(&s_aString[0], "ENDSET") == 0)
					{
						nCnt++;
						break;
					}
				}
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
}
//----------------------------
//Player����
//----------------------------
void MovePlayer(void)
{
	//---------------------------------------
	//�P�̖ڂ̍s��
	//----------------------------------------
	if (GetKeyboardPress(DIK_W))
	{
		s_Player[0].move.y = -s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_A))
	{
		s_Player[0].move.x = -s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_S))
	{
		s_Player[0].move.y = s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_D))
	{
		s_Player[0].move.x = s_Player[0].Speed;
	}
	if (GetKeyboardPress(DIK_SPACE))
	{//�����ɋʓ����铮��i�p���[���ʂ̑��x�ɂ���Ƃ����񂶂�Ȃ����ȁj
		
	}
	//---------------------------------------
	//�Q�̖ڂ̍s��
	//----------------------------------------
	if (GetKeyboardPress(DIK_UP))
	{
		s_Player[1].move.y = -s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		s_Player[1].move.x = -s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		s_Player[1].move.y = s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		s_Player[1].move.x = s_Player[1].Speed;
	}
	if (GetKeyboardPress(DIK_RETURN))
	{//�����ɋʓ����铮��i�p���[���ʂ̑��x�ɂ���Ƃ����񂶂�Ȃ����ȁj

	}

}

//----------------------------
//Player�����擾
//----------------------------
Player* GetPlayer(void)
{
	return s_Player;
}