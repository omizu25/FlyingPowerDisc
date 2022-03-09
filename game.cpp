//==================================================
//
// FPD制作 ( game.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "disc.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "rectangle.h"
#include "sound.h"
#include "rule.h"

#include <assert.h>

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		// ゲームの状態
static int				s_nCounterState;					// 状態管理カウンター
static bool				s_bPause = false;					// ポーズ中かどうか [してる  : true してない  : false]

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	// 矩形の初期化
	InitRectangle();

	// ディスクの初期化
	InitDisc();

	//ルール選択画面の初期化
	InitRule();

	//ルール選択画面の設定
	SetRule(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	s_gameState = GAMESTATE_START;		// 開始状態に設定

	s_nCounterState = 0;				// カウンターの初期化

	s_bPause = false;					// ポーズ解除
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//ルール選択画面の終了
	UninitRule();

	// 矩形の終了
	UninitRectangle();

	// ディスクの終了
	UninitDisc();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START, 0))
	{// Pキーが押されたかどうか
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{// ポーズ中
		return;
	}

	switch (s_gameState)
	{
	case GAMESTATE_NONE:		// 何もしていない状態
		assert(false);
		break;

	case GAMESTATE_START:		// 開始状態
		s_gameState = GAMESTATE_NORMAL;
		break;

	case GAMESTATE_NORMAL:		// 通常状態
		//ルール選択画面の更新
		UpdateRule();

		// ディスクの更新
		UpdateDisc();

		break;

	case GAMESTATE_END:			// 終了状態

		break;

	case GAMESTATE_RESULT:		// リザルト状態

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	if (s_bPause)
	{// ポーズ中
		return;
	}

	//ルール選択画面の描画
	DrawRule();

	// 矩形の描画
	DrawRectangle();}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;
	s_nCounterState = 0;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
GAMESTATE GetGameState(void)
{
	return s_gameState;
}

//--------------------------------------------------
// ポーズの有効無効設定
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}
