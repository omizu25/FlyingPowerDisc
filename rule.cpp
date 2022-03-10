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

//�}�N����`
#define MAX_RULE	(3)		//���[���̍ő吔

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^
static int	s_nIdx;	// ��`�̃C���f�b�N�X
static Rule s_Rule[MAX_RULE];	//�\���̂̎擾
static int s_nTime;				//�_�ł̎���

//============================
// ���[���I����ʂ̏�����
//============================
void InitRule(void)
{
	// ��`�̏�����
	InitRectangle();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//------------------------------
	//	�e�N�X�`���̓ǂݍ���
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Title_blue.png",
							  &s_pTexture);

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
	//�e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// ��`���g���̂��~�߂�
	StopUseRectangle(s_nIdx);
}

//============================
// ���[���I����ʂ̍X�V
//============================
void UpdateRule(void)
{
	s_nTime++;		//�^�C���̉��Z
	s_nTime %= 40;	//�^�C���̏�����

	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == true)
		{//�g�p���Ă���Ȃ�
			//------------------------------
			//	�e�N�X�`���̓_��
			//------------------------------
			if (s_nTime >= 20)
			{
				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
			else
			{
				// ��`�̐F�̐ݒ�
				SetColorRectangle(s_nIdx, GetColor(COLOR_WHITE));
			}
		}
	}
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
			rule->fWidth = 50.0f;
			rule->fHeight = 25.0f;
			rule->bUse = true;

			// ��`�̐ݒ�
			s_nIdx = SetRectangle(s_pTexture);

			D3DXVECTOR3 size = D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);

			// ��`�̈ʒu�̐ݒ�
			SetPosRectangle(s_nIdx, pos, size);

			break;
		}
	}
}