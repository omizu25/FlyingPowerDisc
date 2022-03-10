//**************************************************
// 
// FPG���� ( menu.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "input.h"
#include "menu.h"
#include "fade.h"
#include "color.h"
#include "rectangle.h"

//==================================================
// �}�N����`
//==================================================
#define MAX_MENU					(16)		// ���j���[�̍ő吔
#define NORMAL_BLINK_SPEED			(0.01f)		// �ʏ펞�̓_�ő��x
#define DECISION_BLINK_SPEED		(0.1f)		// ���莞�̓_�ő��x
#define MIN_ALPHA					(0.6f)		// ���l�̍ŏ��l

//==================================================
// �\����
//==================================================

/*�� �I���� ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXCOLOR				col;			// �F
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
	int						nIdx;			// ��`�̃C���f�b�N�X
	float					fWidth;			// ��
	float					fHeight;		// ����
}Option;

/*�� ���j���[ ��*/

typedef struct
{
	D3DXVECTOR3				pos;					// �ʒu
	LPDIRECT3DTEXTURE9		pTexture;				// �e�N�X�`��
	Option					Option[MAX_OPTION];		// �I�����̏��
	int						nNumUse;				// �g�p��
	int						nIdx;					// ��`�̃C���f�b�N�X
	float					fWidth;					// ��
	float					fHeight;				// ����
	float					fInterval;				// �I�����̊Ԋu
	float					fBlinkSpeed;			// �_�ő��x
	bool					bFrame;					// �g�����邩�ǂ��� [ true : ���� false : ����Ȃ� ]
	bool					bUse;					// �g�p���Ă��邩�ǂ���
}Menu;

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static Menu		s_aMenu[MAX_MENU];		// ���j���[�̏��
static int		s_nIdxMenu;				// �I�΂�Ă��郁�j���[�̔ԍ�
static int		s_nIdxOption;			// �I�΂�Ă���I�����̔ԍ�
static int		s_nAlphaTime;			// ���l�ύX�p�̎���

//==================================================
// �v���g�^�C�v�錾
//==================================================
static void ChangeColor(Menu *pMenu);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMenu(void)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;

	// �������̃N���A
	memset(s_aMenu, 0, sizeof(s_aMenu));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMenu(void)
{
	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		if (pMenu->bFrame)
		{// �g���g���Ă���
			// �g���̂��~�߂�
			StopUseRectangle(pMenu->nIdx);
		}

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			// �g���̂��~�߂�
			StopUseRectangle(pOption->nIdx);
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	if (!pMenu->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}
	
	// �F�̕ύX
	ChangeColor(pMenu);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame)
{
	int nIdx = 0;
	Menu *pMenu = nullptr;

	for (nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pMenu->nNumUse = menu.nNumUse;
		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;
		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;
		pMenu->pTexture = Frame.pTexture;
		pMenu->bFrame = Frame.bUse;

		if (menu.bSort)
		{// �c
			pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		}
		else
		{// ��
			pMenu->fInterval = pMenu->fWidth / (menu.nNumUse + 1);
		}

		pMenu->bUse = true;

		s_nIdxMenu = nIdx;
		s_nIdxOption = 0;

		if (Frame.bUse)
		{// �g������
			// ��`�̐ݒ�
			pMenu->nIdx = SetRectangle(Frame.pTexture);

			fPosX = menu.fLeft + (pMenu->fWidth * 0.5f);
			fPosY = menu.fTop + (pMenu->fHeight * 0.5f);
			D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
			D3DXVECTOR3 size = D3DXVECTOR3(pMenu->fWidth, pMenu->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(pMenu->nIdx, pos, size);

			// ��`�̐F�̐ݒ�
			SetColorRectangle(pMenu->nIdx, Frame.col);
		}

		break;
	}

	for (int j = 0; j < pMenu->nNumUse; j++)
	{
		Option *pOption = &pMenu->Option[j];

		if (menu.bSort)
		{// �c
			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
		}
		else
		{// ��
			pOption->pos = D3DXVECTOR3(menu.fLeft + (pMenu->fInterval * (j + 1)), pMenu->pos.y, 0.0f);
		}
		
		pOption->col = GetColor(COLOR_WHITE);
		pOption->fWidth = menu.fWidth;
		pOption->fHeight = menu.fHeight;
		pOption->pTexture = menu.pTexture[j];

		// ��`�̐ݒ�
		pOption->nIdx = SetRectangle(menu.pTexture[j]);

		D3DXVECTOR3 size = D3DXVECTOR3(menu.fWidth, menu.fHeight, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(pOption->nIdx, pOption->pos, size);

		// ��`�̐F�̐ݒ�
		SetColorRectangle(pOption->nIdx, pOption->col);
	}

	return nIdx;
}

//--------------------------------------------------
// �I�����̐F�̏�����
//--------------------------------------------------
void InitColorOption(void)
{
	Option *pOption = &s_aMenu[s_nIdxMenu].Option[s_nIdxOption];

	// ��`�̐F�̐ݒ�
	SetColorRectangle(pOption->nIdx, GetColor(COLOR_WHITE));
}

//--------------------------------------------------
// �I�����̕ύX
//--------------------------------------------------
void ChangeOption(int nIdx)
{
	s_nIdxOption = nIdx;
}

//--------------------------------------------------
// �I�����̌���
//--------------------------------------------------
void DecisionOption(void)
{
	s_aMenu[s_nIdxMenu].fBlinkSpeed = DECISION_BLINK_SPEED;
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;
	
	Menu *pMenu = &s_aMenu[nIdx];

	if (pMenu->bFrame)
	{// �g���g���Ă���
		// �g���̂��~�߂�
		StopUseRectangle(pMenu->nIdx);
	}

	for (int i = 0; i < pMenu->nNumUse; i++)
	{
		Option *pOption = &pMenu->Option[i];

		// �g���̂��~�߂�
		StopUseRectangle(pOption->nIdx);
	}

	pMenu->bUse = false;
}

//--------------------------------------------------
// �F�̕ύX
//--------------------------------------------------
static void ChangeColor(Menu *pMenu)
{
	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nIdxOption];

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));
	float fAlpha = (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);

	// ��`�̐F�̐ݒ�
	SetColorRectangle(pOption->nIdx, pOption->col);
}
