//========================================================
//
//�`���[�g���A�����
//Author:����]��
//
//========================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "mode.h"

//====================================
//�O���[�o���ϐ�
//====================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//���_�o�b�t�@�ւ̃|�C���^
//====================================
//���C���֐�
//====================================

//====================================
//�`���[�g���A����ʂ̏���������
//====================================
void InitTutorial(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial000.png",
		&g_pTextureTutorial);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}
//====================================
//�`���[�g���A����ʂ̏I������
//====================================
void UninitTutorial(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}
//====================================
//�`���[�g���A����ʂ̍X�V����
//====================================
void UpdateTutorial(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//ENTER�L�[�������ꂽ
	 //�t�F�[�h�̎擾����
		FADE p_fade = GetFade();
		if (p_fade == FADE_NONE)
		{
			ChangeMode(MODE_TITLE);
		}
	}
}
//====================================
//�`���[�g���A����ʂ̕`�揈��
//====================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u(�|���S��)��
}