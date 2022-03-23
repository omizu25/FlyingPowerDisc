//**************************************************
// 
// FPG制作 ( cursor.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
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
// 定義
//==================================================
namespace
{
const int	MAX_CURSOR = 16;		// カーソルの最大数

/*↓ メニュー ↓*/
typedef struct
{
	D3DXVECTOR3	pos;				// 位置
	int			nNumUse;			// 使用数
	int			nIdx;				// 矩形のインデックス
	float		fTop;				// 上端
	float		fWidth;				// 幅
	float		fHeight;			// 高さ
	float		fInterval;			// 選択肢の間隔
	bool		bUse;				// 使用しているかどうか
}Cursor;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Cursor	s_aCursor[MAX_CURSOR];	// カーソルの情報
}// namespaceはここまで


//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCursor(void)
{
	// メモリのクリア
	memset(s_aCursor, 0, sizeof(s_aCursor));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitCursor(void)
{
	for (int i = 0; i < MAX_CURSOR; i++)
	{
		Cursor *pCursor = &s_aCursor[i];

		if (!pCursor->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		StopUseRectangle(pCursor->nIdx);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateCursor(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawCursor(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetCursor(const CursorArgument &cursor)
{
	assert(cursor.nNumUse >= 0 && cursor.nNumUse < MAX_CURSOR);

	for (int i = 0; i < MAX_CURSOR; i++)
	{
		Cursor *pCursor = &s_aCursor[i];

		if (pCursor->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pCursor->nNumUse = cursor.nNumUse;

		pCursor->fInterval = (cursor.fBottom - cursor.fTop) / (cursor.nNumUse + 1);
		pCursor->pos = D3DXVECTOR3(cursor.fPosX, cursor.fTop + (pCursor->fInterval * (cursor.nSelect + 1)), 0.0f);
		pCursor->fWidth = cursor.fWidth;
		pCursor->fHeight = cursor.fHeight;
		pCursor->fTop = cursor.fTop;
		pCursor->bUse = true;

		// 矩形の設定
		pCursor->nIdx = SetRectangle(cursor.texture);

		D3DXVECTOR3 size = D3DXVECTOR3(cursor.fWidth, cursor.fHeight, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(pCursor->nIdx, pCursor->pos, size);

		// 矩形の色の設定
		SetColorRectangle(pCursor->nIdx, GetColor(COLOR_WHITE));

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 位置の変更
//--------------------------------------------------
void ChangePosCursor(int nIdx, int nSelect)
{
	assert(nIdx >= 0 && nIdx < MAX_CURSOR);
	assert(nSelect >= 0 && nSelect < MAX_OPTION);

	Cursor *pCursor = &s_aCursor[nIdx];

	if (!pCursor->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pCursor->pos = D3DXVECTOR3(pCursor->pos.x, pCursor->fTop + (pCursor->fInterval * (nSelect + 1)), 0.0f);

	D3DXVECTOR3 size = D3DXVECTOR3(pCursor->fWidth, pCursor->fHeight, 0.0f);

	// 矩形の位置の設定
	SetPosRectangle(pCursor->nIdx, pCursor->pos, size);
}