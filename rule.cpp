//================================
//
// ���[���I����ʂ̏���
// Author:Sato Teruto
//
//================================

//�C���N���[�h
#include "rule.h"
#include "main.h"
#include "rectangle.h"
#include "color.h"
#include "input.h"
#include "texture.h"

//�}�N����`
#define MAX_RULE	(3)				//���[���̍ő吔
#define MAX_FLASH	(80)			//�_�ł̉�������
#define HALF_FLASH	(MAX_FLASH / 2)	//�_�ł̐؂�ւ�����

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^
static Rule s_Rule[MAX_RULE];	//�\���̂̎擾
static int s_nTime;				//�_�ł̎���
static int s_nSelect;			//�I�𒆂̔ԍ�

//============================
// ���[���I����ʂ̏�����
//============================
void InitRule(void)
{
	// ��`�̏�����
	InitRectangle();

	// �e�N�X�`���̎擾
	s_pTexture = GetTexture(TEXTURE_Title_blue);

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		rule->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		rule->fWidth = 0.0f;	//��
		rule->fHeight = 0.0f;	//����
		rule->bUse = false;		//�g�p���Ă��Ȃ����
	}
}

//============================
// ���[���I����ʂ̏I��
//============================
void UninitRule(void)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		// ��`���g���̂��~�߂�
		StopUseRectangle(rule->nIdx);
	}
}

//============================
// ���[���I����ʂ̍X�V
//============================
void UpdateRule(void)
{
	s_nTime++;				//�^�C���̉��Z
	s_nTime %= MAX_FLASH;	//�^�C���̏�����

	//�I��ԍ��̐؂�ւ�
	int nNumber = ChangeSelect();

	//�e�N�X�`���̓_��
	FlashTexture(nNumber);
}

//============================
// ���[���I����ʂ̕`��
//============================
void DrawRule(void)
{
	// ��`�̕`��
	DrawRectangle();
}

//============================
// ���[���I����ʂ̐ݒ�
//============================
void SetRule(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
			//�\���̂̐ݒ�
			rule->pos = pos;
			rule->fWidth = 200.0f;
			rule->fHeight = 100.0f;
			rule->bUse = true;

			// ��`�̐ݒ�
			rule->nIdx = SetRectangle(s_pTexture);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(rule->nIdx, pos, size);

			break;
		}
	}
}

//============================
// �e�N�X�`���̓_��
//============================
void FlashTexture(int nNumber)
{
	if (s_nTime >= HALF_FLASH)
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Rule[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else
	{
		// ��`�̐F�̐ݒ�
		SetColorRectangle(s_Rule[nNumber].nIdx, GetColor(COLOR_WHITE));
	}
}

//============================
// �I��ԍ��̐؂�ւ�
//============================
int ChangeSelect(void)
{
	// ��`�̐F�̐ݒ�
	SetColorRectangle(s_Rule[s_nSelect].nIdx, GetColor(COLOR_WHITE));

	if (GetKeyboardTrigger(DIK_W))
	{//W�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 1 && s_nSelect <= MAX_RULE)
		{//0�����ɂȂ�Ȃ��Ȃ�
			s_nSelect--;
		}
	}
	else if (GetKeyboardTrigger(DIK_S))
	{//S�L�[�������ꂽ�Ƃ�
		if (s_nSelect >= 0 && s_nSelect < (MAX_RULE - 1))
		{//�ő吔�𒴂��Ȃ�Ȃ��Ȃ�
			s_nSelect++;
		}
	}

	return s_nSelect;
}