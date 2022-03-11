//**************************************************
//
// FPG制作 ( disc.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _DISC_H_	// このマクロ定義がされてなかったら
#define _DISC_H_	// ２重インクルード防止のマクロ定義

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitDisc(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitDisc(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateDisc(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawDisc(void);

#endif // !_DISC_H_
