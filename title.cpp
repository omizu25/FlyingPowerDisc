//==================================================
//
// �^�C�g����� ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "rectangle.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_LIGHT				(2)				// ����̍ő吔
#define TITLE_POS_Y				(200.0f)		// �^�C�g����Y�̈ʒu
#define TITLE_WIDTH				(900.0f)		// �^�C�g���̕�
#define TITLE_HEIGHT			(200.0f)		// �^�C�g���̍���
#define LIGHT_WIDTH				(280.0f)		// ���C�g�̕�
#define LIGHT_HEIGHT			(280.0f)		// ���C�g�̍���
#define CHANGE_SPEED			(0.005f)		// ���k�̑��x
#define CHANGE_AMOUNT			(0.1f)			// �ω���
#define MEDIAN_LENGTH			(0.95f)			// �����̒����l
#define MAX_ROTATION			(0.003f)		// ��]�̍ő�l
#define DECREASE_SIZE			(0.6f)			// �T�C�Y�̌�������
#define DECREASE_SPEED			(0.5f)			// ���x�̌�������
#define MENU_WIDTH				(540.0f)		// ���j���[�̕�
#define MENU_HEIGHT				(64.0f)			// ���j���[�̍���

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		// �Q�[��
	MENU_MAX
}MENU;

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		rot;				// ����
	float			fLength;			// �Ίp���̒���
	float			fSaveLength;		// �������ł̑Ίp���̒�����ۑ�
	float			fAngle;				// �Ίp���̊p�x
	float			fSpeed;				// ���x
}Light;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTextureBG = NULL;			// �w�i�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// �w�i�̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureLight = NULL;			// ����̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLight = NULL;			// ����̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureFrame = NULL;			// �g�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		// ���j���[�̃e�N�X�`���ւ̃|�C���^
static Light						s_light[MAX_LIGHT];				// ����̏��
static int							s_nTime;						// ����
static int							s_nSelectMenu;					// �I�΂�Ă��郁�j���[
static int							s_nIdxUseMenu;					// �g���Ă��郁�j���[�̔ԍ�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateLight(void);
static void Input(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTitle(void)
{
	// ��`�̏�����
	InitRectAngle();

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

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIGHT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffLight,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = WHITE_COLOR;
	pVtx[1].col = WHITE_COLOR;
	pVtx[2].col = WHITE_COLOR;
	pVtx[3].col = WHITE_COLOR;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	fWidth = TITLE_WIDTH * 0.5f;
	fHeight = TITLE_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = WHITE_COLOR;
	pVtx[1].col = WHITE_COLOR;
	pVtx[2].col = WHITE_COLOR;
	pVtx[3].col = WHITE_COLOR;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffLight->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		pLight->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = pos.x + (sinf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.y = pos.y + (cosf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + (sinf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.y = pos.y + (cosf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + (sinf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.y = pos.y + (cosf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + (sinf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.y = pos.y + (cosf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = WHITE_COLOR;
		pVtx[1].col = WHITE_COLOR;
		pVtx[2].col = WHITE_COLOR;
		pVtx[3].col = WHITE_COLOR;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (i * 1.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f + (i * -1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (i * 1.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f + (i * -1.0f), 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffLight->Unlock();

	// ��`�̐ݒ�
	SetRectAngle(&s_pTextureBG, &s_pVtxBuffBG, 1);

	SetRectAngle(&s_pTextureLight, &s_pVtxBuffLight, MAX_LIGHT);

	SetRectAngle(&s_pTexture, &s_pVtxBuff, 1);

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
		menu.pTexture[i] = &s_pTextureMenu[i];
	}

	FrameArgument Frame;
	Frame.bUse = true;
	Frame.col = WHITE_COLOR;
//	Frame.col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	Frame.pTexture = &s_pTextureFrame;
//	Frame.pTexture = NULL;
	
	// ���j���[�̐ݒ�
	s_nIdxUseMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTitle(void)
{
	// ��`�̏I��
	UninitRectAngle();

	// ���j���[�̏I��
	UninitMenu();

	if (s_pTextureBG != NULL)
	{// �e�N�X�`���̉��
		s_pTextureBG->Release();
		s_pTextureBG = NULL;
	}

	if (s_pVtxBuffBG != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureLight != NULL)
	{// �e�N�X�`���̉��
		s_pTextureLight->Release();
		s_pTextureLight = NULL;
	}

	if (s_pTextureLight != NULL)
	{// ���_�o�b�t�@�̉��
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
	DrawRectAngle();

	// ���j���[�̕`��
	DrawMenu();
}

//--------------------------------------------------
// ���
//--------------------------------------------------
static void UpdateLight(void)
{
	s_nTime++;

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffLight->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_POS_Y, 0.0f);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		Light *pLight = &s_light[i];

		float fCurve = sinf((s_nTime * CHANGE_SPEED) * (D3DX_PI * 2.0f));

		pLight->fLength = pLight->fSaveLength * ((fCurve * CHANGE_AMOUNT) + MEDIAN_LENGTH);

		if (i == 0)
		{// ��
			pLight->rot.z += pLight->fSpeed;
		}
		else if (i == 1)
		{// ��O
			pLight->rot.z += -pLight->fSpeed;
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = pos.x + (sinf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.y = pos.y + (cosf(pLight->rot.z + (-D3DX_PI + pLight->fAngle)) * pLight->fLength);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + (sinf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.y = pos.y + (cosf(pLight->rot.z + (D3DX_PI + (pLight->fAngle * -1.0f))) * pLight->fLength);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + (sinf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.y = pos.y + (cosf(pLight->rot.z + (pLight->fAngle * -1.0f)) * pLight->fLength);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + (sinf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.y = pos.y + (cosf(pLight->rot.z + pLight->fAngle) * pLight->fLength);
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffLight->Unlock();
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
