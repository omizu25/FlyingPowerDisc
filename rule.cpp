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
#define MAX_RULE	(3)					//���[���̍ő吔
#define MAX_SWITCH	(6)					//�X�C�b�`�̍ő吔
#define HALF_SWITCH	(MAX_SWITCH / 2)	//�X�C�b�`�̔���
#define MAX_TEXTURE	(3)					//�e�N�X�`���̍ő吔
#define MAX_FLASH	(80)				//�_�ł̉�������
#define HALF_FLASH	(MAX_FLASH / 2)		//�_�ł̐؂�ւ�����

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture[MAX_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
static Rule s_Rule[MAX_RULE];		//���[���\���̂̎擾
static Switch s_Switch[MAX_SWITCH];	//�X�C�b�`�\���̂̎擾
static int s_nTime;					//�_�ł̎���
static int s_nSelect;				//�I�𒆂̔ԍ�

//============================
// ���[���I����ʂ̏�����
//============================
void InitRule(void)
{
	// ��`�̏�����
	InitRectangle();

	//------------------------------
	// �e�N�X�`���̎擾
	//------------------------------
	s_pTexture[0] = GetTexture(TEXTURE_Title_blue);
	s_pTexture[1] = GetTexture(TEXTURE_Select_Left);
	s_pTexture[2] = GetTexture(TEXTURE_Select_Right);

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	//���[���\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		rule->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		rule->fWidth = 0.0f;	//��
		rule->fHeight = 0.0f;	//����
		rule->bUse = false;		//�g�p���Ă��Ȃ����
	}

	//�X�C�b�`�\���̂̏�����
	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		Switch->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		Switch->fWidth = 0.0f;		//��
		Switch->fHeight = 0.0f;		//����
		Switch->bUse = false;		//�g�p���Ă��Ȃ����
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
			rule->nIdx = SetRectangle(s_pTexture[0]);
			
			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(rule->nIdx, pos, size);

			//�؂�ւ��{�^���̐ݒ�
			SetSwitchLeft(rule->pos);	//��
			SetSwitchRight(rule->pos);	//�E

			break;
		}
	}
}

//============================
// �؂�ւ��{�^��(��)�̐ݒ�
//============================
void SetSwitchLeft(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < HALF_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
			//�\���̂̐ݒ�
			Switch->pos = D3DXVECTOR3(pos.x + 400.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// ��`�̐ݒ�
			Switch->nIdx = SetRectangle(s_pTexture[1]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// �؂�ւ��{�^��(�E)�̐ݒ�
//============================
void SetSwitchRight(D3DXVECTOR3 pos)
{
	for (int nCnt = HALF_SWITCH; nCnt < MAX_SWITCH; nCnt++)
	{
		Switch *Switch = s_Switch + nCnt;

		if (Switch->bUse == false)
		{//�g�p���Ă��Ȃ��Ȃ�
		 //�\���̂̐ݒ�
			Switch->pos = D3DXVECTOR3(pos.x + 600.0f, pos.y, pos.z);
			Switch->fWidth = 100.0f;
			Switch->fHeight = 100.0f;
			Switch->bUse = true;

			// ��`�̐ݒ�
			Switch->nIdx = SetRectangle(s_pTexture[2]);

			D3DXVECTOR3 size = D3DXVECTOR3(Switch->fWidth, Switch->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(Switch->nIdx, Switch->pos, size);

			break;
		}
	}
}

//============================
// �e�N�X�`���̓_��
//============================
void FlashTexture(int nNumber)
{
	//------------------------------
	// ���ڂ̃e�N�X�`��
	//------------------------------
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

	//----------------
	// ���I��
	//----------------
	if (GetKeyboardTrigger(DIK_A))
	{
		if (s_nTime >= HALF_FLASH)
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber].nIdx, GetColor(COLOR_WHITE));
		}
	}

	//----------------
	// �E�I��
	//----------------
	if (GetKeyboardTrigger(DIK_A))
	{
		if (s_nTime >= HALF_FLASH)
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// ��`�̐F�̐ݒ�
			SetColorRectangle(s_Switch[nNumber + 3].nIdx, GetColor(COLOR_WHITE));
		}
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