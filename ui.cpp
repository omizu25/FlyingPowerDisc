//========================================================
//
//UI����
//Author:����]��
//
//========================================================
#include "ui.h"
#include "game.h"
//====================================
//�}�N����`
//====================================
#define MAX_UI		128				//UI�̍ő吔
#define NUM_UI		10				//UI�̎��
#define MAX_TIME	60				//�o�Ă��鎞��
//====================================
//�O���[�o���ϐ�
//====================================
LPDIRECT3DTEXTURE9 g_pTextureUi[NUM_UI] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Ui g_aUi[MAX_UI];		//UI�̏��
//====================================
//���C���֐�
//====================================

//====================================
//UI�̏���������
//====================================
void InitUi(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI000.png",
		&g_pTextureUi[0]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI001.png",
		&g_pTextureUi[1]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAME\\FivePoints.png",
		&g_pTextureUi[2]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAME\\ThreePoints.png",
		&g_pTextureUi[3]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI001.png",
		&g_pTextureUi[4]);
	//UI�̏��̏�����
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		g_aUi[nCntUi].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].nCntTime = 0.0f;
		g_aUi[nCntUi].fHeight = 0.0f;
		g_aUi[nCntUi].fWidth = 0.0f;
		g_aUi[nCntUi].nType = 0;
		g_aUi[nCntUi].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		g_aUi[nCntUi].bSwitch = false;	//�����Ă�����Ԃɂ���
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2, 0.0f);
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

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi->Unlock();
}
//====================================
//UI�̏I������
//====================================
void UninitUi(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureUi[NUM_UI] != NULL)
	{
		g_pTextureUi[NUM_UI]->Release();
		g_pTextureUi[NUM_UI] = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}
}
//====================================
//UI�̍X�V����
//====================================
void UpdateUi(void)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		//��ނ��Ƃ̓���(������o��)
		if (g_aUi[nCntUi].nType == 2 && g_aUi[nCntUi].bSwitch == false)
		{//�o�Ă��鏈��
			g_aUi[nCntUi].scale.x += 0.01f;
			//�g�嗦�̒���
			if (g_aUi[nCntUi].scale.x > 1.0f)
			{
				g_aUi[nCntUi].scale.x = 1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 2 && g_aUi[nCntUi].bSwitch)
		{//�����鏈��
			g_aUi[nCntUi].scale.x -= 0.01f;
		}

		//��ނ��Ƃ̓���(�E����o��)
		if (g_aUi[nCntUi].nType == 3 && g_aUi[nCntUi].bSwitch == false)
		{//�o�Ă��鏈��
			g_aUi[nCntUi].scale.x -= 0.01f;
			//�g�嗦�̒���
			if (g_aUi[nCntUi].scale.x < -1.0f)
			{
				g_aUi[nCntUi].scale.x = -1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 3 && g_aUi[nCntUi].bSwitch)
		{//�����鏈��
			g_aUi[nCntUi].scale.x += 0.01f;
		}

		//��ނ��Ƃ̓���(�㉺�ɐL�т�)
		if (g_aUi[nCntUi].nType == 4 && g_aUi[nCntUi].bSwitch == false)
		{//�o�Ă��鏈��
			g_aUi[nCntUi].scale.y += 0.01f;
			//�g�嗦�̒���
			if (g_aUi[nCntUi].scale.y > 1.0f)
			{
				g_aUi[nCntUi].scale.y = 1.0f;
				g_aUi[nCntUi].nCntTime++;
				if (g_aUi[nCntUi].nCntTime == MAX_TIME)
				{
					g_aUi[nCntUi].bSwitch = true;
				}
			}
		}
		else if (g_aUi[nCntUi].nType == 4 && g_aUi[nCntUi].bSwitch)
		{//�����鏈��
			g_aUi[nCntUi].scale.y -= 0.01f;

			if (g_aUi[nCntUi].scale.y <= 0.0f)
			{
				// �Q�[���̏�Ԃ̐ݒ�
				SetGameState(GAMESTATE_START);
			}
		}

		//�g�嗦�̒���
		if (g_aUi[nCntUi].scale.x > 1.0f)
		{
			g_aUi[nCntUi].scale.x = 1.0f;
			g_aUi[nCntUi].bSwitch = true;
		}
		else if(g_aUi[nCntUi].scale.y > 1.0f)
		{
			g_aUi[nCntUi].scale.y = 1.0f;
			g_aUi[nCntUi].bSwitch = true;
		}
		if (g_aUi[nCntUi].scale.x < -1.0f)
		{
			g_aUi[nCntUi].scale.x = -1.0f;
		}
		else if (g_aUi[nCntUi].scale.y < -1.0f)
		{
			g_aUi[nCntUi].scale.y = -1.0f;
		}

		if (g_aUi[nCntUi].nType == 2)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
		}
		else if (g_aUi[nCntUi].nType == 3)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - (g_aUi[nCntUi].fWidth / 2 * -g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth  / 2, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - (g_aUi[nCntUi].fWidth / 2 * -g_aUi[nCntUi].scale.x), (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth  / 2 , (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
		}
		else if (g_aUi[nCntUi].nType == 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2),							g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2),							g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + (g_aUi[nCntUi].fWidth / 2 * g_aUi[nCntUi].scale.x), g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2 * g_aUi[nCntUi].scale.y, 0.0f);
		}
		if (g_aUi[nCntUi].nType == 3)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * -g_aUi[nCntUi].scale.x);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f * g_aUi[nCntUi].scale.x);
		}

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi->Unlock();
}
//====================================
//UI�̕`�揈��
//====================================
void DrawUi(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse)
		{//UI���g�p����Ă���
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureUi[g_aUi[nCntUi].nTexture]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);	//�v���~�e�B�u(�|���S��)��
		}
	}
}
//====================================
//UI�̐ݒ菈��
//====================================
void SetUi(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, D3DXVECTOR3 scale,int nTexture)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse == false)
		{//UI���g�p����Ă��Ȃ�
			g_aUi[nCntUi].pos = pos;
			g_aUi[nCntUi].fWidth = fWidth;
			g_aUi[nCntUi].fHeight = fHeight;
			g_aUi[nCntUi].nType = nType;
			g_aUi[nCntUi].scale = scale;
			g_aUi[nCntUi].nTexture = nTexture;
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fWidth / 2) *  g_aUi[nCntUi].scale.x, (g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fHeight / 2) * g_aUi[nCntUi].scale.y, 0.0f);

			g_aUi[nCntUi].bUse = true;		//�g�p���Ă�����
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi->Unlock();
}
//====================================
//UI���̎擾
//====================================
Ui * GetUi(void)
{
	return &g_aUi[0];
}