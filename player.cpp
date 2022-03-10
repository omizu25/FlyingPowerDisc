//====================
//�v���C���[�ݒ�
//autho hamada ryuuga
//====================

#include"main.h"
#include"Player.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include <stdio.h>

#define MAXPLAYER (2)
#define MAXPLAYERTYPE (4)
#define MOVESPEED (5.0f)
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPlayer = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3	s_rotPlayer;	//����
static float s_fLengthPlayer;      //�Ίp���m����
static float s_fAnglePlayer;		//�Ίp���p�x
static float s_fLengthAttack;      //�Ίp���m����
static float s_fAngleAttack;		//�Ίp���p�x
static Player s_Player[MAXPLAYER];//�v���C���[�\���̎擾
static Player s_PlayerType[MAXPLAYERTYPE];//�v���C���[�\���̎擾Type��


//=======================
//�v���C���[�̏������ݒ�
//=======================
void InitPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^

							  //�f�o�C�X�̎擾
	pDevice = GetDevice();

	LoadFile("data\\txt\\Status.txt");

	//�Ίp���̒������Z�o����
	s_fLengthAttack = sqrtf(PLAYERSIZ_X * PLAYERSIZ_X + PLAYERSIZ_Y * PLAYERSIZ_Y) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	s_fAngleAttack = atan2f(PLAYERSIZ_X, PLAYERSIZ_Y);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAXPLAYER *4,//�����͊m�ۂ���o�b�t�@�̃T�C�Ysizeof(VERTEX_2D)*�ł����͂��܂�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffPlayer,
		NULL);

	for (int count = 0; count < MAXPLAYER; count++)
	{
		
		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bUse = false;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;

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


	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPlayer != NULL)
	{
		s_pVtxBuffPlayer->Release();
		s_pVtxBuffPlayer = NULL;
	}

}

//===================
//�X�V����
//===================
void UpdatePlayer(void)
{

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);


	for (int count = 0; count < MAXPLAYER; count++,pVtx += 4)
	{
	

		if (count == 0)
		{
			//�s��
			if (GetKeyboardPress(DIK_W))
			{
				s_Player[0].move.y = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_A))
			{
				s_Player[0].move.x = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_S))
			{
				s_Player[0].move.y = s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_D))
			{
				s_Player[0].move.x = s_Player[count].Speed;
			}

		}
		else
		{
			if (GetKeyboardPress(DIK_UP))
			{
				s_Player[1].move.y = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				s_Player[1].move.x = -s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				s_Player[1].move.y = s_Player[count].Speed;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				s_Player[1].move.x = s_Player[count].Speed;
			}
		}

		//�ړ��ʂ��X�V(����������)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��											  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��

			//�O��̈ʒu�̕ۑ�
			s_Player[count].posOld = s_Player[count].pos;
			//�X�V
			s_Player[count].pos.x += s_Player[count].move.x;
			s_Player[count].pos.y += s_Player[count].move.y;

			//��
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
			
			SetUp(pVtx,
				s_Player[count].pos.x,		//�v���C���[���S�_��
				s_Player[count].pos.y,		//�v���C���[���S�_��
				s_rotPlayer.z,		//�v���C���[��]
				s_fAngleAttack, 		//�Ίp���p�x
				s_fLengthAttack);	//�Ίp���m�����������Z�o����
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPlayer->Unlock();
}
//===================
//�`�揈��
//===================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int count = 0; count < MAXPLAYER; count++)
	{
		if (s_Player[count].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexturePlayer[s_Player[count].nType]); //);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, count*4, 2);

		}
	}
}
void SetPlayer(D3DXVECTOR3 pos, int nType,bool light)
{
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count< MAXPLAYER; count++, pVtx += 4)
	{
		if (s_Player[count].bUse)
		{
			continue;
		}
		
		s_Player[count].nType = nType;
		s_Player[count].bUse = true;
		s_Player[count].pos = pos;

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = s_PlayerType[nType].Speed;
		s_Player[count].Pow = s_PlayerType[nType].Pow;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;
	
		if (light)
		{
			//�e�N�X�`���̍��W�ݒ�
			SetTex2d(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

		}
		else
		{//�e�N�X�`���̍��W�ݒ�
			SetTex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);
			

		}
		SetUp(pVtx,
			s_Player[count].pos.x,		//�v���C���[���S�_��
			s_Player[count].pos.y,		//�v���C���[���S�_��
			s_rotPlayer.z,		//�v���C���[��]
			s_fAngleAttack, 		//�Ίp���p�x
			s_fLengthAttack);	//�Ίp���m�����������Z�o����
		break;
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPlayer->Unlock();

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
	int     MoveSet = 0;

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
