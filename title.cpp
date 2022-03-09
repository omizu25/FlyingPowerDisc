//**************************************************
//
// �^�C�g����� ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "rectangle.h"
#include "title.h"
#include "color.h"

#include <assert.h>

//==================================================
// �}�N����`
//==================================================
#define MAX_LIGHT			(2)				// ����̍ő吔
#define TITLE_POS_Y			(200.0f)		// �^�C�g����Y�̈ʒu
#define TITLE_WIDTH			(900.0f)		// �^�C�g���̕�
#define TITLE_HEIGHT		(200.0f)		// �^�C�g���̍���
#define LIGHT_WIDTH			(280.0f)		// ���C�g�̕�
#define LIGHT_HEIGHT		(280.0f)		// ���C�g�̍���
#define CHANGE_SPEED		(0.005f)		// ���k�̑��x
#define CHANGE_AMOUNT		(0.1f)			// �ω���
#define MEDIAN_LENGTH		(0.95f)			// �����̒����l
#define MAX_ROTATION		(0.003f)		// ��]�̍ő�l
#define DECREASE_SIZE		(0.6f)			// �T�C�Y�̌�������
#define DECREASE_SPEED		(0.5f)			// ���x�̌�������
#define MENU_WIDTH			(540.0f)		// ���j���[�̕�
#define MENU_HEIGHT			(64.0f)			// ���j���[�̍���

//==================================================
// �񋓌^
//==================================================
typedef enum
{
	MENU_GAME = 0,		// �Q�[��
	MENU_MAX
}MENU;

//==================================================
// �\����
//==================================================
typedef struct
{
	D3DXVECTOR3		rot;				// ����
	float			fLength;			// �Ίp���̒���
	float			fSaveLength;		// �������ł̑Ίp���̒�����ۑ�
	float			fAngle;				// �Ίp���̊p�x
	float			fSpeed;				// ���x
	int				nIdx;				// ��`�̃C���f�b�N�X
}Light;

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static LPDIRECT3DTEXTURE9		s_pTextureBG = NULL;			// �w�i�̃e�N�X�`���ւ̃|�C���^
static int						s_nIdxBG;						// �w�i�̋�`�̃C���f�b�N�X
static LPDIRECT3DTEXTURE9		s_pTextureLight = NULL;			// ����̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static int						s_nIdx;							// ��`�̃C���f�b�N�X
static LPDIRECT3DTEXTURE9		s_pTextureFrame = NULL;			// �g�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureMenu[MENU_MAX];		// ���j���[�̃e�N�X�`���ւ̃|�C���^
static Light					s_light[MAX_LIGHT];				// ����̏��
static int						s_nTime;						// ����
static int						s_nSelectMenu;					// �I�΂�Ă��郁�j���[
static int						s_nIdxUseMenu;					// �g���Ă��郁�j���[�̔ԍ�

//==================================================
// �v���g�^�C�v�錾
//==================================================
static void UpdateLight(void);
static void Input(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTitle(void)
{
	// ��`�̏�����
	InitRectangle();

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;
	s_nSelectMenu = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BG.png",
		&s_pTextureBG);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title_blue.png",
		&s_pTexture);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TitleLight_red.png",
		&s_pTextureLight);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Frame.png",
		&s_pTextureFrame);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/New_GAMESTART_NoBG.png",
		&s_pTextureMenu[MENU_GAME]);

	// ��`�̐ݒ�
	s_nIdxBG = SetRectangle(s_pTextureBG);

	// ��`�̐ݒ�
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		pLight->nIdx = SetRectangle(s_pTextureLight);
	}

	// ��`�̐ݒ�
	s_nIdx = SetRectangle(s_pTexture);

	{// �w�i
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdxBG, pos, size);
	}

	{// ���S
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(s_nIdx, pos, size);
	}

	{// ���
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			Light *pLight = &s_light[i];

			pLight->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			float fWidth = 0.0f, fHeight = 0.0f;

			if (i == 0)
			{//	��
				fWidth = LIGHT_WIDTH;
				fHeight = LIGHT_HEIGHT;
				pLight->fSpeed = MAX_ROTATION * DECREASE_SPEED;
			}
			else
			{// ��O
				fWidth = LIGHT_WIDTH * DECREASE_SIZE;
				fHeight = LIGHT_HEIGHT * DECREASE_SIZE;
				pLight->fSpeed = MAX_ROTATION;
			}

			// �Ίp���̒������Z�o����
			pLight->fSaveLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
			pLight->fLength = pLight->fSaveLength;

			// �Ίp���̊p�x���Z�o����
			pLight->fAngle = atan2f(fWidth, fHeight);

			// ��`�̉�]����ʒu�̐ݒ�
			SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, pLight->fAngle, pLight->fLength);

			D3DXVECTOR2 U = D3DXVECTOR2(0.0f + (i * 1.0f), 1.0f + (i * -1.0f));
			D3DXVECTOR2 V = D3DXVECTOR2(0.0f, 1.0f);

			// ��`�̃e�N�X�`�����W�̐ݒ�
			SetTexRectangle(pLight->nIdx, U, V);
		}
	}

	// ���j���[�̏�����
	InitMenu();

	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = SCREEN_WIDTH * 0.25f;
	menu.fRight = SCREEN_WIDTH * 0.75f;
	menu.fTop = SCREEN_HEIGHT * 0.5f;
	menu.fBottom = SCREEN_HEIGHT;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	for (int i = 0; i < MENU_MAX; i++)
	{
		menu.pTexture[i] = s_pTextureMenu[i];
	}

	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = GetColor(COLOR_WHITE);
	Frame.pTexture = s_pTextureFrame;
	
	// ���j���[�̐ݒ�
	s_nIdxUseMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTitle(void)
{
	// ��`�̏I��
	UninitRectangle();

	// ���j���[�̏I��
	UninitMenu();

	// �g���̂��~�߂�
	StopUseRectangle(s_nIdxBG);
	StopUseRectangle(s_nIdx);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];
		StopUseRectangle(pLight->nIdx);
	}

	if (s_pTextureBG != NULL)
	{// �e�N�X�`���̉��
		s_pTextureBG->Release();
		s_pTextureBG = NULL;
	}

	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pTextureLight != NULL)
	{// �e�N�X�`���̉��
		s_pTextureLight->Release();
		s_pTextureLight = NULL;
	}

	if (s_pTextureFrame != NULL)
	{// �e�N�X�`���̉��
		s_pTextureFrame->Release();
		s_pTextureFrame = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateTitle(void)
{
	// ���
	UpdateLight();

	// ����
	Input();

	// ���j���[�̍X�V
	UpdateMenu();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawTitle(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//--------------------------------------------------
// ���
//--------------------------------------------------
static void UpdateLight(void)
{
	s_nTime++;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);
	float fCurve = sinf((s_nTime * CHANGE_SPEED) * (D3DX_PI * 2.0f));

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		pLight->fLength = pLight->fSaveLength * ((fCurve * CHANGE_AMOUNT) + MEDIAN_LENGTH);

		if (i == 0)
		{// ��
			pLight->rot.z += pLight->fSpeed;
		}
		else if (i == 1)
		{// ��O
			pLight->rot.z += -pLight->fSpeed;
		}

		// ��`�̉�]����ʒu�̐ݒ�
		SetRotationPosRectangle(pLight->nIdx, pos, pLight->rot, pLight->fAngle, pLight->fLength);
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// W�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);

	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// S�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAME:				// �Q�[��
			ChangeMode(MODE_GAME);
			break;

		default:
			assert(false);
			break;
		}

		// �I�����̌���
		DecisionOption();
	}
}
