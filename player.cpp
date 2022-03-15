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
#include "effect.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h>

#define MAXPLAYERTYPE (4)//Type�ő吔
#define MOVESPEED (5.0f)
#define DEAD_ZONE	(0.1f)		// �X�e�B�b�N�̗V��
#define MAX_DIVECOUNT (15)		// �_�C�u�̍d������
#define MAX_RESET_SPEED (5.0f)	// ���Z�b�g��Ԃ̑���
#define MAX_HAVE_COUNT (120)	// �����Ă鎞�Ԃ̍ő�l
#define DISC_SPEED (2.0f)		// �f�B�X�N�̑���
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTexturePlayer[MAXPLAYERTYPE] = {}; //�e�N�X�`���̃|�C���^
static Player s_Player[MAXPLAYER];//�v���C���[�\���̎擾
static Player s_PlayerType[MAXPLAYERTYPE];//�v���C���[��Type��ۑ�����
static bool	s_bKeyBoardWASD;			// WASD�̃L�[�{�[�h���͂����邩�ǂ���
static bool	s_bKeyBoardArrow;			// �₶�邵�̃L�[�{�[�h���͂����邩�ǂ���
static bool	s_bJoyPad[MAXPLAYER];		// �W���C�p�b�h���͂����邩�ǂ���
static bool	s_bStickLeft[MAXPLAYER];	// ���X�e�B�b�N���͂����邩�ǂ���

