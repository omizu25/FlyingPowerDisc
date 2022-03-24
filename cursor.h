//**************************************************
//
// FPD制作 ( Cursor.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _CURSOR_H_	//このマクロ定義がされてなかったら
#define _CURSOR_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "texture.h"

//==================================================
// 構造体
//==================================================

/*↓ メニューの引数 ↓*/
typedef struct
{
	TEXTURE	texture;	// テクスチャ
	int		nNumUse;	// 使用数
	int		nSelect;	// 選ばれている
	float	fPosX;		// Xの位置
	float	fTop;		// 上端
	float	fBottom;	// 下端
	float	fWidth;		// 幅
	float	fHeight;	// 高さ
	bool	bRotation;	// 回転するかどうか
}CursorArgument;

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCursor(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitCursor(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateCursor(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawCursor(void);

//--------------------------------------------------
// 設定
// 引数  : CursorArgument &menu / 構造体 カーソルの情報
// 返値  ; int / 何番目かのインデックス
//--------------------------------------------------
int SetCursor(const CursorArgument &cursor);

//--------------------------------------------------
// 位置の変更
// 引数1  : int nIdx / 配列のインデックス
// 引数2  : int nSelect / 選ばれている番号
//--------------------------------------------------
void ChangePosCursor(int nIdx, int nSelect);

//--------------------------------------------------
// リセット
// 引数  : int nIdx / 配列のインデックス
//--------------------------------------------------
void ResetCursor(int nIdx);

#endif // !_CURSOR_H_
