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
const float	MAX_WIDTH = 400.0f;		// 幅の最大値
const float	MAX_HEIGHT = 300.0f;	// 高さの最大値
const float	DRAW_WIDTH = 700.0f;	// 幅の最大値
const float	DRAW_HEIGHT = 350.0f;	// 高さの最大値
const int	LIMIT_TIME = 300;		// 時間の限界

/*↓ 勝敗 ↓*/

typedef struct
{
	int		nIdx;	// 矩形のインデックス
	RESULT	result;	// 勝敗
}Result;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Result	s_aResult[MAXPLAYER];	// 勝敗の情報
int		s_nIdxDraw;				// 引き分けの情報
int		s_nTime;				// 時間
}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		D3DXVECTOR3 size = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.6f, 0.0f);

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

		pResult->result = RESULT_NONE;
	}

	{// 引き分け
		// 矩形の設定
		s_nIdxDraw = SetRectangle(TEXTURE_Result_Draw);

		D3DXVECTOR3 size = D3DXVECTOR3(DRAW_WIDTH, DRAW_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxDraw, pos, size);

		// 矩形の描画するかどうか
		SetDrawRectangle(s_nIdxDraw, false);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitResult(void)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		// 使うのを止める
		StopUseRectangle(pResult->nIdx);
	}

	// 使うのを止める
	StopUseRectangle(s_nIdxDraw);
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

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_START) ||
		GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
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
void SetResult(int nPlayerNo, RESULT result)
{
	assert(nPlayerNo >= 0 && nPlayerNo < MAXPLAYER);

	Result *pResult = &s_aResult[nPlayerNo];

	pResult->result = result;

	switch (result)
	{// 矩形のテクスチャの設定
	case RESULT_WIN:	// 勝ち
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Result_Win);
		break;

	case RESULT_LOSE:	// 負け
		ChangeTextureRectangle(pResult->nIdx, TEXTURE_Result_Lose);
		break;

	case RESULT_DRAW:	// 引き分け
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawResult(bool bDraw)
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		Result *pResult = &s_aResult[i];

		if (pResult->result == RESULT_DRAW)
		{// 引き分け
			// 矩形の描画するかどうか
			SetDrawRectangle(s_nIdxDraw, true);

			continue;
		}

		// 矩形の描画するかどうか
		SetDrawRectangle(pResult->nIdx, bDraw);

		// 矩形の描画するかどうか
		SetDrawRectangle(s_nIdxDraw, false);
	}
}