// �X�^�e�B�b�N�֐��v���g�^�C�v�錾
static void UpdateNormal(void);
static void UpdateReset(void);
static void InputMove(void);

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
		s_Player[count].bDive = false;
		s_Player[count].fheight = PLAYERSIZE_Y;
		s_Player[count].fwidth = PLAYERSIZE_X;
		s_Player[count].nSkillCount = 0;
		s_Player[count].nDiveCount = 0;
		s_Player[count].nHaveCount = 0;
		// ��`�̐ݒ�
		s_Player[count].nIdx = SetRectangleWithTex(s_pTexturePlayer[count]);
	}
	SetPlayer(D3DXVECTOR3((float)PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 0,true);
	SetPlayer(D3DXVECTOR3((float)SCREEN_WIDTH - PLAYERSIZE_X, (float)SCREEN_HEIGHT * 0.5f, 0.0f), 1,false);
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

	case GAMESTATE_RESET:	// ���Z�b�g���
		UpdateReset();
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

//===================
//�Z�b�g
//===================
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
		s_Player[count].fheight = PLAYERSIZE_Y;
		s_Player[count].fwidth = PLAYERSIZE_X;
		s_Player[count].bHave = false;
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

//----------------------------
//Player�̃X�e�[�^�X�ǂݍ���
//----------------------------
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
	// �ړ��̓��͂����邩�Ȃ���
	InputMove();

	Disc *pDisc = GetDisc();

	
	//---------------------------------------
	//�P�̖ڂ̍s��
	//----------------------------------------
	if (!s_Player[0].bHave)
	{// �f�B�X�N�������Ă��Ȃ�
		
		if (s_Player[0].bDive == true && pDisc->nThrow == 1)
		{//�^�b�N���K�p��
			Player *pPlayer = &s_Player[0];

			float fHeight = ((PLAYERSIZE_Y * 0.5f));
			float fWidth = ((PLAYERSIZE_X * 0.5f));

			if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
				(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
				(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
				(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
			{// �v���C���[�Ƀf�B�X�N������������
				
				pDisc->nThrow = 0;
				s_Player[0].bDive = false;
				s_Player[0].nDiveCount = 0;
				pDisc->move = D3DXVECTOR3(DISC_SPEED, 0.0f, 0.0f)*s_Player[0].Pow * 3;
			}
		}
		
		if (s_Player[0].bDive == true)
		{
			s_Player[0].nDiveCount++;

			if (s_Player[0].nDiveCount >= MAX_DIVECOUNT)
			{
				s_Player[0].nDiveCount = 0;
				s_Player[0].bDive = false;
			}
		}
		else if (GetKeyboardTrigger(DIK_C) || GetJoypadIdxPress(JOYKEY_A, 0))
		{//�^�b�N��
			s_Player[0].pos.x += s_Player[0].Speed * 5;
			s_Player[0].bDive = true;
			s_Player[0].nDiveCount = 0;
		}
		else if (s_bKeyBoardWASD)
		{// �L�[�{�[�h
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetKeyboardPress(DIK_A))
			{// �L�[�������ꂽ
				vec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_D))
			{// �L�[�������ꂽ
				vec.x += 1.0f;
			}
			if (GetKeyboardPress(DIK_W))
			{// �L�[�������ꂽ
				vec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_S))
			{// �L�[�������ꂽ
				vec.y += 1.0f;
			}

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vec, &vec);

			s_Player[0].move = vec * s_Player[0].Speed;
		}
		else if (s_bJoyPad[0])
		{// �W���C�p�b�h
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetJoypadIdxPress(JOYKEY_LEFT, 0))
			{// �{�^���������ꂽ
				vec.x -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_RIGHT, 0))
			{// �{�^���������ꂽ
				vec.x += 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_UP, 0))
			{// �{�^���������ꂽ
				vec.y -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_DOWN, 0))
			{// �{�^���������ꂽ
				vec.y += 1.0f;
			}

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vec, &vec);

			s_Player[0].move = vec * s_Player[0].Speed;
		}
		else if (s_bStickLeft[0])
		{// ���X�e�B�b�N
			D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
			stick.y = GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&stick, &stick);

			s_Player[0].move = stick * s_Player[0].Speed;
		}
	}
	else
	{// �f�B�X�N�������Ă���
		s_Player[0].nHaveCount++;
		if (GetKeyboardPress(DIK_SPACE) || GetJoypadIdxPress(JOYKEY_A, 0)|| s_Player[0].nHaveCount >= MAX_HAVE_COUNT)
		{//�����ɋʓ����铮��i�p���[���ʂ̑��x�ɂ���Ƃ����񂶂�Ȃ����ȁj
			s_Player[0].bHave = false;
			pDisc->nThrow = 0;
			//�^�C�~���O�̂ɂ���đ��x�ς�����
			int Ross = s_Player[0].nHaveCount / 10;
			pDisc->move = D3DXVECTOR3(DISC_SPEED - Ross*0.1f, 0.0f, 0.0f)*s_Player[0].Pow;
			pDisc->bHave = false;
			s_Player[0].nHaveCount = 0;
		}
	}

	//---------------------------------------
	//�Q�̖ڂ̍s��
	//----------------------------------------
	if (!s_Player[1].bHave)
	{// �f�B�X�N�������Ă��Ȃ�
		if (s_Player[1].bDive == true && pDisc->nThrow == 0)
		{//�^�b�N���K�p��
			Player *pPlayer = &s_Player[1];

			float fHeight = ((PLAYERSIZE_Y * 0.5f));
			float fWidth = ((PLAYERSIZE_X * 0.5f));

			if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
				(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
				(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
				(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
			{// �v���C���[�Ƀf�B�X�N������������

				pDisc->nThrow = 1;
				s_Player[1].bDive = false;
				s_Player[1].nDiveCount = 0;
				pDisc->move = D3DXVECTOR3(-DISC_SPEED, 0.0f, 0.0f)*s_Player[1].Pow * 3;
			}
		}

		if (s_Player[1].bDive == true)
		{
			s_Player[1].nDiveCount++;

			if (s_Player[1].nDiveCount >= MAX_DIVECOUNT)
			{
				s_Player[1].nDiveCount = 0;
				s_Player[1].bDive = false;
			}
		}
		else if (GetKeyboardTrigger(DIK_L) || GetJoypadIdxPress(JOYKEY_A, 1))
		{//�^�b�N��
			s_Player[1].pos.x -= s_Player[1].Speed * 5;
			s_Player[1].bDive = true;
			s_Player[1].nDiveCount = 0;
		}
		else if (s_bKeyBoardArrow)
		{// �L�[�{�[�h
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetKeyboardPress(DIK_LEFT))
			{// �L�[�������ꂽ
				vec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{// �L�[�������ꂽ
				vec.x += 1.0f;
			}
			if (GetKeyboardPress(DIK_UP))
			{// �L�[�������ꂽ
				vec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{// �L�[�������ꂽ
				vec.y += 1.0f;
			}

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vec, &vec);

			s_Player[1].move = vec * s_Player[1].Speed;
		}
		else if (s_bJoyPad[1])
		{// �W���C�p�b�h
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetJoypadIdxPress(JOYKEY_LEFT, 1))
			{// �{�^���������ꂽ
				vec.x -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_RIGHT, 1))
			{// �{�^���������ꂽ
				vec.x += 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_UP, 1))
			{// �{�^���������ꂽ
				vec.y -= 1.0f;
			}
			if (GetJoypadIdxPress(JOYKEY_DOWN, 1))
			{// �{�^���������ꂽ
				vec.y += 1.0f;
			}

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vec, &vec);

			s_Player[1].move = vec * s_Player[1].Speed;
		}
		else if (s_bStickLeft[1])
		{// ���X�e�B�b�N
			D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 1).x;
			stick.y = GetJoypadStick(JOYKEY_LEFT_STICK, 1).y;

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&stick, &stick);

			s_Player[1].move = stick * s_Player[1].Speed;
		}
	}
	else
	{// �f�B�X�N�������Ă���
		s_Player[1].nHaveCount++;
		if (GetKeyboardPress(DIK_RETURN) || GetJoypadIdxPress(JOYKEY_A, 1) || s_Player[1].nHaveCount >= MAX_HAVE_COUNT)
		{//�����ɋʓ����铮��i�p���[���ʂ̑��x�ɂ���Ƃ����񂶂�Ȃ����ȁj
			s_Player[1].bHave = false;
			pDisc->nThrow = 1;
			//�^�C�~���O�̂ɂ���đ��x�ς�����
			int Ross = s_Player[1].nHaveCount / 10;
			pDisc->move = D3DXVECTOR3(-DISC_SPEED + Ross*0.1f, 0.0f, 0.0f) * s_Player[1].Pow;
			pDisc->bHave = false;
			s_Player[1].nHaveCount = 0;
		}
	}
}

