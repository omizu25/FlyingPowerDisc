//**************************************************
//
// FPD���� ( effect.cpp )
// Author  : hamada ryuuga
//
//**************************************************
#include"effect.h"
#include"player.h"
#include"rectangle.h"
#include"texture.h"

#include <assert.h>

#define NUR_EFFECT (4)

//�X�^�e�B�b�N
static Effect s_aEffect[MAX_EFFECT];
static TEXTURE s_aTexture[EFFECTSTATE_MAX];

//==================================
//������
//==================================
void InitEffect(void)
{
	s_aTexture[EFFECTSTATE_SPIN] = TEXTURE_spin;
	s_aTexture[EFFECTSTATE_SHOOT] = TEXTURE_fire;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�������Y��Ȃ�
		s_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_aEffect[nCntEffect].nLife = 0;
		s_aEffect[nCntEffect].bUse = false;

		// ��`�̐ݒ�
		s_aEffect[nCntEffect].nIdx = SetRectangle(TEXTURE_NONE);

		// ��`��`�悷�邩�ǂ���
		SetDrawRectangle(s_aEffect[nCntEffect].nIdx, false);

		// ��`�̉��Z���������邩�ǂ���
		SetAddRectangle(s_aEffect[nCntEffect].nIdx, true);
	}
}

//==================================
//�j��
//==================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// �g���̂��~�߂�
		StopUseRectangle(s_aEffect[nCntEffect].nIdx);
	}
}

//==================================
//�X�V
//==================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse)
		{
			// ���_�o�b�t�@�̎擾
			pVtxBuff = GetVtxBuffRectangle(s_aEffect[nCntEffect].nIdx);

			//���_�o�b�t�@���A�����b�N
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			s_aEffect[nCntEffect].pos += s_aEffect[nCntEffect].move;

			SetNormalpos2d(pVtx, s_aEffect[nCntEffect].pos.x - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.x + s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y + s_aEffect[nCntEffect].fRadeius);

			s_aEffect[nCntEffect].col.a -= 1.0f / s_aEffect[nCntEffect].nMaxLife;

			if (s_aEffect[nCntEffect].col.a <=0.0f)
			{
				s_aEffect[nCntEffect].bUse = false;
			}

			// �e�N�X�`���̐ݒ�
			SetVtxAnimTex(pVtx, &s_aEffect[nCntEffect].AnimTex);
			SetTex2d(pVtx, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)*s_aEffect[nCntEffect].AnimTex.nPatternX
				, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)* (s_aEffect[nCntEffect].AnimTex.nPatternX + 1)
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*s_aEffect[nCntEffect].AnimTex.nPatternY
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*(s_aEffect[nCntEffect].AnimTex.nPatternY + 1));

			//���_�J���[�̐ݒ�
			pVtx[0].col = s_aEffect[nCntEffect].col;
			pVtx[1].col = s_aEffect[nCntEffect].col;
			pVtx[2].col = s_aEffect[nCntEffect].col;
			pVtx[3].col = s_aEffect[nCntEffect].col;

			//���_�o�b�t�@���A�����b�N
			pVtxBuff->Unlock();
		}
	}

	
}
void DrawEffect(void)
{
}

//==================================
//�Z�b�g�@�����@���W�A�F�A�g�������^�C�v�A�����A�T�C�Y
//==================================
void SetEffect(D3DXVECTOR3 pos,  D3DXCOLOR col, EFFECTSTATE nType, int life, float size)
{
	assert(nType >= 0 && nType < EFFECTSTATE_MAX);

	int nCntEffect;
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!s_aEffect[nCntEffect].bUse)
		{
			//�G���g�p����ĂȂ��ꍇ
			s_aEffect[nCntEffect].fRadeius = size;
			s_aEffect[nCntEffect].pos.x = pos.x;
			s_aEffect[nCntEffect].pos.y = pos.y;
			s_aEffect[nCntEffect].pos.z = pos.z;

			// ���_�o�b�t�@�̎擾
			pVtxBuff = GetVtxBuffRectangle(s_aEffect[nCntEffect].nIdx);

			//���_�o�b�t�@���A�����b�N
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

													//���_���W
			SetNormalpos2d(pVtx, s_aEffect[nCntEffect].pos.x - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.x + s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y - s_aEffect[nCntEffect].fRadeius,
				s_aEffect[nCntEffect].pos.y + s_aEffect[nCntEffect].fRadeius);

			s_aEffect[nCntEffect].nType = nType;
			s_aEffect[nCntEffect].nLife = life;
			s_aEffect[nCntEffect].bUse = true;
			s_aEffect[nCntEffect].col = col;
			s_aEffect[nCntEffect].nMaxLife = life;

			switch (nType)
			{
			case EFFECTSTATE_SHOOT:
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 10;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 1;
				break;
			case EFFECTSTATE_SPIN:
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 5;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 3;
				break;
			default:
				assert(false);
				s_aEffect[nCntEffect].AnimTex.nDivisionX = 1;
				s_aEffect[nCntEffect].AnimTex.nDivisionY = 1;
				break;
			}

			SetTex2d(pVtx, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)*s_aEffect[nCntEffect].AnimTex.nPatternX
				, (1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionX)* (s_aEffect[nCntEffect].AnimTex.nPatternX + 1)
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*s_aEffect[nCntEffect].AnimTex.nPatternY
				, 1.0f / s_aEffect[nCntEffect].AnimTex.nDivisionY*(s_aEffect[nCntEffect].AnimTex.nPatternY + 1));

			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			//���_�o�b�t�@���A�����b�N
			pVtxBuff->Unlock();

			// ��`��`�悷�邩�ǂ���
			SetDrawRectangle(s_aEffect[nCntEffect].nIdx, true);

			// ��`�̃e�N�X�`���̕ύX
			ChangeTextureRectangle(s_aEffect[nCntEffect].nIdx, s_aTexture[nType]);

			break;
		}
	}
}

//=========================================
// ���W�ݒ�
//Auther�Fhamada ryuuga
//=========================================
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}
//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//Auther�Fhamada ryuuga
//---------------------------------------
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}

//--------------------------------------------------
// �e�N�X�`���A�j���[�V����
// Author�FHamada Ryuga
//--------------------------------------------------
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex)
{
	pAnimTex->nCnt++;
	pAnimTex->nSpeed = 5;//�����ő��x����
	if ((pAnimTex->nCnt % pAnimTex->nSpeed) == 0)
	{

		pAnimTex->nPatternX++;

		if (pAnimTex->nPatternX >= pAnimTex->nDivisionX)
		{
			pAnimTex->nPatternX = 0;
			pAnimTex->nPatternY++;
		}
		if (pAnimTex->nPatternY >= pAnimTex->nDivisionY)
		{
			pAnimTex->nPatternY = 0;
		}

		SetTex2d(pVtx,
			1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX
			, 1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX + (1.0f / pAnimTex->nDivisionX)
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY + 1.0f / pAnimTex->nDivisionY);
	}
}
