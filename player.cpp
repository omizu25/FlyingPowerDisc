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
#define MOVESPEED (5.0f)
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTexturePlayer = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPlayer = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3	s_rotPlayer;	//����
static float s_fLengthPlayer;      //�Ίp���m����
static float s_fAnglePlayer;		//�Ίp���p�x
static float s_fLengthAttack;      //�Ίp���m����
static float s_fAngleAttack;		//�Ίp���p�x
static Player s_Player[MAXPLAYER];//�v���C���[�\���̎擾


//=======================
//�v���C���[�̏������ݒ�
//=======================
void InitPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^

							  //�f�o�C�X�̎擾
	pDevice = GetDevice();


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

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

					//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count< MAXPLAYER; count++,pVtx += 4)
	{
		s_Player[count].pos = D3DXVECTOR3((float)SCREEN_WIDTH*count, (float)SCREEN_HEIGHT * 0.5f, 0.0f);


		SetUp(pVtx,
			s_Player[count].pos.x,		//�v���C���[���S�_��
			s_Player[count].pos.y,		//�v���C���[���S�_��
			s_rotPlayer.z,		//�v���C���[��]
			s_fAngleAttack, 		//�Ίp���p�x
			s_fLengthAttack);	//�Ίp���m�����������Z�o����

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
		s_Player[count].rotstate = PLAYEROTSTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bUse = true;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth	= PLAYERSIZ_X;
		s_Player[count].MapMove = 0.0f;

	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPlayer->Unlock();

}

//===================
//�I������
//===================
void UninitPlayer(void)
{
	//�T�E���h��~
	StopSound();

	//�e�N�X�`���̔j��

	//�e�N�X�`���̔j��
	if (s_pTexturePlayer != NULL)
	{
		s_pTexturePlayer->Release();
		s_pTexturePlayer = NULL;
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


#ifdef _DEBUG


#endif // DEBUG

	for (int count = 0; count < MAXPLAYER; count++,pVtx += 4)
	{
		switch (s_Player[count].state)
		{
			//�m�[�}��------------------------
		case PLAYESTATE_NORMAL:

			break;

			//�_���[�W---------------------------
		case PLAYESTATE_DAMAGE:

			break;

			//�A�^�b�N---------------
		case PLAYESTATE_ATTACK:

			break;
		}
		//�ړ��ʂ��X�V(����������)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��
																	  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��
		if (count == 0)
		{
			//�s��
			if (GetKeyboardPress(DIK_W))
			{
				s_Player[0].move.y = -MOVESPEED;
			}
			if (GetKeyboardPress(DIK_A))
			{
				s_Player[0].move.x = -MOVESPEED;
			}
			if (GetKeyboardPress(DIK_S))
			{
				s_Player[0].move.y = MOVESPEED;
			}
			if (GetKeyboardPress(DIK_D))
			{
				s_Player[0].move.x = +MOVESPEED;
			}
		}
		else
		{
			if (GetKeyboardPress(DIK_UP))
			{
				s_Player[1].move.y = -MOVESPEED;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				s_Player[1].move.x = -MOVESPEED;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				s_Player[1].move.y = MOVESPEED;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				s_Player[1].move.x = +MOVESPEED;
			}
		}

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
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^

							  //���
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int count = 0; count < MAXPLAYER; count++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);


		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, count*4, 2);
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

//void LoadFile(char *Filename)
//{
//	char	s_aString[256];//
//	int		Num_Tex = 0;
//	int     MoveSet = 0;
//
//	// �t�@�C���|�C���^�̐錾
//	FILE* pFile;
//
//	//�t�@�C�����J��
//	pFile = fopen(Filename, "r");
//	int nCntEnemy = 0;
//
//	if (pFile != NULL)
//	{//�t�@�C�����J�����ꍇ
//		fscanf(pFile, "%s", &s_aString);
//
//		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
//		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
//			s_aString[0] = {};
//			fscanf(pFile, "%s", &s_aString[0]);
//		}
//		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
//		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���
//
//			fscanf(pFile, "%s", &s_aString[0]);
//
//			if (strncmp(&s_aString[0], "#", 1) == 0)
//			{//����̂��ƃR�����g
//				fgets(&s_aString[0], sizeof(s_aString), pFile);
//				continue;
//			}
//
//			if (strcmp(&s_aString[0], "NUM_TEXTURE") == 0)
//			{
//				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
//
//				fscanf(pFile, "%d", &Num_Tex);
//
//			}
//			if (strcmp(&s_aString[0], "DIVISION") == 0)
//			{
//				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
//				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionX);
//				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionY);
//			}
//			if (strcmp(&s_aString[0], "ANIMESPEED") == 0)
//			{
//				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
//				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nSpeed);
//			}
//			if (strcmp(&s_aString[0], "MOVESPEED") == 0)
//			{
//				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
//				fscanf(pFile, "%f", &s_aTypeEnemy[nCntEnemy].fSpeed);
//			}
//			if (strcmp(&s_aString[0], "TRACKING") == 0)
//			{
//				int checker = 0;
//				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
//				fscanf(pFile, "%d", &checker);
//				if (checker == 1)
//				{
//					s_aTypeEnemy[nCntEnemy].bTracking = true;
//				}
//				else
//				{
//					s_aTypeEnemy[nCntEnemy].bTracking = false;
//				}
//			}
//		}
//
//}

