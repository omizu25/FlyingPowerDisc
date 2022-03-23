//**************************************************
//
// FPD制作 ( result.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "result.h"
#include "player.h"
#include "rectangle.h"
#include "game.h"
#include "mode.h"
#include "input.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float	MAX_WIDTH = 200.0f;		// 幅の最大値
const float	MAX_HEIGHT = 125.0f;	// 高さの最大値
const int	LIMIT_TIME = 300;		// 時間の限界

/*↓ 勝敗 ↓*/

typedef struct
{
	int		nIdx;		// 矩形のインデックス
	bool	bResult;	// 勝敗 [ true : 勝ち / false : 負け ]
	bool	bDraw;		// 描画するかどうか
}Result;

/*↓ 引き分け ↓*/

typedef struct
{
	int		nIdx;		// 矩形のインデックス
	bool	bDraw;		// 描画するかどうか
}Draw;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Result	s_result[MAXPLAYER];	// 勝敗の情報
Draw	s_draw;					// 引き分けの情報
int		s_nTime;				// 時間

}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_result[i];

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5f, 0.0f);

		switch (i)
		{
		case 0:
			pos.x = SCREEN_WIDTH * 0.25f;
			break;

		case 1:
			pos.x = SCREEN_WIDTH * 0.75f;
			break;
		
		default:
			assert(false);
			break;
		}

		// 矩形の設定
		pResult->nIdx = SetRectangle(TEXTURE_NONE);

		// 矩形の位置の設定
		SetPosRectangle(pResult->nIdx, pos, size);

		// 矩形の描画するかどうか
		SetDrawRectangle(pResult->nIdx, false);

		pResult->bResult = false;
		pResult->bDraw = false;
	}

	// 矩形の設定
	s_draw.nIdx = SetRectangle(TEXTURE_NONE);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// 矩形の位置の設定
	SetPosRectangle(s_draw.nIdx, pos, size);

	// 矩形の描画するかどうか
	SetDrawRectangle(s_draw.nIdx, false);

	s_draw.bDraw = false;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitResult(void)
{
	
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetGameState() != GAMESTATE_RESULT)
	{// リザルト状態ではない
		return;
	}

	s_nTime++;

	if (s_nTime >= LIMIT_TIME)
	{// 時間が越した
		s_nTime = 0;

		// モードの変更
		ChangeMode(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{//決定キー(ENTERキー)が押されたかどうか
		// モードの変更
		ChangeMode(MODE_TITLE);
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawResult(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 勝敗の設定
//--------------------------------------------------
void SetResult(int nPlayerNo, bool bResult)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	Result *pResult = &s_result[nPlayerNo];

	pResult->bResult = bResult;

	if (bResult)
	{// 勝ち
		// 矩形のテクスチャの設定
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Disc);
	}
	else
	{// 負け
		// 矩形のテクスチャの設定
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Select_Left);
	}
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawResult(bool bDraw)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_result[i];

		pResult->bDraw = bDraw;

		// 矩形の描画するかどうか
		SetDrawRectangle(pResult->nIdx, bDraw);
	}
}
