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
const int	MAX_CURSOR = 16;	// カーソルの最大数
const float	ROT_SPEED = -0.15f;	// 回転速度

/*↓ メニュー ↓*/
typedef struct
{
	D3DXVECTOR3	pos;		// 位置
	D3DXVECTOR3	rot;		// 向き
	int			nNumUse;	// 使用数
	int			nIdx;		// 矩形のインデックス
	float		fTop;		// 上端
	float		fWidth;		// 幅
	float		fHeight;	// 高さ
	float		fInterval;	// 選択肢の間隔
	bool		bRotation;	// 回転するかどうか
	bool		bDraw;		// 描画するかどうか
	bool		bUse;		// 使用しているかどうか
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

		// 使うのを止める
		StopUseRectangle(pCursor->nIdx);
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateCursor(void)
{
	for (int i = 0; i < MAX_CURSOR; i++)
	{
		Cursor *pCursor = &s_aCursor[i];

		if (!pCursor->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		if (!pCursor->bRotation)
		{// 回転しない
			continue;
		}

		/*↓ 回転する ↓*/

		pCursor->rot.z += ROT_SPEED;

		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pCursor->nIdx, pCursor->pos, pCursor->rot, pCursor->fWidth, pCursor->fHeight);
	}
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
		pCursor->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pCursor->fInterval = (cursor.fBottom - cursor.fTop) / (cursor.nNumUse + 1);
		pCursor->pos = D3DXVECTOR3(cursor.fPosX, cursor.fTop + (pCursor->fInterval * (cursor.nSelect + 1)), 0.0f);
		pCursor->fWidth = cursor.fWidth;
		pCursor->fHeight = cursor.fHeight;
		pCursor->fTop = cursor.fTop;
		pCursor->bRotation = cursor.bRotation;
		pCursor->bDraw = true;
		pCursor->bUse = true;

		// 矩形の設定
		pCursor->nIdx = SetRectangle(cursor.texture);

		// 矩形の回転する位置の設定
		SetRotationPosRectangle(pCursor->nIdx, pCursor->pos, pCursor->rot, cursor.fWidth, cursor.fHeight);

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

	// 矩形の回転する位置の設定
	SetRotationPosRectangle(pCursor->nIdx, pCursor->pos, pCursor->rot, pCursor->fWidth, pCursor->fHeight);
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetCursor(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_CURSOR);

	Cursor *pCursor = &s_aCursor[nIdx];

	// 使うのを止める
	StopUseRectangle(pCursor->nIdx);
	
	pCursor->bUse = false;
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawCursor(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_CURSOR);

	Cursor *pCursor = &s_aCursor[nIdx];

	if (!pCursor->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	// 矩形の描画するかどうか
	SetDrawRectangle(pCursor->nIdx, bDraw);
	
	pCursor->bDraw = bDraw;
}