//----------------------------
//Player�����蔻��
//----------------------------
bool CollisionPlayer(Disc *pDisc, float Size, int number)
{
	bool bIsLanding = false;
	Player *pPlayer = &s_Player[number];

	float fDiscSize = Size * 0.0f;
	float fHeight = (fDiscSize + (PLAYERSIZE_Y * 0.5f));
	float fWidth = (fDiscSize + (PLAYERSIZE_X * 0.5f));

	if ((pDisc->pos.y <= (pPlayer->pos.y + fHeight)) &&
		(pDisc->pos.y >= (pPlayer->pos.y - fHeight)) &&
		(pDisc->pos.x <= (pPlayer->pos.x + fWidth)) &&
		(pDisc->pos.x >= (pPlayer->pos.x - fWidth)))
	{// �v���C���[�Ƀf�B�X�N������������
		switch (number)
		{
		case 0:
			pDisc->pos.x = s_Player[number].pos.x + fWidth;
			break;

		case 1:

			pDisc->pos.x = s_Player[number].pos.x - fWidth;
			break;

		default:
			assert(false);
			break;
		}
		pDisc->pos.y = s_Player[number].pos.y;
		pDisc->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pDisc->bHave = true;
		s_Player[number].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Player[number].nSkillCount++;
		if (s_Player[number].nSkillCount >= 10)
		{
			s_Player[number].nSkillCount = 0;
			s_Player[number].bSkill = true;
		}
		s_Player[number].bHave = true;
	
		bIsLanding = true;
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
		if (s_Player[count].pos.y <= 0.0f + pPlayer->fheight / 2.0f+ WALLSIZ_Y /2)
		{//��ǁ@
			s_Player[count].pos.y = 0.0f + pPlayer->fheight / 2.0f + WALLSIZ_Y / 2;
		}
		if (s_Player[count].pos.y >= SCREEN_HEIGHT - pPlayer->fheight / 2.0f - WALLSIZ_Y / 2)
		{//����
			s_Player[count].pos.y = SCREEN_HEIGHT - pPlayer->fheight / 2.0f - WALLSIZ_Y / 2;
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
		//skill�g�p�\�Ȏ��̃G�t�F�N�g
		if (s_Player[count].bSkill)
		{
			SetEffect(D3DXVECTOR3(s_Player[count].pos.x, s_Player[count].pos.y - 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_SHOOT, 10, 200.0f);
		}
		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}
}

//----------------------------
// ���Z�b�g��Ԃ̍X�V
//----------------------------
static void UpdateReset(void)
{
	bool bOverlap[MAXPLAYER];

	for (int nPlayerNo = 0; nPlayerNo < MAXPLAYER; nPlayerNo++)
	{
		Player *pPlayer = &s_Player[nPlayerNo];

		bOverlap[nPlayerNo] = false;

		D3DXVECTOR3 posDest;
		
		switch (nPlayerNo)
		{
		case 0:
			posDest = D3DXVECTOR3(PLAYERSIZE_X, SCREEN_HEIGHT * 0.5f, 0.0f);
			break;

		case 1:
			posDest = D3DXVECTOR3(SCREEN_WIDTH - PLAYERSIZE_X, SCREEN_HEIGHT * 0.5f, 0.0f);
			break;
		
		default:
			posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			assert(false);
			break;
		}

		// �z�[�~���O
		bOverlap[nPlayerNo] = Homing(&pPlayer->pos, pPlayer->pos, posDest, MAX_RESET_SPEED);
		
		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pPlayer->nIdx, pPlayer->pos, pPlayer->rot, pPlayer->fwidth, pPlayer->fheight);
	}

	if (bOverlap[0] && bOverlap[1])
	{// �ǂ�����w��̈ʒu
		// �Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTATE_START);
	}
}

//----------------------------
// �ړ��̓��͂����邩�Ȃ���
//----------------------------
static void InputMove(void)
{
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;

	for (int i = 0; i < MAXPLAYER; i++)
	{
		s_bJoyPad[i] = false;
		s_bStickLeft[i] = false;
	}

	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) ||
		GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
	{// �L�[�������ꂽ
		s_bKeyBoardWASD = true;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// �L�[�������ꂽ
		s_bKeyBoardArrow = true;
	}

	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (GetJoypadIdxPress(JOYKEY_LEFT, i) || GetJoypadIdxPress(JOYKEY_RIGHT, i) ||
			GetJoypadIdxPress(JOYKEY_UP, i) || GetJoypadIdxPress(JOYKEY_DOWN, i))
		{// �{�^���������ꂽ
			s_bJoyPad[i] = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, i).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, i).y < -DEAD_ZONE)
		{// ���X�e�B�b�N���X����
			s_bStickLeft[i] = true;
		}
	}
}
