//================================
//
// ���[���I����ʂ̏���
// Author:Sato Teruto
//
//================================

//�C���N���[�h
#include "rule.h"
#include "main.h"

//�}�N����`
#define MAX_RULE	(3)		//���[���̍ő吔

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Rule s_Rule[MAX_RULE];	//�\���̂̎擾
static int s_nTime;				//�_�ł̎���

//============================
// ���[���I����ʂ̏�����
//============================
void InitRule(void)
{
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

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RULE,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------
	//	���_���̐ݒ�
	//------------------------------
	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
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

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================
// ���[���I����ʂ̍X�V
//============================
void UpdateRule(void)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================
// ���[���I����ʂ̕`��
//============================
void DrawRule(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_RULE; nCnt++)
	{
		Rule *rule = s_Rule + nCnt;

		if (rule->bUse == true)
		{//�g�p���Ă���Ȃ�
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//���U���g�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								   nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								   2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//============================
// ���[���I����ʂ̐ݒ�
//============================
void SetRule(D3DXVECTOR3 pos)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

			//���_���W�̐ݒ�
			pVtx[0].pos = rule->pos + D3DXVECTOR3(-rule->fWidth, rule->fHeight, 0.0f);
			pVtx[1].pos = rule->pos + D3DXVECTOR3(rule->fWidth, rule->fHeight, 0.0f);
			pVtx[2].pos = rule->pos + D3DXVECTOR3(-rule->fWidth, -rule->fHeight, 0.0f);
			pVtx[3].pos = rule->pos + D3DXVECTOR3(rule->fWidth, -rule->fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}