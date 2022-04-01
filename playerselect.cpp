//================================
//
// �L�����I����ʂ̏���
// Author:HAMADA
// Author:KATSUKI MIZUKI
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
#include "cursor.h"
#include "sound.h"

#include <stdio.h>
#include <assert.h>

 //------------------------------
 // �}�N����`
 //------------------------------
#define MAX_CHARACTER		(5)				//�L�����̍ő吔
#define START_POS_X			(175.0f)		//�X�^�[�g�ʒu�A�����p
#define GAUGE_WIDTH			(50.0f)			//�Q�[�W�̕�
#define GAUGE_HEIGHT		(35.0f)			//�Q�[�W�̍���
#define FRAME_SIZE			(5.0f)			//�g�̃T�C�Y
#define DESCRIPTION_WIDTH	(50.0f)			//�����̕�
#define DESCRIPTION_HEIGHT	(350.0f)		//�����̍���
#define CURSOR_SIZE			(75.0f)			//�J�[�\���̃T�C�Y
#define UI_WIDTH			(100.0f)		//UI�̕�
#define UI_HEIGHT			(75.0f)			//UI�̍���
#define OK_WIDTH			(150.0f)		//OK�̕�
#define OK_HEIGHT			(75.0f)			//OK�̍���
#define SELECT_WIDTH		(250.0f)		//�I���̕�
#define SELECT_HEIGHT		(50.0f)			//�I���̍���

 //------------------------------
 // �񋓌^
 //------------------------------
typedef enum
{
	MENU_FOX = 0,	// �L�c�l
	MENU_CAPYBARA,	// �J�s�o��
	MENU_SLIME,		// �X���C��
	MENU_GHOST,		// �S�[�X�g
	MENU_CAT,		// �L
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
static int s_nSelect[MAXPLAYER];
static int s_nIdxPowerGauge[MAXPLAYER];
static int s_nIdxSpeedGauge[MAXPLAYER];
static int s_nIdxPowerFrame[MAXPLAYER];
static int s_nIdxSpeedFrame[MAXPLAYER];
static int s_nIdxPower[MAXPLAYER];
static int s_nIdxSpeed[MAXPLAYER];
static Status s_status[MAX_CHARACTER];
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_CHARACTER];
static int s_nIdxBG;
static int s_nIdxMenu;
static int s_nIdxUI[MAXPLAYER];
static int s_nIdxCursor[MAXPLAYER];
static int s_nIdx;
static int s_nIdxSelect[MAXPLAYER];
static bool s_bSelect[MAXPLAYER];

//------------------------------
// �v���g�^�C�v�錾
//------------------------------
static void UpdateChangeGauge(void);

