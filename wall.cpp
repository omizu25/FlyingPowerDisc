//====================
//���אݒ�
//autho hamada ryuuga
//====================

#include"main.h"
#include"wall.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include"effect.h"
#include <stdio.h>

//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTextureWall[MAXWALLTYPE] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffWall = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3	s_rotWall;	//����
static WALL Type[MAXWALL];
//�v�Z�p�֐�
//void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
 //=======================
 //���ׂ̏������ݒ�
 //=======================
void InitWall(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	 //�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAXWALL * 4,//�����͊m�ۂ���o�b�t�@�̃T�C�Ysizeof(VERTEX_2D)*�ł����͂��܂�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffWall,
		NULL);
	
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int count = 0; count< MAXWALL; count++,pVtx+=4)
	{
	
		if (count == 0)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT - WALLWIDE + 5.0f, SCREEN_HEIGHT + 5.0f);
			Type[count] = WALL_UP;
		}
		if (count == 1)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, 130.0f , 130.0f + WALLWIDE);
			Type[count] = WALL_DOWN;
		}

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuffWall->Unlock();
	}
}

//===================
//�I������
//===================
void UninitWall(void)
{
	//�T�E���h��~
	StopSound();

	//�e�N�X�`���̔j��
	for (int Tex = 0; Tex < MAXWALLTYPE; Tex++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureWall[Tex] != NULL)
		{
			s_pTextureWall[Tex]->Release();
			s_pTextureWall[Tex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffWall != NULL)
	{
		s_pVtxBuffWall->Release();
		s_pVtxBuffWall = NULL;
	}

}

//===================
//�X�V����
//===================
void UpdateWall(void)
{
}
//===================
//�`�揈��
//===================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffWall, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int count = 0; count < MAXWALL; count++)
	{	
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL); //);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, count * 4, 2);	
	}
}

//----------------------------
//Wall����
//----------------------------
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);


	bool bIsLanding = false;
	for (int count = 0; count< MAXWALL; count++, pVtx += 4)
	{
			//V1 move
			D3DXVECTOR3 vecMove = *pPos - *pPosOld;
			D3DXVECTOR3 vecWall;
			D3DXVECTOR3 vecPos;
			if (Type[count] == WALL_UP)
			{
				//�x�N�g��S2 V2
				vecWall = D3DXVECTOR3(pVtx[1].pos) - D3DXVECTOR3(pVtx[0].pos) + (vecMove);


				//�x�N�g�����݂�POS�Ǝn�_�܂ł̋���
				vecPos = *pPos - D3DXVECTOR3(pVtx[0].pos);
			}
			else
			{
				//�x�N�g��S2 V2
				vecWall = D3DXVECTOR3(pVtx[2].pos) - D3DXVECTOR3(pVtx[3].pos) + (vecMove);


				//�x�N�g�����݂�POS�Ǝn�_�܂ł̋���
				vecPos = *pPos - D3DXVECTOR3(pVtx[3].pos);
			}
			//�O�όv�Z
			float vecLine = Vec2Cross(&vecPos, &vecWall);

			if (vecLine <= 0.0f)
			{
				bIsLanding = true;
				D3DXVECTOR3 vecOld;
				D3DXVECTOR3 vecV3;
				if (Type[count] == WALL_UP)
				{
					//�x�N�g��V
					vecOld = D3DXVECTOR3(pVtx[0].pos) - *pPosOld;

					//V3 
					vecV3 = D3DXVECTOR3(pVtx[0].pos) - D3DXVECTOR3(pVtx[3].pos);
				}
				else
				{
					//�x�N�g��V
					vecOld = D3DXVECTOR3(pVtx[3].pos) - *pPosOld;

					//V3 
					vecV3 = D3DXVECTOR3(pVtx[3].pos) - D3DXVECTOR3(pVtx[1].pos);
				}
				//�@��
				D3DXVECTOR3 vecB;

				//A��B�̖@�������߂���
				D3DXVec3Cross(&vecB, &vecWall, &vecV3);

				//vecB ���m�[�}���C�Y���āA���� 1�ɂ���B
				D3DXVec3Normalize(&vecB, &vecB);

				D3DXVECTOR3 reversevecMove = -vecMove;
				//A��B�̓��ς����߂�
				float Dot = Vec2Dot(&reversevecMove, &vecB);

				//����
				D3DXVECTOR3 vecC = vecB*Dot;

				//�v�Z�p�̔�
				float vecCalculation[2];

				//T1���߂���
				vecCalculation[0] = Vec2Cross(&vecOld, &vecWall);
				vecCalculation[1] = Vec2Cross(&vecMove, &vecWall);

				//posOld�����_�܂ł̋���
				float t1 = vecCalculation[0] / vecCalculation[1];

				//T2���߂���
				vecCalculation[0] = Vec2Cross(&vecOld, &vecMove);
				vecCalculation[1] = Vec2Cross(&vecMove, &vecWall);

				//�ǂ̎��_�����_�܂ł̋���
				float t2 = vecCalculation[0] / vecCalculation[1];

				//��
				float eps = 0.00001f;

				if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
				{//�������ĂȂ��Ƃ��̔�
				 //���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
				else
				{//�������Ă�Ƃ�
					pPos->x = (pPosOld->x + vecMove.x*t1) + (vecC.x*5.5f);
					pPos->y = (pPosOld->y - vecMove.y*t1) - (vecC.y*5.5f);
					bIsLanding = true;
					//���Ƃ�����Move���]������΂���//�܂�����
					SetEffect(*pPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTSTATE_SPARK, 50, 150.0f, false);
					PlaySound(SOUND_LABEL_WALLHIT);

					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				}
			}
		}
	
	// ���_���W���A�����b�N
	s_pVtxBuffWall->Unlock();

	return bIsLanding;

}

// 2D�x�N�g���̊O��
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}

// 2D�x�N�g���̊O��
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->x + v1->y * v2->y;
}