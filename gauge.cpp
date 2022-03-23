//**************************************************
//
// FPD���� ( gauge.cpp )
// Author  : katsuki mizuki
//
//**************************************************
//==================================================
// �C���N���[�h
//==================================================
#include "gauge.h"
#include "rectangle.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_GAUGE = 256;	// �Q�[�W�̍ő吔

typedef struct
{
	D3DXVECTOR3	pos;			// �ʒu
	int			nIdx;			// ��`�̃C���f�b�N�X
	float		fWidth;			// ��
	float		fHeight;		// ����
	float		fWidthDest;		// �ړI�̕�
	float		fHeightDest;	// �ړI�̍���
	bool		bUse;			// �g�p���Ă��邩�ǂ���
}Gauge;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Gauge	s_gauge[MAX_GAUGE];	// �Q�[�W�̏��
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGauge(void)
{
	// �������̃N���A
	memset(&s_gauge[0], 0, sizeof(s_gauge));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGauge(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (!pGauge->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		// �g���̂��~�߂�
		StopUseRectangle(pGauge->nIdx);
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGauge(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawGauge(void)
{
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_gauge[i];

		if (pGauge->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		pGauge->pos = pos;
		pGauge->fWidth = fWidth;
		pGauge->fHeight = fHeight;
		pGauge->fWidthDest = fWidth;
		pGauge->fHeightDest = fHeight;
		pGauge->bUse = true;

		// ��`�̐ݒ�
		pGauge->nIdx = SetRectangle(TEXTURE_NONE);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(pGauge->nIdx, pos, D3DXVECTOR3(fWidth, fHeight, 0.0f));

		// ��`�̐F�̐ݒ�
		SetColorRectangle(pGauge->nIdx, col);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// ����
//--------------------------------------------------
void SubGauge(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	assert(nIdx >= 0 && nIdx < MAX_GAUGE);

	Gauge *pGauge = &s_gauge[nIdx];

	pGauge->pos = pos;
	pGauge->fWidth = fWidth;
	pGauge->fHeight = fHeight;
	pGauge->fWidthDest = fWidth;
	pGauge->fHeightDest = fHeight;

	D3DXVECTOR3 size = D3DXVECTOR3(pGauge->fWidth, pGauge->fHeight, 0.0f);

	// ��`�̈ʒu�̐ݒ�
	SetPosRectangle(pGauge->nIdx, pGauge->pos, size);
}
