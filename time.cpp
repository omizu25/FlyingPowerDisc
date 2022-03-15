//================================
//
// �^�C���̏���
// Author:Seino Riku
//
//================================
#include "time.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;
D3DXVECTOR3 g_posTime;
int g_nTime;				//�ő厞�Ԃ�����
int g_TimenCnt;				//�J�E���^�[

//�X�R�A�̏���������
void InitTime(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;				//for����p�̃J�E���g

								//���_���W�ւ̃|�C���^
	VERTEX_2D*pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureTime);

	// �X�R�A�̏��̏�����
	g_posTime = D3DXVECTOR3(800.0f, 50.0f, 0.0f);
	g_nTime = 0;
	g_TimenCnt = 100;
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - MAX_SIZE / 2 + (nCntTime * 40), g_posTime.y - MAX_SIZE / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + MAX_SIZE / 2 + (nCntTime * 40), g_posTime.y - MAX_SIZE / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - MAX_SIZE / 2 + (nCntTime * 40), g_posTime.y + MAX_SIZE / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + MAX_SIZE / 2 + (nCntTime * 40), g_posTime.y + MAX_SIZE / 2, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//�X�R�A��
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//�X�R�A��
void UpdateTime(void)
{
	//���_���W�ւ̃|�C���^
	VERTEX_2D *pVtx;
	int nNumber[3];
	int nCntTime;

	//�f�N�������g�ŃJ�E���g
	g_TimenCnt--;
	if (g_TimenCnt == 0)
	{
		g_TimenCnt = 100;
		g_nTime--;
	}

	nNumber[0] = g_nTime % 1000 / 100;
	nNumber[1] = g_nTime % 100 / 10;
	nNumber[2] = g_nTime % 10 / 1;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��

	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//�X�R�A��
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntTime * 4,
			2);
	}
}

//�X�R�A��
void SetTime(int nTime)
{
	//���_���W�ւ̃|�C���^
	VERTEX_2D *pVtx;
	int nNumber[3];
	int nCntTime;

	g_nTime = nTime;

	nNumber[0] = g_nTime % 1000 / 100;
	nNumber[1] = g_nTime % 100 / 10;
	nNumber[2] = g_nTime % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber[nTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber[nTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber[nTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber[nTime] + 0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//�X�R�A��
void AddTime(int nValue)
{
	//���_���W�ւ̃|�C���^
	VERTEX_2D*pVtx;
	int nNumber[2];	//�e���̐������i�[

	g_nTime -= nValue;

	nNumber[0] = g_nTime % 1000 / 100;
	nNumber[1] = g_nTime % 100 / 10;
	nNumber[2] = g_nTime % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)

	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//�X�R�A��
int GetTime(void)
{
	return g_nTime;
}