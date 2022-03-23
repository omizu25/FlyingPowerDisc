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
#include "menu.h"
#include "color.h"
#include "gauge.h"

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // �}�N����`
 //------------------------------
#define MAX_CHARACTER	(5)					//�L�����̍ő吔
#define START_POS_X		(50.0f)				//�X�^�[�g�ʒu�A�����p
#define GAUGE_WIDTH		(50.0f)				//�Q�[�W�̕�
#define GAUGE_HEIGHT	(45.0f)				//�Q�[�W�̍���

 //------------------------------
 // �񋓌^
 //------------------------------
typedef enum
{
	MENU_FOX = 0,	// �L�c�l
	MENU_CAPYBARA,	// �J�s�o��
	MENU_SLIME,		// �X���C��
	MENU_GHOST,		// �S�[�X�g
	MENU_GROUND,	// �n��
	MENU_MAX
}MENU;

//------------------------------
// �\����
//------------------------------
typedef struct
{
	float fPower;	// �p���[
	float fSpeed;	// �X�s�[�h
}Status;

 //------------------------------
 // �X�^�e�B�b�N�ϐ�
 //------------------------------
int s_nSelect[MAXPLAYER];
int s_nIdxPower[MAXPLAYER];
int s_nIdxSpeed[MAXPLAYER];
Status s_status[MAX_CHARACTER];
LPDIRECT3DTEXTURE9 s_pTexture[MAX_CHARACTER];
int s_nIdxBG;
int s_nIdxMenu;

//------------------------------
// �v���g�^�C�v�錾
//------------------------------
static void ChangeGauge(void);

//============================
// �L�����I����ʂ̏�����
//============================
void InitCharacter(void)
{
	// ��`�̏�����
	InitRectangle();

	{// �w�i
		// ��`�̐ݒ�
		s_nIdxBG = SetRectangle(TEXTURE_BG);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	LoadFileSet("data\\txt\\Status.txt");

	//Player
	InitPlayer();

	Player *pPlayer = GetPlayer();
	s_nSelect[0] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(START_POS_X + PLAYERSIZE , SCREEN_HEIGHT * 0.2f, 0.0f), pPlayer->nType, true, PLAYERSIZE*2);

	pPlayer++;
	s_nSelect[1] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - START_POS_X - PLAYERSIZE, SCREEN_HEIGHT * 0.2f, 0.0f), pPlayer->nType, false, PLAYERSIZE*2);
	
	{// �Q�[�W
		// �Q�[�W�̏�����
		InitGauge();

		float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// �Q�[�W�̐ݒ�
		s_nIdxPower[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// �Q�[�W�̐ݒ�
		s_nIdxSpeed[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// �Q�[�W�̐ݒ�
		s_nIdxPower[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

		// �Q�[�W�̐ݒ�
		s_nIdxSpeed[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH, fHeight);
	}

	{// ���j���[
		// ���j���[�̏�����
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft = 0.0f;
		menu.fRight = SCREEN_WIDTH;
		menu.fTop = 0.0f;
		menu.fBottom = SCREEN_HEIGHT;
		menu.fWidth = PLAYERSIZE;
		menu.fHeight = PLAYERSIZE;
		menu.bSort = true;

		menu.texture[MENU_FOX] = TEXTURE_kitune;
		menu.texture[MENU_CAPYBARA] = TEXTURE_enemy000;
		menu.texture[MENU_SLIME] = TEXTURE_player000;
		menu.texture[MENU_GHOST] = TEXTURE_ghost;
		menu.texture[MENU_GROUND] = TEXTURE_zolbak;

		FrameArgument Frame;
		Frame.bUse = false;
		Frame.col = GetColor(COLOR_WHITE);
		Frame.texture = TEXTURE_NONE;

		// ���j���[�̐ݒ�
		s_nIdxMenu = SetMenu(menu, Frame);
	}
}

//============================
// �L�����I����ʂ̏I��
//============================
void UninitCharacter(void)
{
	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);

	UninitPlayer();

	// ���j���[�̏I��
	UninitMenu();

	// �Q�[�W�̏I��
	UninitGauge();

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

		if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
		{//S�L�[�������ꂽ�Ƃ�
		 //���l�̌��Z
			player->nType++;

			if (player->nType >= MAX_CHARACTER)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
		{//W�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z
			player->nType--;
			if (player->nType < 0)
			{
				player->nType = MAX_CHARACTER - 1;
				
			}
		}

		s_nSelect[0] = player->nType;
		//�e�N�X�`���X�V
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

		ChangeGauge();
	}

	{	
		Player*player = GetPlayer();
		player++;
		if (GetKeyboardTrigger(DIK_NUMPAD2) || GetJoypadTrigger(JOYKEY_DOWN))
		{//S�L�[�������ꂽ�Ƃ�
		 //���l�̌��Z
			player->nType++;
			if (player->nType >= MAX_CHARACTER)
			{
				player->nType = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_NUMPAD5) || GetJoypadTrigger(JOYKEY_UP))
		{//W�L�[�������ꂽ�Ƃ�
		 //���l�̉��Z
			player->nType--;
			
			if (player->nType < 0)
			{
				player->nType = MAX_CHARACTER - 1;
			}
		}

		s_nSelect[1] = player->nType;
		//�e�N�X�`���X�V
		ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

		ChangeGauge();
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{	//�Q�[���I����ʍs��
		ChangeMode(MODE_TITLE);
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
	int		number = 0;

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");

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

			if (strcmp(&s_aString[0], "STATUSSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &s_aString[0]);

					if (strcmp(&s_aString[0], "ATTACKPOW") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_status[number].fPower);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_status[number].fSpeed);
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
//�Q�[�W�̕ύX
//----------------------------
static void ChangeGauge(void)
{
	float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// �Q�[�W�̌���
	SubGauge(s_nIdxPower[0], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// �Q�[�W�̐ݒ�
	SubGauge(s_nIdxSpeed[0], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// �Q�[�W�̐ݒ�
	SubGauge(s_nIdxPower[1], pos, GAUGE_WIDTH, fHeight);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT - (fHeight * 0.5f), 0.0f);

	// �Q�[�W�̐ݒ�
	SubGauge(s_nIdxSpeed[1], pos, GAUGE_WIDTH, fHeight);
}