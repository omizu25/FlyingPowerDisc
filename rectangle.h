//==================================================
// 
// FPG制作 ( rectangle.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _RECTANGLE_H_		//このマクロ定義がされてなかったら
#define _RECTANGLE_H_		//２重インクルード防止のマクロ定義

//**************************************************
// インクルード
//**************************************************
#include "main.h"

//**************************************************
// プロトタイプ宣言
//**************************************************
//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitRectangle(void);

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void UninitRectangle(void);

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void DrawRectangle(void);

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
int SetRectangle(LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void StopUseRectangle(int nIdx);

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx);

#endif // !_RECTANGLE_H_
