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
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXVECTOR3		move;			// �ړ���
	float			fLength;		// �Ίp���̒���
	float			fAngle;			// �Ίp���̊p�x
	bool			bUse;			// �g�p���Ă邩�ǂ���
}Disc;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^
static Disc							s_disc;					// �f�B�X�N�̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void OffScreen(void);

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

	s_disc.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	s_disc.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_disc.move = D3DXVECTOR3(5.0f, 5.0f, 0.0f);

	float fSize = DISC_SIZE * 0.5f;

	// �Ίp���̒������Z�o����
	s_disc.fLength = sqrtf((fSize * fSize) + (fSize * fSize));

	// �Ίp���̊p�x���Z�o����
	s_disc.fAngle = atan2f(fSize, fSize);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// ��`�̐ݒ�
	//SetRectangle(&s_pTexture, &s_pVtxBuff, 1);
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
	s_disc.rot.z += 0.25f;

	if (s_disc.rot.z >= D3DX_PI)
	{// 3.14���傫��
		s_disc.rot.z -= D3DX_PI * 2.0f;
	}
	else if (s_disc.rot.z <= -D3DX_PI)
	{// -3.14��菬����
		s_disc.rot.z += D3DX_PI * 2.0f;
	}

	s_disc.pos += s_disc.move;

	// ��ʊO
	OffScreen();

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (-D3DX_PI + s_disc.fAngle)) * s_disc.fLength);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (D3DX_PI + (s_disc.fAngle * -1.0f))) * s_disc.fLength);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + (s_disc.fAngle * -1.0f)) * s_disc.fLength);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_disc.pos.x + (sinf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.y = s_disc.pos.y + (cosf(s_disc.rot.z + s_disc.fAngle) * s_disc.fLength);
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// ��ʊO
//--------------------------------------------------
static void OffScreen(void)
{
	float fSize = DISC_SIZE * 0.5f;

	if (s_disc.pos.y >= SCREEN_HEIGHT - fSize)
	{// ��
		s_disc.pos.y = SCREEN_HEIGHT - fSize;
		s_disc.move.y *= -1.0f;
	}
	else if (s_disc.pos.y <= 0.0f + fSize)
	{// ��
		s_disc.pos.y = 0.0f + fSize;
		s_disc.move.y *= -1.0f;
	}

	if (s_disc.pos.x >= SCREEN_WIDTH - fSize)
	{// �E
		s_disc.pos.x = SCREEN_WIDTH - fSize;
		s_disc.move.x *= -1.0f;
	}
	else if (s_disc.pos.x <= 0.0f + fSize)
	{// ��
		s_disc.pos.x = 0.0f + fSize;
		s_disc.move.x *= -1.0f;
	}
}
