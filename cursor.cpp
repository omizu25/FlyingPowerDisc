//**************************************************
// 
// FPG���� ( cursor.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "input.h"
#include "cursor.h"
#include "fade.h"
#include "color.h"
#include "rectangle.h"
#include "menu.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int	MAX_CURSOR = 16;		// �J�[�\���̍ő吔

/*�� ���j���[ ��*/
typedef struct
{
	D3DXVECTOR3	pos;				// �ʒu
	int			nNumUse;			// �g�p��
	int			nIdx;				// ��`�̃C���f�b�N�X
	float		fTop;				// ��[
	float		fWidth;				// ��
	float		fHeight;			// ����
	float		fInterval;			// �I�����̊Ԋu
	bool		bUse;				// �g�p���Ă��邩�ǂ���
}Cursor;
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
Cursor	s_aCursor[MAX_CURSOR];	// �J�[�\���̏��
}// namespace�͂����܂�


//--------------------------------------------------
// ������
//--------------------------------------------------
void InitCursor(void)
{
	// �������̃N���A
	memset(s_aCursor, 0, sizeof(s_aCursor));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitCursor(void)
{
	for (int i = 0; i < MAX_CURSOR; i++)
	{
		Cursor *pCursor = &s_aCursor[i];

		if (!pCursor->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		StopUseRectangle(pCursor->nIdx);
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateCursor(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawCursor(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetCursor(const CursorArgument &cursor)
{
	assert(cursor.nNumUse >= 0 && cursor.nNumUse < MAX_CURSOR);

	for (int i = 0; i < MAX_CURSOR; i++)
	{
		Cursor *pCursor = &s_aCursor[i];

		if (pCursor->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		pCursor->nNumUse = cursor.nNumUse;

		pCursor->fInterval = (cursor.fBottom - cursor.fTop) / (cursor.nNumUse + 1);
		pCursor->pos = D3DXVECTOR3(cursor.fPosX, cursor.fTop + (pCursor->fInterval * (cursor.nSelect + 1)), 0.0f);
		pCursor->fWidth = cursor.fWidth;
		pCursor->fHeight = cursor.fHeight;
		pCursor->fTop = cursor.fTop;
		pCursor->bUse = true;

		// ��`�̐ݒ�
		pCursor->nIdx = SetRectangle(cursor.texture);

		D3DXVECTOR3 size = D3DXVECTOR3(cursor.fWidth, cursor.fHeight, 0.0f);

		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(pCursor->nIdx, pCursor->pos, size);

		// ��`�̐F�̐ݒ�
		SetColorRectangle(pCursor->nIdx, GetColor(COLOR_WHITE));

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// �ʒu�̕ύX
//--------------------------------------------------
void ChangePosCursor(int nIdx, int nSelect)
{
	assert(nIdx >= 0 && nIdx < MAX_CURSOR);
	assert(nSelect >= 0 && nSelect < MAX_OPTION);

	Cursor *pCursor = &s_aCursor[nIdx];

	if (!pCursor->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	pCursor->pos = D3DXVECTOR3(pCursor->pos.x, pCursor->fTop + (pCursor->fInterval * (nSelect + 1)), 0.0f);

	D3DXVECTOR3 size = D3DXVECTOR3(pCursor->fWidth, pCursor->fHeight, 0.0f);

	// ��`�̈ʒu�̐ݒ�
	SetPosRectangle(pCursor->nIdx, pCursor->pos, size);
}