//============================
// �L�����I����ʂ̏�����
//============================
void InitCharacter(void)
{
	PlaySound(SOUND_LABEL_TITLE);

	// ��`�̏�����
	InitRectangle();

	{// �w�i
		// ��`�̐ݒ�
		s_nIdxBG = SetRectangle(TEXTURE_Select_BG);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// �v���C���[�I��
		// ��`�̐ݒ�
		s_nIdx = SetRectangle(TEXTURE_Char_Title);

		D3DXVECTOR3 size = D3DXVECTOR3(SELECT_WIDTH, SELECT_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SELECT_WIDTH * 0.5f, SELECT_HEIGHT * 0.5f, 0.0f);

		SetPosRectangle(s_nIdx, pos, size);
	}

	{// UI
		// ��`�̐ݒ�
		s_nIdxUI[0] = SetRectangle(TEXTURE_UI000);

		D3DXVECTOR3 pos = D3DXVECTOR3(UI_WIDTH, 130.0f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(UI_WIDTH, UI_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxUI[0], pos, size);

		// ��`�̐ݒ�
		s_nIdxUI[1] = SetRectangle(TEXTURE_UI001);

		pos.x = SCREEN_WIDTH - UI_WIDTH;

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxUI[1], pos, size);
	}

	{// OK
		// ��`�̐ݒ�
		s_nIdxSelect[0] = SetRectangle(TEXTURE_OK);

		D3DXVECTOR3 pos = D3DXVECTOR3(UI_WIDTH, SCREEN_HEIGHT * 0.3f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(OK_WIDTH, OK_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxSelect[0], pos, size);

		// ��`�̐ݒ�
		s_nIdxSelect[1] = SetRectangle(TEXTURE_OK);

		pos.x = SCREEN_WIDTH - UI_WIDTH;

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxSelect[1], pos, size);

		s_bSelect[0] = false;
		s_bSelect[1] = false;
	}

	LoadFileSet("data\\txt\\Status.txt");

	//Player
	InitPlayer();

	Player *pPlayer = GetPlayer();
	s_nSelect[0] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(START_POS_X + PLAYERSIZE , SCREEN_HEIGHT * 0.25f, 0.0f), pPlayer->nType, true, PLAYERSIZE*2);

	pPlayer++;
	s_nSelect[1] = pPlayer->nType;
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - START_POS_X - PLAYERSIZE, SCREEN_HEIGHT * 0.25f, 0.0f), pPlayer->nType, false, PLAYERSIZE*2);
	
	{// �Q�[�W
		// �Q�[�W�̏�����
		InitGauge();

		float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.903f, 0.0f);
		float fFrameWidth = (FRAME_SIZE * 2.0f);
		
		// �Q�[�W�̐ݒ�
		s_nIdxPowerFrame[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE, GAUGE_BOTTOM);
		s_nIdxPowerGauge[0] = SetGauge(pos, GetColor(COLOR_RED), GAUGE_WIDTH, fHeight, GAUGE_BOTTOM);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
		pos.x = SCREEN_WIDTH * 0.3f;

		// �Q�[�W�̐ݒ�
		s_nIdxSpeedFrame[0] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE, GAUGE_BOTTOM);
		s_nIdxSpeedGauge[0] = SetGauge(pos, GetColor(COLOR_BLUE), GAUGE_WIDTH, fHeight, GAUGE_BOTTOM);

		fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
		pos.x = SCREEN_WIDTH * 0.875f;

		// �Q�[�W�̐ݒ�
		s_nIdxPowerFrame[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE, GAUGE_BOTTOM);
		s_nIdxPowerGauge[1] = SetGauge(pos, GetColor(COLOR_RED), GAUGE_WIDTH, fHeight, GAUGE_BOTTOM);

		fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
		pos.x = SCREEN_WIDTH * 0.7f;

		// �Q�[�W�̐ݒ�
		s_nIdxSpeedFrame[1] = SetGauge(pos, GetColor(COLOR_WHITE), GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE, GAUGE_BOTTOM);
		s_nIdxSpeedGauge[1] = SetGauge(pos, GetColor(COLOR_BLUE), GAUGE_WIDTH, fHeight, GAUGE_BOTTOM);
	}

	{// ����
		D3DXVECTOR3 size = D3DXVECTOR3(DESCRIPTION_WIDTH, DESCRIPTION_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT * 0.9025f) - (size.y * 0.5f), 0.0f);

		for (int i = 0; i < MAXPLAYER; i++)
		{
			// ��`�̐ݒ�
			s_nIdxPower[i] = SetRectangle(TEXTURE_Power);
			s_nIdxSpeed[i] = SetRectangle(TEXTURE_Speed);

			switch (i)
			{// �p���[��X�̈ʒu
			case 0:
				pos.x = SCREEN_WIDTH * 0.075f;
				break;

			case 1:
				pos.x = SCREEN_WIDTH * 0.925f;
				break;

			default:
				assert(false);
				break;
			}

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(s_nIdxPower[i], pos, size);

			switch (i)
			{// �X�s�[�h��X�̈ʒu
			case 0:
				pos.x = SCREEN_WIDTH * 0.25f;
				break;

			case 1:
				pos.x = SCREEN_WIDTH * 0.75f;
				break;

			default:
				assert(false);
				break;
			}

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(s_nIdxSpeed[i], pos, size);
		}
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
		menu.texture[MENU_CAT] = TEXTURE_cat;

		FrameArgument Frame;
		Frame.bUse = false;
		Frame.col = GetColor(COLOR_WHITE);
		Frame.texture = TEXTURE_NONE;

		// ���j���[�̐ݒ�
		s_nIdxMenu = SetMenu(menu, Frame);
	}

	{// �J�[�\��
		// �J�[�\��������
		InitCursor();

		CursorArgument cursor;
		cursor.nNumUse = MENU_MAX;
		cursor.fPosX = SCREEN_WIDTH * 0.4f;
		cursor.fTop = 0.0f;
		cursor.fBottom = SCREEN_HEIGHT;
		cursor.fWidth = CURSOR_SIZE;
		cursor.fHeight = CURSOR_SIZE;
		cursor.texture = TEXTURE_Disc;
		cursor.nSelect = s_nSelect[0];
		cursor.bRotation = true;

		// �J�[�\���̐ݒ�
		s_nIdxCursor[0] = SetCursor(cursor);

		cursor.fPosX = SCREEN_WIDTH * 0.6f;
		cursor.texture = TEXTURE_Disc;
		cursor.nSelect = s_nSelect[1];

		// �J�[�\���̐ݒ�
		s_nIdxCursor[1] = SetCursor(cursor);
	}
}

