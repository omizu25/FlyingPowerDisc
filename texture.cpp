//**************************************************
// 
// FPG���� ( texture.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "texture.h"

#include <assert.h>

////==================================================
//// �萔
////==================================================
//static const D3DXCOLOR s_COLOR_RED(1.0f, 0.0f, 0.0f, 1.0f);				// ��
//static const D3DXCOLOR s_COLOR_GREEN(0.0f, 1.0f, 0.0f, 1.0f);			// ��
//static const D3DXCOLOR s_COLOR_BLUE(0.0f, 0.0f, 1.0f, 1.0f);			// ��
//static const D3DXCOLOR s_COLOR_YELLOW(1.0f, 1.0f, 0.0f, 1.0f);			// ���F
//static const D3DXCOLOR s_COLOR_PURPLE(1.0f, 0.0f, 1.0f, 1.0f);			// ��
//static const D3DXCOLOR s_COLOR_LIGHTBLUE(0.0f, 1.0f, 1.0f, 1.0f);		// ���F
//static const D3DXCOLOR s_COLOR_WHITE(1.0f, 1.0f, 1.0f, 1.0f);			// ��
//static const D3DXCOLOR s_COLOR_BLACK(0.0f, 0.0f, 0.0f, 1.0f);			// ��
//
//static const D3DXCOLOR* s_Colors[] =
//{// �F�̔z��
//	&s_COLOR_RED,
//	&s_COLOR_GREEN,
//	&s_COLOR_BLUE,
//	&s_COLOR_YELLOW,
//	&s_COLOR_PURPLE,
//	&s_COLOR_LIGHTBLUE,
//	&s_COLOR_WHITE,
//	&s_COLOR_BLACK
//};
//
//static_assert(sizeof(s_Colors) / sizeof(s_Colors[0]) == COLOR_MAX, "aho");
//
////#define ARRAY_LENGTH(array_) (sizeof(array_)/sizeof(array_[0]))
////static_assert(ARRAY_LENGTH(s_Colors) == COLOR_MAX, "aho");
////#define STATIC_ASSERT(a) static_assert(a, #a)
////STATIC_ASSERT(ARRAY_LENGTH(s_Colors) == COLOR_MAX);
//
////--------------------------------------------------
//// �擾
////--------------------------------------------------
//D3DXCOLOR GetColor(COLOR color)
//{
//	assert(color >= 0 && color < COLOR_MAX);
//
//	D3DXToRadian(10.0f);
//
//	return *s_Colors[color];
//}
