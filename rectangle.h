//==================================================
// 
// FPG制作 ( rectangle.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _RECTANGLE_H_		//このマクロ定義がされてなかったら
#define _RECTANGLE_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitRectAngle(void);
void UninitRectAngle(void);
void DrawRectAngle(void);
void SetRectAngle(LPDIRECT3DTEXTURE9 *pTexture, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff, const int nBuffLength);
void StopUseRectAngle(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff);

#endif // !_RECTANGLE_H_