//============================
// �L�����I����ʂ̏I��
//============================
void UninitCharacter(void)
{
	//���̒�~
	StopSound();

	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);
	StopUseRectangle(s_nIdxMenu);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		StopUseRectangle(s_nIdxUI[i]);
		StopUseRectangle(s_nIdxCursor[i]);
		StopUseRectangle(s_nIdxPower[i]);
		StopUseRectangle(s_nIdxSpeed[i]);
	}

	UninitPlayer();

	// ���j���[�̏I��
	UninitMenu();

	// �J�[�\���̏I��
	UninitCursor();

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
	if (s_bSelect[0] && s_bSelect[1])
	{
		//�Q�[���I����ʍs��
		ChangeMode(MODE_TITLE);
	}
	else
	{
		{
			Player*player = GetPlayer();

			if (GetKeyboardTrigger(DIK_S) || GetJoypadIdxTrigger(JOYKEY_DOWN, 0) ||
				GetJoypadIdxStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_DOWN, 0))
			{//S�L�[�������ꂽ�Ƃ�
			 //���̍Đ�
				PlaySound(SOUND_LABEL_SELECT);

				//���l�̌��Z
				player->nType++;

				if (player->nType >= MAX_CHARACTER)
				{
					player->nType = 0;
				}

				s_bSelect[0] = false;

				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdxSelect[0], GetColor(COLOR_WHITE));
			}

			if (GetKeyboardTrigger(DIK_W) || GetJoypadIdxTrigger(JOYKEY_UP, 0) ||
				GetJoypadIdxStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_UP, 0))
			{//W�L�[�������ꂽ�Ƃ�
			 //���̍Đ�
				PlaySound(SOUND_LABEL_SELECT);

				//���l�̉��Z
				player->nType--;
				if (player->nType < 0)
				{
					player->nType = MAX_CHARACTER - 1;

				}

				s_bSelect[0] = false;

				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdxSelect[0], GetColor(COLOR_WHITE));
			}

			s_nSelect[0] = player->nType;
			//�e�N�X�`���X�V
			ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

			// �Q�[�W�̕ύX
			UpdateChangeGauge();

			// �J�[�\���̈ʒu�̕ύX
			ChangePosCursor(s_nIdxCursor[0], s_nSelect[0]);
		}

		{
			Player*player = GetPlayer();
			player++;
			if (GetKeyboardTrigger(DIK_NUMPAD2) || GetJoypadIdxTrigger(JOYKEY_DOWN, 1) ||
				GetJoypadIdxStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_DOWN, 1))
			{//S�L�[�������ꂽ�Ƃ�
			 //���̍Đ�
				PlaySound(SOUND_LABEL_SELECT);

				//���l�̌��Z
				player->nType++;
				if (player->nType >= MAX_CHARACTER)
				{
					player->nType = 0;
				}

				s_bSelect[1] = false;

				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdxSelect[1], GetColor(COLOR_WHITE));
			}

			if (GetKeyboardTrigger(DIK_NUMPAD5) || GetJoypadIdxTrigger(JOYKEY_UP, 1) ||
				GetJoypadIdxStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_UP, 1))
			{//W�L�[�������ꂽ�Ƃ�
			 //���̍Đ�
				PlaySound(SOUND_LABEL_SELECT);

				//���l�̉��Z
				player->nType--;

				if (player->nType < 0)
				{
					player->nType = MAX_CHARACTER - 1;
				}

				s_bSelect[1] = false;

				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdxSelect[1], GetColor(COLOR_WHITE));
			}

			s_nSelect[1] = player->nType;
			//�e�N�X�`���X�V
			ChangeTextureRectangleWithTex(player->nIdx, s_pTexture[player->nType]);

			// �Q�[�W�̕ύX
			UpdateChangeGauge();

			// �J�[�\���̈ʒu�̕ύX
			ChangePosCursor(s_nIdxCursor[1], s_nSelect[1]);
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
			GetJoypadIdxTrigger(JOYKEY_START, 0) ||
			GetJoypadIdxTrigger(JOYKEY_A, 0) || GetJoypadIdxTrigger(JOYKEY_B, 0))
		{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
			//���̍Đ�
			PlaySound(SOUND_LABEL_ENTER);

			s_bSelect[0] = true;

			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_nIdxSelect[0], GetColor(COLOR_RED));
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
			GetJoypadIdxTrigger(JOYKEY_START, 1) ||
			GetJoypadIdxTrigger(JOYKEY_A, 1) || GetJoypadIdxTrigger(JOYKEY_B, 1))
		{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		 //���̍Đ�
			PlaySound(SOUND_LABEL_ENTER);

			s_bSelect[1] = true;

			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_nIdxSelect[1], GetColor(COLOR_RED));
		}

		// �Q�[�W�̍X�V
		UpdateGauge();

		// �J�[�\���̍X�V
		UpdateCursor();
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
static void UpdateChangeGauge(void)
{
	float fFrameWidth = (FRAME_SIZE * 2.0f);
	float fHeight = GAUGE_HEIGHT * s_status[s_nSelect[0]].fPower;

	// �Q�[�W�̕ύX
	ChangeGauge(s_nIdxPowerGauge[0], GAUGE_WIDTH, fHeight);
	ChangeGauge(s_nIdxPowerFrame[0], GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[0]].fSpeed * 0.5f);
	
	// �Q�[�W�̕ύX
	ChangeGauge(s_nIdxSpeedGauge[0], GAUGE_WIDTH, fHeight);
	ChangeGauge(s_nIdxSpeedFrame[0], GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE);

	fHeight = GAUGE_HEIGHT * s_status[s_nSelect[1]].fPower;
	
	// �Q�[�W�̕ύX
	ChangeGauge(s_nIdxPowerGauge[1], GAUGE_WIDTH, fHeight);
	ChangeGauge(s_nIdxPowerFrame[1], GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE);

	fHeight = GAUGE_HEIGHT * (s_status[s_nSelect[1]].fSpeed * 0.5f);
	
	// �Q�[�W�̕ύX
	ChangeGauge(s_nIdxSpeedGauge[1], GAUGE_WIDTH, fHeight);
	ChangeGauge(s_nIdxSpeedFrame[1], GAUGE_WIDTH + fFrameWidth, fHeight + FRAME_SIZE);
}