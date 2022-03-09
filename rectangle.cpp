//==================================================
// 
// FPG���� ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "rectangle.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_RECTANGLE		(1024)		// ��`�̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9			*pTexture;			// �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9		*pVtxBuff;			// ���_�o�b�t�@
	int							nBuffLength;		// �o�b�t�@�̒���
	bool						bUse;				// �g�p���Ă��邩�ǂ���
}RectAngle;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
RectAngle		s_aRectAngle[MAX_RECTANGLE];		// ��`�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRectAngle(void)
{
	// �������̃N���A
	memset(s_aRectAngle, 0, sizeof(s_aRectAngle));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitRectAngle(void)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->pTexture != NULL)
		{// �e�N�X�`���̉��
			pRectAngle->pTexture = NULL;
		}

		if (pRectAngle->pVtxBuff != NULL)
		{// ���_�o�b�t�@�̉��
			pRectAngle->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawRectAngle(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (!pRectAngle->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, *pRectAngle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		if (*pRectAngle->pTexture != NULL)
		{// �e�N�X�`��������
			pDevice->SetTexture(0, *pRectAngle->pTexture);
		}
		else
		{// �e�N�X�`�����Ȃ�
			pDevice->SetTexture(0, NULL);
		}

		for (int j = 0; j < pRectAngle->nBuffLength; j++)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				j * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetRectAngle(LPDIRECT3DTEXTURE9 *pTexture, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff, const int nBuffLength)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		if (pTexture != NULL)
		{// �e�N�X�`��������
			pRectAngle->pTexture = pTexture;
		}
		else
		{// �e�N�X�`�����Ȃ�
			pRectAngle->pTexture = NULL;
		}

		pRectAngle->pVtxBuff = pVtxBuff;
		pRectAngle->nBuffLength = nBuffLength;
		pRectAngle->bUse = true;

		break;
	}
}

//--------------------------------------------------
// �g�p����߂�
//--------------------------------------------------
void StopUseRectAngle(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		RectAngle *pRectAngle = &s_aRectAngle[i];

		if (pRectAngle->pVtxBuff != pVtxBuff)
		{// ���_�o�b�t�@���Ⴄ
			continue;
		}

		/*�� ���_�o�b�t�@������ ��*/

		pRectAngle->bUse = false;

		break;
	}
}