//**************************************************
// 
// FPG���� ( color.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "color.h"

#include <assert.h>

//==================================================
// �萔
//==================================================
static const D3DXCOLOR		s_COLOR_RED(1.0f, 0.0f, 0.0f, 1.0f);			// ��
static const D3DXCOLOR		s_COLOR_GREEN(0.0f, 1.0f, 0.0f, 1.0f);			// ��
static const D3DXCOLOR		s_COLOR_BLUE(0.0f, 0.0f, 1.0f, 1.0f);			// ��
static const D3DXCOLOR		s_COLOR_YELLOW(1.0f, 1.0f, 0.0f, 1.0f);			// ���F
static const D3DXCOLOR		s_COLOR_PURPLE(1.0f, 0.0f, 1.0f, 1.0f);			// ��
static const D3DXCOLOR		s_COLOR_LIGHTBLUE(0.0f, 1.0f, 1.0f, 1.0f);		// ���F
static const D3DXCOLOR		s_COLOR_WHITE(1.0f, 1.0f, 1.0f, 1.0f);			// ��
static const D3DXCOLOR		s_COLOR_BLACK(0.0f, 0.0f, 0.0f, 1.0f);			// ��

static const D3DXCOLOR s_Colors[] =
{// �F�̔z��
	s_COLOR_RED,
	s_COLOR_GREEN,
	s_COLOR_BLUE,
	s_COLOR_YELLOW,
	s_COLOR_PURPLE,
	s_COLOR_LIGHTBLUE,
	s_COLOR_WHITE,
	s_COLOR_BLACK
};

//--------------------------------------------------
// �擾
//--------------------------------------------------
D3DXCOLOR GetColor(COLOR color)
{
	assert(color >= 0 && color < COLOR_MAX);

	return s_Colors[color];
}
