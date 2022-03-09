//==================================================
//
// FPD���� ( disc.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "disc.h"
#include "rectangle.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define DISC_SIZE		(150.0f)		// �f�B�X�N�̃T�C�Y

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitDisc(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/disc.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	float fSize = DISC_SIZE * 0.5f;

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos + D3DXVECTOR3(-fSize, -fSize, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fSize, -fSize, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fSize, fSize, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fSize, fSize, 0.0f);

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

	// ��`�̐ݒ�
	SetRectAngle(&s_pTexture, &s_pVtxBuff, 1);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitDisc(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateDisc(void)
{
	
}
