//**************************************************
// 
// FPD���� ( number.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "number.h"
#include "rectangle.h"
#include "texture.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_NUMBER = 256;	// ���̍ő吔
const int	MAX_DIGIT = 8;		// �����̍ő吔
const int	TEX_DIVIDE = 10;	// �e�N�X�`���̕�����

typedef struct
{
	int		nNumber[MAX_DIGIT];	// ��
	int		nDigit;				// ����
	int		nIdx[MAX_DIGIT];	// ��`�̃C���f�b�N�X
	bool	bUse;				// �g�p���Ă��邩�ǂ���
}Number;
}// namesapce�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Number	s_Number[MAX_NUMBER];	// ���̏��
int		s_nDigit;				// �����v�Z�p
}// namesapce�͂����܂�

 //==================================================
 // �X�^�e�B�b�N�֐��v���g�^�C�v�錾
 //==================================================
namespace
{
void SetOneDigitNumber(Number *pNumber, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col);
}// namespace�͂����܂�


//--------------------------------------------------
// ������
//--------------------------------------------------
void InitNumber(void)
{
	// �������̃N���A
	memset(&s_Number, NULL, sizeof(s_Number));

	s_nDigit = 0;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitNumber(void)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		for (int j = 0; j < pNumber->nDigit; j++)
		{
			// �g���̂��~�߂�
			StopUseRectangle(pNumber->nIdx[j]);
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawNumber(void)
{
	
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetNumber(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col, int nNumber)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		// ����
		pNumber->nDigit = DigitNumber(nNumber);

		int nSaveNumber = nNumber;

		for (int j = 0; j < pNumber->nDigit; j++)
		{// �ꌅ���ɕ�����
			pNumber->nNumber[j] = nSaveNumber % 10;
			nSaveNumber /= 10;
		}

		// �ꌅ���̐ݒ�
		SetOneDigitNumber(pNumber, pos, size, col);

		pNumber->bUse = true;

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// �ύX
//--------------------------------------------------
void ChangeNumber(int nIdx, int nNumber)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	if (!pNumber->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	// ����
	pNumber->nDigit = DigitNumber(nNumber);

	int nSaveNumber = nNumber;

	for (int j = 0; j < pNumber->nDigit; j++)
	{// �ꌅ���ɕ�����
		pNumber->nNumber[j] = nSaveNumber % 10;
		nSaveNumber /= 10;
	}

	float fDivide = 1.0f / TEX_DIVIDE;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		float fTex = fDivide * pNumber->nNumber[i];

		// ��`�̃e�N�X�`�����W�̐ݒ�
		SetTexRectangle(pNumber->nIdx[i], D3DXVECTOR2(fTex, fTex + fDivide), D3DXVECTOR2(0.0f, 1.0f));
	}
}

//--------------------------------------------------
// �g���̂��~�߂�
//--------------------------------------------------
void StopUseNumber(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_NUMBER);

	Number *pNumber = &s_Number[nIdx];

	pNumber->bUse = false;

	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// �g���̂��~�߂�
		StopUseRectangle(pNumber->nIdx[i]);
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
int DigitNumber(int nNumber)
{
	s_nDigit = 0;
	int nSaveNumber = nNumber;

	while (1)
	{// �������[�v
		if (nSaveNumber >= 10)
		{// 2���ȏ�
			nSaveNumber /= 10;
			s_nDigit++;
		}
		else
		{// 1��
			s_nDigit++;

			if (s_nDigit > MAX_DIGIT)
			{// �w��̒l���傫��
				s_nDigit = MAX_DIGIT;
			}
			break;
		}
	}

	return s_nDigit;
}

namespace
{
//--------------------------------------------------
// �ꌅ���̐ݒ�
//--------------------------------------------------
void SetOneDigitNumber(Number *pNumber, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col)
{
	for (int i = 0; i < pNumber->nDigit; i++)
	{
		// ��`�̐ݒ�
		pNumber->nIdx[i] = SetRectangle(TEXTURE_Number_0To9);

		float fInterval = (size.x * i) + (size.x * 0.5f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(pNumber->nIdx[i], pos + D3DXVECTOR3(-fInterval, 0.0f, 0.0f), size);

		// ��`�̐F�̐ݒ�
		SetColorRectangle(pNumber->nIdx[i], col);

		float fDivide = 1.0f / TEX_DIVIDE;
		float fTex = fDivide * pNumber->nNumber[i];

		// ��`�̃e�N�X�`�����W�̐ݒ�
		SetTexRectangle(pNumber->nIdx[i], D3DXVECTOR2(fTex, fTex + fDivide), D3DXVECTOR2(0.0f, 1.0f));
	}
}
}// namespace�͂����܂�
