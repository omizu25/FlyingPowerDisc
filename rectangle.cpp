//**************************************************
// 
// FPG���� ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "rectangle.h"

#include <assert.h>

//==================================================
// �}�N����`
//==================================================
#define MAX_RECTANGLE		(1024)		// ��`�̍ő吔

//==================================================
// �\����
//==================================================
typedef struct
{
	LPDIRECT3DTEXTURE9			pTexture;		// �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9		pVtxBuff;		// ���_�o�b�t�@
	bool						bUse;			// �g�p���Ă��邩�ǂ���
}MyRectangle;

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
MyRectangle		s_aRectangle[MAX_RECTANGLE];		// ��`�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRectangle(void)
{
	// �������̃N���A
	memset(s_aRectangle, 0, sizeof(s_aRectangle));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitRectangle(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawRectangle(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle *pRectangle = &s_aRectangle[i];

		if (!pRectangle->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pRectangle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pRectangle->pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetRectangle(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle *pRectangle = &s_aRectangle[i];

		if (pRectangle->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		pRectangle->pTexture = pTexture;
		pRectangle->bUse = true;

		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRectangle->pVtxBuff,
			NULL);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// �g�p����߂�
//--------------------------------------------------
void StopUseRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle *pRectangle = &s_aRectangle[nIdx];

	pRectangle->bUse = false;

	if (pRectangle->pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		pRectangle->pVtxBuff->Release();
		pRectangle->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// ���_�o�b�t�@���擾
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	return s_aRectangle[nIdx].pVtxBuff;
}
