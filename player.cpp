//====================
//�v���C���[�ݒ�
//autho hamada ryuuga
//====================

#include "main.h"
#include "Player.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "rectangle.h"
#include "wall.h"
#include "game.h"
#include <stdio.h>
#include <assert.h>

#define MAXPLAYERTYPE (4)//Type�ő吔
#define MOVESPEED (5.0f)
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //�e�N�X�`���̃|�C���^
static Player s_Player[MAXPLAYER];//�v���C���[�\���̎擾
static Player s_PlayerType[MAXPLAYERTYPE];//�v���C���[��Type��ۑ�����

// �v���g�^�C�v�錾
static void UpdateNormal(void);

//=======================
//�v���C���[�̏������ݒ�
//=======================
void InitPlayer(void)
{
	LoadFile("data\\txt\\Status.txt");

	for (int count = 0; count < MAXPLAYER; count++)
	{
		
		s_Player[count].state = PLAYESTATE_NORMAL;
		s_Player[count].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[count].nLife = 5;
		s_Player[count].Speed = 0;
		s_Player[count].bHave = false;
		s_Player[count].bUse = false;
		s_Player[count].fheight = PLAYERSIZ_Y;
		s_Player[count].fwidth = PLAYERSIZ_X;

		// ��`�̐ݒ�
		s_Player[count].nIdx = SetRectangleWithTex(s_pTexturePlayer[count]);
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
	switch (GetGameState())
	{
	case GAMESTATE_START:	// �J�n���
		break;

	case GAMESTATE_NORMAL:	// �ʏ���
		UpdateNormal();
		break;

	case GAMESTART_RESET:	// ���Z�b�g���

		break;

	case GAMESTATE_END:		// �I�����

		break;
	case GAMESTATE_RESULT:	// ���U���g���

		break;

	case GAMESTATE_NONE:	// �������Ă��Ȃ����
	default:
		assert(false);
		break;
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
		s_Player[count].have = false;
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

#if 0	// �����g���ĂȂ���B
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
#endif

void LoadFile(char *Filename)
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
						fscanf(pFile, "%f", &s_PlayerType[number].Pow);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_PlayerType[number].Speed);
					}
					if (strcmp(&s_aString[0], "ENDSET") == 0)
					{
						number++;
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
		s_Player[0].have = false;
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
		s_Player[0].have = false;
	}

}
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Size, int number)
{
	bool bIsLanding = false;

	/*	float vecA = ;
		float vecB;*/
	if (pPos->y + Size / 2 > s_Player[number].pos.y + PLAYERSIZ_Y/2,
		pPos->y - Size / 2 < s_Player[number].pos.y + PLAYERSIZ_Y / 2)
	{

		if (pPos->x + Size / 2 > s_Player[number].pos.x - PLAYERSIZ_X / 2
			&& pPosOld->x + Size / 2 <= s_Player[number].pos.x + PLAYERSIZ_X / 2)
		{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//��
			pPos->x = s_Player[number].pos.x;
			pPos->y = s_Player[number].pos.y;
			bIsLanding = true;
			s_Player[number].have = true;
		}
		if (pPos->x - Size / 2 < s_Player[number].pos.x + PLAYERSIZ_X / 2
			&& pPosOld->x - Size / 2 >= s_Player[number].pos.x - PLAYERSIZ_X / 2)
		{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//�E
			pPos->x = s_Player[number].pos.x;
			pPos->y = s_Player[number].pos.y;
			bIsLanding = true;
			s_Player[number].have = true;
		}
	}

	return bIsLanding;
}

//----------------------------
//Player�����擾
//----------------------------
Player* GetPlayer(void)
{
	return s_Player;
}

//----------------------------
// �ʏ��Ԃ̍X�V
//----------------------------
static void UpdateNormal(void)
{
	MovePlayer();

	for (int count = 0; count < MAXPLAYER; count++)
	{
		Player *pPlayer = &s_Player[count];

		CollisionWall(&s_Player[1].pos, &s_Player[1].posOld);


		CollisionWall(&s_Player[0].pos, &s_Player[0].posOld);
		//�ړ��ʂ��X�V(����������)
		s_Player[count].move.x += (0.0f - s_Player[count].move.x)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��											  
		s_Player[count].move.y += (0.0f - s_Player[count].move.y)*0.2f;//�i�ړI�̒l-���݂̒l�j�������W��

			//�O��̈ʒu�̕ۑ�
		s_Player[count].posOld = s_Player[count].pos;
		//�X�V
		s_Player[count].pos.x += s_Player[count].move.x;
		s_Player[count].pos.y += s_Player[count].move.y;

		//��---------------------------------------------------
		if (s_Player[count].pos.x <= 0.0f + pPlayer->fwidth / 2.0f)
		{//���ǁi���j
			s_Player[count].pos.x = 0.0f + pPlayer->fwidth / 2.0f;
		}
		else if (s_Player[count].pos.x >= SCREEN_WIDTH - pPlayer->fwidth / 2.0f)
		{//���ǁi�E�j
			s_Player[count].pos.x = SCREEN_WIDTH - pPlayer->fwidth / 2.0f;
		}
		if (s_Player[count].pos.y <= 0.0f + pPlayer->fheight / 2.0f)
		{//��ǁ@
			s_Player[count].pos.y = 0.0f + pPlayer->fheight / 2.0f;
		}
		if (s_Player[count].pos.y >= SCREEN_HEIGHT - pPlayer->fheight / 2.0f)
		{//����
			s_Player[count].pos.y = SCREEN_HEIGHT - pPlayer->fheight / 2.0f;
		}
		//�^�񒆃��C��
		if (s_Player[0].pos.x >= SCREEN_WIDTH* 0.5f - pPlayer->fwidth / 2.0f)
		{
			s_Player[0].pos.x = SCREEN_WIDTH* 0.5f - pPlayer->fwidth / 2.0f;
		}
		if (s_Player[1].pos.x <= SCREEN_WIDTH * 0.5f + pPlayer->fwidth / 2.0f)
		{
			s_Player[1].pos.x = SCREEN_WIDTH * 0.5f + pPlayer->fwidth / 2.0f;
		}

		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}
}
