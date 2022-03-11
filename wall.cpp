//====================
//���אݒ�
//autho hamada ryuuga
//====================

#include"main.h"
#include"wall.h"
#include "input.h"
#include"fade.h"
#include"sound.h"
#include <stdio.h>

#define MAXWALL			(1)//�o��ő吔
#define MAXWALLTYPE		(4)//Type�ő吔
#define MOVESPEED		(5.0f)
#define WALLWIDE		(100.0f)
//�X�^�e�B�b�N�ϐ�///�X�^�e�B�b�N���w�b�^�Ɏg���Ȃ�H

static LPDIRECT3DTEXTURE9 s_pTextureWall[MAXWALLTYPE] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffWall = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3	s_rotWall;	//����

//�v�Z�p�֐�
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
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
		SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, 200.0f, 200.0f + WALLWIDE);
	/*	if (count == 0)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT - WALLWIDE, SCREEN_HEIGHT);
		}
		if (count == 1)
		{
			SetNormalpos2d(pVtx, 0.0f, SCREEN_WIDTH, 0.0f , 0.0f+ WALLWIDE);
		}*/

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		


		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



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

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);



	for (int count = 0; count < MAXWALL; count++, pVtx += 4)
	{

	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffWall->Unlock();
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
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *move)
{
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);


	bool bIsLanding = false;
	for (int count = 0; count< MAXWALL; count++, pVtx += 4)
	{

			//V1 move
			D3DXVECTOR3 vecMove = *pPos - *pPosOld;

			//�x�N�g��S2 V2
			D3DXVECTOR3 vecWall = D3DXVECTOR3(pVtx[1].pos) - D3DXVECTOR3(pVtx[0].pos) + (vecMove);

			//�x�N�g�����݂�POS�Ǝn�_�܂ł̋���
			D3DXVECTOR3 vecPos = *pPos - D3DXVECTOR3(pVtx[0].pos);

			//�O�όv�Z
			float vecLine = Vec2Cross(&vecPos, &vecWall);

			if (vecLine <= 0.0f)
			{
				bIsLanding = true;

				//�x�N�g��V
				D3DXVECTOR3 vecOld = D3DXVECTOR3(pVtx[0].pos) - *pPosOld;

				//V3 
				D3DXVECTOR3 vecV3 = D3DXVECTOR3(pVtx[0].pos) - D3DXVECTOR3(pVtx[3].pos);

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
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else
				{//�������Ă�Ƃ�
					pPos->x += (vecMove.x) + (vecC.x*10.0f);
					pPos->y += (vecMove.y) + (vecC.y*10.0f);
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


//���ʂƋ��̏Փˌ㑬�x�Z�o�֐�
///////////////////////////////////////////////////
// ���ʂƋ��̏Փˌ㑬�x�Z�o�֐�
// pColliPos : �Փ˒��̃p�[�e�B�N���̒��S�ʒu
// pVelo : �Փ˂̏u�Ԃ̃p�[�e�B�N���̑��x
// weight : �p�[�e�B�N���̎���
// res : �p�[�e�B�N���̕ǂɑ΂��锽����
// time : ���ˌ�̈ړ��\����
// pNormal : ���ʂ̖@��
// pOut_pos : �p�[�e�B�N���̔��ˌ�ʒu
// pOut_velo : �p�[�e�B�N���̔��ˌ㑬�x�x�N�g��
//
//bool CalcParticlePlaneAfterPos(
//	D3DXVECTOR3 *pColliPos,
//	D3DXVECTOR3 *pVelo,
//	FLOAT res,
//	FLOAT time,
//	D3DXVECTOR3 *pOut_pos,
//	D3DXVECTOR3 *pOut_velo
//)
//{
//	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
//	s_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
//
//
//	D3DXVECTOR3 vecWall = D3DXVECTOR3(pVtx[1].pos) - D3DXVECTOR3(pVtx[0].pos);
//	// ���ˌ㑬�x���Z�o
//	D3DXVECTOR3 N;
//	D3DXVec3Normalize(&N, &vecWall);
//	*pOut_velo = *pVelo - (1 + res)*D3DXVec3Dot(&N, pVelo)*N;
//
//	// �ړ��ʒu���v�Z
//	*pOut_pos = *pColliPos + *pOut_velo * time;
//	// ���_���W���A�����b�N
//	s_pVtxBuffWall->Unlock();
//
//	return true;
//}

// ���˃x�N�g��
//
// out : ���K�����˃x�N�g���i�߂�l�j
// front : �i�s�x�N�g��
// normal: �Փ˓_�ł̖@���x�N�g��
//
//D3DXVECTOR3* calcReflectVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal)
//{
//	D3DXVECTOR3 normal_n;
//	D3DXVec3Normalize(&normal_n, &normal);
//	return D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal_n) * normal_n));
//}

void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
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